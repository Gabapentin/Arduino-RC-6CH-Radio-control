// This code is based on ardutx v1.3 sketch
// (c) 2014 Patricio Reinoso
// www.patolin.com

/* This project is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
  this code is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.
  You should have received a copy of the GNU General Public License
  along with this code. If not, see <http://www.gnu.org/licenses/>.*/

// *************************************************************************************************************
// A great Patolin idea to upgrade cheap Hobbyking HK-T6A or Flysky FS-CT6B transmitters or any other old RC TX
// These transmitters offer a configuration through a serial port with an external PC software program
// and no space to store more than one model.
// *************************************************************************************************************
// This sketch has been modified to work with SH1106 SPI 1.3" or SSD1306 SPI 0.96" OLED display through U8g2 library
// Has been used all possible pins of ATmega328 (Arduino UNO, Nano, Pro Mini)
// Every text strings has been stored in flash memory to mantain a lot of free RAM memory
// Sketch terms, text, items has been translated in English to ease users/developers
// *************************************************************************************************************
// Added features to original Patolin code:
// - Support for SH1106 SPI 1.3" or SSD1306 SPI 0.96"(128x64)(U8g2 library)
// - Switches checking status with sound alert at power ON and display which is not in default position
// - Throttle warning with display status and sound alert
// - Hidden data default restoring by pressing and hold Down/Next button on power on
// - Hidden calibration procedure by pressing and hold Menu/Select button on power on
// - Low battery control in real time whith buzzer sound(buzzer with stand alone sound generator)
// - N°2 supplementary switch, they can be configured instead of VRA or VRB pots(SWA, SWB)(Servo Direction Menu)
// - Basic two channel mixer with switch activation V-Mix (Ch1/ch2)
// - Basic four channels Sub Trim setting (ch1, ch2, ch3, ch4)
// *************************************************************************************************************
// Recommended Arduino IDE 1.8.2 // Seems to be optimized and stable compiler. Less Flash and RAM used
// Arduino AVR Boards 1.6.18
// *************************************************************************************************************
/*
  +++++++++++++++++
  Input Pots
  6 channels
  +++++++++++++++++
  Aileron  CH1 = A0
  Elevon   CH2 = A1
  Throttle CH3 = A2
  Rudder   CH4 = A3
  Pot VRA  CH5 = A4
  Pot VRB  CH6 = A5

  //---------------------------------------------------------------------------------------------
  // 2 position switch (pin A6 in the center, VCC and GND at both ends)
  // NOTE: switch is activated when pin A6 is closed to GND (normaly closed to VCC)
  //---------------------------------------------------------------------------------------------
  swA = A6

  //---------------------------------------------------------------------------------------------
  // Must be used R1-10K/R2-10K voltage divider for 2S lion/LiPo battery
  // (R1-20K/R2-10K voltage divider for 3S lion/LiPo battery)
  //---------------------------------------------------------------------------------------------
  batPin = A7

  (Battery voltage divider)

  Vbatt MAX 10V ----+
                   |
                   R1 10K
                   |
                   |+-----> (PIN A7)
                   |
                   R2 10K
                   |
                   =
                   .  (GND)

  //---------------------------------------------------------------------------------------------
  // V-Mix switch
  // WARNING !!! If closed to GND will deny to upload sketch
  //---------------------------------------------------------------------------------------------
  swMix = pin 0[RXD]

  //---------------------------------------------------------------------------------------------
  // Mount 0,1uF/100nF(104)ceramic capacitor from pin to GND
  // WARNING !!! the capacitor on pin 1[TXD](buttonSelect) will deny to upload sketch
  //---------------------------------------------------------------------------------------------
  Enter Button pin 1(TXD)
  UP Button    pin 2
  Down Button  pin 3

  //---------------------------------------------------------------------------------------------
  // 2 position switch (is activated when pin is closed to GND)
  //---------------------------------------------------------------------------------------------
  swDR         pin 4
  swTC         pin 5
  swB          Pin 12

  //---------------------------------------------------------------------------------------------
  buzzer       pin 6   // Buzzer with sound generator (need transistor driving)

  //---------------------------------------------------------------------------------------------
  // Must be used voltage divider on pin 7 to limit at 3Vpp ppm signal
  // because T6A/CT6B(A7105)TX module is not 5V tollerant
  //---------------------------------------------------------------------------------------------
  sigPin       pin 7

  (PPM Signal voltage divider)

  sigPin 7 ----------+
                   |
                   R1 10K
                   |
                   |+-----> 2.7Vpp(to TX PPM PIN)
                   |
                   R2 12K(Max 15K)
                   |
                   =
                   .  (GND)
*/
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include <SPI.h>                   // Arduino standard library
#include <EEPROM.h>                // Arduino standard library
#include <U8g2lib.h>               // Using U8G2 library "https://github.com/olikraus/u8g2"
#include "Config.h"                // Load static and variable configuration settings
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void setup() {

  //Serial.begin(9600);

  // LCD config with U8G2 library display init (mandatory)
  u8g2.begin();

  // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // Input ADC sampling rate for reading analog input pots (1000 KHz is maximum for stable reading)
  // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

  ADCSRA &= ~(1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);   // Remove bit settings from Arduino library
  ADCSRA |= (1 << ADPS2);                                  // Set ADC prescaler to 16 (1000 KHz)

  ADMUX |= (1 << REFS0);
  ADMUX &= ~(1 << REFS1);                                  // Avcc(+5v) as voltage reference

  ADCSRB &= ~((1 << ADTS2) | (1 << ADTS1) | (1 << ADTS0)); // ADC in free-running mode

  ADCSRA |= (1 << ADEN);                                   // Turn on ADC
  ADCSRA |= (1 << ADSC);                                   // Do an initial conversion this one is the slowest to ensure that everything is up and running

  DIDR0 = 0x3F;                                            // Digital circuit on analog ports disabled
  DIDR1 = 0x03;                                            // Disable digital input on AIN0/1

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // Debouncing mechanical buttons
  // NOTE: For input pin buttons is necessary to mount on every pin
  // 0,1uF/100nF(104)ceramic capacitors from pin to GND
  // WARNING !!! the capacitor on pin 1(buttonSelect) will deny to upload sketch
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  pinMode(buttonSelect, INPUT);          // Pull up is not necessary (pin on LED TXD)
  pinMode(buttonUp, INPUT_PULLUP);
  pinMode(buttonDown, INPUT_PULLUP);
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  pinMode(swTC, INPUT_PULLUP);
  pinMode(swDR, INPUT_PULLUP);
  pinMode(swB,  INPUT_PULLUP);
  pinMode(swMix, INPUT);                 // Pull up is not necessary (pin on LED RXD)
  pinMode(buzzer, OUTPUT);
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

  res_calc = (R1 + R2) / R2;             // Voltage divider calculation

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // Initialize value for average ADC readings
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

  for (int thisReading = 0; thisReading < numReadings; thisReading++)

    readings[thisReading] = 0; // initialize all the readings to 0

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // Default state values for ppm
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  ppm[0] = servoCenter;     // channel 1 (A)
  ppm[1] = servoCenter;     // channel 2 (E)
  ppm[2] = ppmMin;          // channel 3 (T)
  ppm[3] = servoCenter;     // channel 4 (R)
  ppm[4] = ppmMin;          // channel 5 (VRA,SWA)
  ppm[5] = ppmMin;          // channel 6 (VRB,SW2)

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // Default state config parameters
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

  // Servo Direction bit mask: 0 Normal, 1 Reverse
  servoReverse = 0b00000000;

  // Dual Rates and Sub Trim default values (only for first four channels)
  for (int i = 0; i < CHANNELS - 2; i++) {

    dual_rate_low[i] = 75;
    dual_rate_hi[i] = 100;
    subTrim[i] = 512;
  }

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // Config output signal pin
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  DDRD |= _BV(sigPin);  // Set pin 7 as an output
  PORTD ^= _BV(sigPin); // Set pin 7 toggle state

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // ppm timer config
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  configTimer1();

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // Hold button "Down" on power ON to initialize default memory data
  // Will only write data settings to default (not erase calibration data)
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

  // Delay before reading button (about charge capacitor pulse on pin)
  delay(100);

  if ((PIND & (1 << 3)) == 0) {

    // Recall "Reset to default" macro
    resetEeprom();

    u8g2.firstPage();
    do {

      // Set font type
      u8g2.setFont(u8g2_font_fub14_tr);

      // Set memory buffer for text strings
      char msg_buffer[21];

      // Print "ERASE DATA" text string
      strcpy_P(msg_buffer, (char*)pgm_read_word(&(messages[5])));
      u8g2.setCursor(0, 40);
      u8g2.print(msg_buffer);

    } while (u8g2.nextPage());

    delay(2000); // Screen message for 2sec)
  }

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // Load data from Eeprom
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  modelActual = storedDataEeprom(255);
  // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // Set default font type used for all display sessions (mandatory)
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  u8g2.setFont(u8g2_font_6x10_tr);
}

// End of Setup +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void loop() {

  // Start Calibration screen if button Enter/Select is pressed on power on ***********************

  if (readKeys() == 2 && statusCalib == 1) {

    Calibration();    // Recall calibration procedure
  }

  // Set condition 0 to continue loop if calibration procedure is not selected
  else {
    statusCalib = 0;
  }

  // Start of checking switches default status ****************************************************

  readSwitches();     // Read sw_status switches values

  // This conditions check all switches and throttle status on startup if not in default position
  while (onStartup == 1 && (
           bitRead(sw_status, 0) == 0 ||
           bitRead(sw_status, 1) == 0 ||
           bitRead(sw_status, 2) == 0 ||
           bitRead(sw_status, 3) == 0 ||
           bitRead(sw_status, 4) == 0 ||
           bitRead(sw_status, 5) == 0 ))
  {
    WarnSwitch();     // Recall switch warning screen
  }

  // Set condition = 0 to continue loop after all switches are in default mode
  onStartup = 0;

  // Starting main session *************************************************************************

  readPots();                 // Recall macro for calculating ppm output

  readSwitches();             // Read switches values

  batteryCheck();             // Checking battery status

  // Start of screen display management ************************************************************

  if (screen == 0) {          // Print and drawing main screen
    Screen_0();
  }
  else {
    if (screen == 1) {        // Print menu selection screen
      Screen_1();
    }
    if (screen == 2) {

      if (menuActual == 1) {  // Execute "Servo Direction" task
        Menu_1();
      }
      if (menuActual == 2) {  // Execute "Dual Rates" task
        Menu_2();
      }
      if (menuActual == 3) {  // Execute "Model Selection" task
        Menu_3();
      }
      if (menuActual == 4) {  // Execute "Save Model Data" task
        Menu_4();
      }
      if (menuActual == 5) {  // Execute "Sub Trim Setup" task
        Menu_5();
      }
    }
  }
  // If pressed buttonSelect/Enter
  if (readKeys() == 2) {
    if (screen < 2) {
      screen++;
      menuActual = menuSubActual;
      menuSubActual = 0;
      menuSubModel = 0;
    }
    else {
      //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
      // Added if Menu/Select button is pressed without menu cursor (do nothing)
      //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
      if (menuActual == 0) {
        screen--;
        menuActual--;
      }
      //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
      // Options for each menu
      //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
      if (menuActual == 1) {

        // Selection value for Servo Direction
        bitWrite(servoReverse, menuSubActual, !bitRead(servoReverse, menuSubActual));
      }
      if (menuActual == 2) {

        // Selection value for Dual Rates
        if (drSelection != 0xFF) {
          drSelection = 0xFF;
        }
        else {
          drSelection = menuSubActual;
        }
      }

      // Model Selection data
      if (menuActual == 3) {

        // Selection value for Model Selection
        modelActual = menuSubModel;

        // Save data in eeprom
        EEPROM.update(511, modelActual);
        storedDataEeprom(modelActual);

        delay(10);
      }

      if (menuActual == 5) {

        // Selection value for Sub Trim
        if (subTrimSelection != 512) {
          subTrimSelection = 512;
        }
        else {
          subTrimSelection = menuSubActual;
        }
      }
    }
  }

  if (readKeys() == 3) {
    switch (menuActual) {

      //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
      // Dual Rate screen step control for selecting channels/values (buttonDown)
      //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
      case 2:
      
      // Initial value for Dual Rates selection value
        if (drSelection == 0xFF) {

          // Only first four channels
          if (menuSubActual < (CHANNELS * 2) - 5) {  // Two values * four channels
            menuSubActual++;
            if (screen == 0) {
              screen++;
            }
          }
        }
        // Dual Rates step control for changing selected channel value(DOWN)
        else {
          if ((drSelection % 2) == 0) {

            // Define Min for LOW Rates value
            if (dual_rate_low[drSelection / 2] > 0) {

              // Define step value
              dual_rate_low[drSelection / 2]--;
            }
          }
          else {

            // Define Min for HIGH Rates value
            if (dual_rate_hi[drSelection / 2] > 0) {

              // Define step value
              dual_rate_hi[drSelection / 2]--;
            }
          }
        }

        break;

      //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
      // Sub Trim screen step control for selecting channels/values (buttonDown)
      //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
      case 5:
      
      // Initial value for Sub Trim selection value
        if (subTrimSelection == 512) {

          // Only first four channels
          if (menuSubActual < CHANNELS - 3) {
            menuSubActual++;
            
            if (screen == 0) {
              screen++;
            }
          }
        }

        // Sub Trim step control for changing selected channel value DOWN
        else {
          if ((subTrimSelection % 1)  == 0) {

            // Define Min Sub trim value
            if (subTrim[subTrimSelection / 1] > 312 - deadBand ) {

              // Define step value
              subTrim[subTrimSelection / 1]--;
            }
          }
        }
        break;
      // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

      default:
        //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        // Added for Model Menu management
        //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        if (menuSubModel < 4) {   // 0 to 4 items
          menuSubModel++;
          if (screen == 0) {
            screen++;
          }
        }
        //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

        if (menuSubActual < 5) {  // 0 to 5 items
          menuSubActual++;
          if (screen == 0) {
            screen++;
          }
        }
        break;
        //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    }
  }

  if (readKeys() == 1) {
    switch (menuActual) {

      //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
      // Dual Rate screen step control for selecting channels/values (buttonUp)
      //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
      case 2:
      
      // Initial value for Dual Rates selection value
        if (drSelection == 0xFF) {
          if (menuSubActual == 0) {
            screen--;
            menuSubActual = 1;
            menuActual = 0;
          }
          else {
            menuSubActual--;
          }
        }

        // Step control for changing selected channel value UP
        else {
          if ((drSelection % 2) == 0) {

            // Define Max for LOW Rates value
            if (dual_rate_low[drSelection / 2] < 100) {

              // Define step value
              dual_rate_low[drSelection / 2]++;
            }
          }
          else {

            // Define Max for HIGH Rates value
            if (dual_rate_hi[drSelection / 2] < 100) {

              // Define step value
              dual_rate_hi[drSelection / 2]++;
            }
          }
        }
        break;

      //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
      // Sub Trim screen step control for selecting channels/values (buttonUp)
      //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
      case 5:
        if (subTrimSelection == 512) {
          if (menuSubActual == 0) {
            screen--;
            menuSubActual = 1;
            menuActual = 0;
          }
          else {
            menuSubActual--;
          }
        }

        // Sub Trim step control for changing selected channel UP
        else {
          if ((subTrimSelection % 1) == 0 ) {

            // Define max value for Sub Trim
            if ((subTrim[subTrimSelection / 1]) < (712 + deadBand)) {

              // Define step value
              subTrim[subTrimSelection / 1]++;
            }
          }
        }
        break;

      //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

      default:
        //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        // Added for Model Menu management
        //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        if (menuSubModel > 0) {
          menuSubModel--;
        }
        else {
          if (screen > 0) {
            screen--;
            menuSubModel = 1;
            menuActual = 0;
          }
        }
        //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

        if (menuSubActual > 0) {
          menuSubActual--;
        }
        else {
          if (screen > 0) {
            screen--;
            menuSubActual = 1;
            menuActual = 0;
          }
        }
        break;

        //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    }
  }
}
