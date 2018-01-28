/* This sketch want provide help for testing and calibrate battery value from Arduino pin
   with low battery alarm functionality through OLED display(without connecting buzzer)

   NOTES and safety WARNING:

   - Use linear voltage variable power supply to simulate voltage battery, when it possible.
   
   - If you want correct visualization of values after uploading sketch in Arduino
     is necessary remove USB programming cable from PC because it makes the readings unstable.
       
   - For 2S voltage divider do not over 10V, For 3S voltage divider do not over 15V.
     otherwise Arduino pin will be fried.
     
   - WARNING if you want to reset ALARM OLED display indication
     is necessary to put battery voltage to the maximum and restart Arduino with reset button  

   - For testing you can modify these parameters:

   ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
   const int numReadings = 8;  // Number of reading to calculate average
   lower value means fast reaction, high value slow reaction (8 value is maximum)
   ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

   ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
   #define minBat 70     // Minimum battery voltage for alarm (70 = 7.0V)for 2S LiOn/LiPo battery
   #define maxBat 82     // Maximum battery voltage (10V) (82 = 8.2V)for 2S LiOn/LiPo battery
   ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

   ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
   #define minBat 100    // Minimum battery voltage for alarm (100 = 10V)for 3S LiOn/LiPo battery
   #define maxBat 125    // Maximum battery voltage (125 = 12.5V)for 3S LiOn/LiPo battery
   ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

   If you need more precision, measure voltage divider resistors value
   with digital multimeter and put them in to R1/R2 variables and trim these values
   ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
   float R1 = 10430;
   float R2 = 10055;
   ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

  //---------------------------------------------------------------------------------------------
  // Must be used R1-10K/R2-10K voltage divider for 2S lion/LiPo battery
  // (R1-20K/R2-10K voltage divider for 3S lion/LiPo battery)
  //---------------------------------------------------------------------------------------------

  batPin = A7

  (Default battery voltage divider for 2S lion/LiPo)

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

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

  (Battery voltage divider for 3S lion/LiPo)

  Vbatt MAX 15V ----+
                   |
                   R1 20K
                   |
                   |+-----> (PIN A7)
                   |
                   R2 10K
                   |
                   =
                   .  (GND)
*/

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include <SPI.h>                   // Arduino standard library
#include <U8g2lib.h>               // Using U8G2 library "https://github.com/olikraus/u8g2"
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// Set library for SH1106 SPI 1.3" or SSD1306 0.96" OLED display TESTED OK! (128 bytes buffer)
U8G2_SH1106_128X64_NONAME_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/* SPI wires cabling (OLED Pin name dependent on the manufacturer)

  OLED   Arduino
  ---------------------
  RES  > pin 8
  DC   > pin 9
  CS   > pin 10 (SS)
  MOSI > pin 11 (MOSI)
  CLK  > pin 13 (SCK)*/

#define buzzer PD6                   // Buzzer pin (buzzer with standalone sound generator)

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define minBat 100                    // Minimum battery voltage for alarm (100 = 10V)
#define maxBat 125                    // Maximum battery voltage (125 = 12.5V)
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Battery voltage calculation, checking status for alarm, drawing battery bar
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
const int numReadings = 8;           // Number of reading to calculate average
int readings[numReadings];           // Readings from the analog input (battery input pin A7)
int index = 0;                       // Index of the current reading
int total = 0;                       // Running total
int average = 0;                     // Average value

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
float pin_batt_read;                 // Raw input battery pin value from average reading
float batt_volt;                     // Battery voltage calculated
float res_calc;                      // Calculation of voltage divider to be applied to the conversion
float R1 = 10000;                    // If you need more precision, measure voltage divider resistors value
float R2 = 10000;                    // with digital multimeter and put them in to R1 R2 variables
unsigned char perc_batt;             // Battery bar level indicator (20 pixel) only Min to Max values

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
float batt_display;                  // Battery voltage on display
unsigned char perc_display;          // Battery percentage on display

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
boolean battStatus = 1;

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
byte pulseBuzzer;                       // Sound pulse for battery alarm

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
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

  res_calc = (R1 + R2) / R2;   // Voltage divider calculation

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // Initialize value for average ADC readings
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

  for (int thisReading = 0; thisReading < numReadings; thisReading++)

    readings[thisReading] = 0; // initialize all the readings to 0

  // End of Setup +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

  pinMode(buzzer, OUTPUT);

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // Set default font type used for all display sessions (mandatory)
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  u8g2.setFont(u8g2_font_7x14_tr);

}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Macro for directly read ADC value on all analog ports
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
unsigned short int read_adc(unsigned char channel) {

  ADMUX &= 0xF0;                        //Clear the older channel that was read
  ADMUX |= channel;                     //Defines the new ADC channel to be read
  ADCSRA |= (1 << ADSC);                //Starts a new conversion
  while (ADCSRA & (1 << ADSC));         //Wait until the conversion is done
  return ADCW;                          //Returns the ADC value of the chosen channel
}

void loop() {

  batteryCheck();
  Screen_0();

}
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Drawing Main Screen display
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void Screen_0() {

  // Start of main screen display informations "0" ************************
  u8g2.firstPage();
  do {

    // Battery simbol contruction
    u8g2.drawFrame(102, 0, 24, 8);             // Battery box
    u8g2.drawBox(104, 2, perc_batt, 4);        // level bar
    u8g2.drawBox(126, 2, 2, 3);                // Battery nipple plus pole

    u8g2.setCursor(10, 10);
    u8g2.print(batt_display);

    u8g2.setCursor(50, 10);
    u8g2.print("Volt");

    u8g2.setCursor(10, 25);
    u8g2.print(perc_batt);

    u8g2.setCursor(40, 25);
    u8g2.print("Batt pixels");

    u8g2.setCursor(10, 40);
    u8g2.print(perc_display);

    u8g2.setCursor(40, 40);
    u8g2.print("Batt %");

    if (perc_batt <= 1 || battStatus == 0) {
      u8g2.setCursor(10, 55);
      u8g2.print("Alarm ON");
    }
  } while (u8g2.nextPage());
}
// End of main screen display informations "0" **************************


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Macro to calculate average ADC value, checking battery voltage alarm and value for battery bar
// For minBat do not under default value in config file because arduino will be more unstable.
// About 16%(presumed)of battery is enough to landing ASAP
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void batteryCheck() {

  // Subtract the last reading:
  total = total - readings[index];

  // Read from adc input:
  readings[index] = read_adc(7);

  // Add reading to total:
  total = total + readings[index];

  // Advance to the next position in the array:
  index = index + 1;

  // If we're at the end of the array...
  if (index >= numReadings)

    // ...wrap around to the beginning:
    index = 0;

  // Calculate the average:
  average = total / numReadings;

  // Mapping battery voltage from average from 0 to 5V
  pin_batt_read = map(average, 0, 1023, 0, 500);

  // Calculating correct voltage value through voltage divider
  // Result = 2 digits with 2 decimal (eg 72 = 7.2V)
  batt_volt = pin_batt_read * res_calc / 10;
  batt_display = pin_batt_read * res_calc / 100;
  //Serial.println (batt_display);

  // Mapping voltage for drawing battery bar
  perc_batt = map(batt_volt, minBat, maxBat, 1, 20);
  perc_batt = constrain(perc_batt, 1, 20);

  // Mapping voltage for display battery percentage
  perc_display = map(batt_volt, minBat, maxBat, 1, 100);
  perc_display = constrain(perc_display, 1, 100);

  // Serial.println (perc_batt);

  // Low battery beep alarm
  if (perc_batt == 1 || battStatus == 0) {

    // Set byte 0 to mantain battery alarm even voltage floating +/- on minBat value
    battStatus = 0;

    // Set value to mantain battery bar to min value even voltage is under minBat
    perc_batt = 1;

    // Recall pulse for buzzer
    beep();
  }

  // Buzzer alarm OFF
  else {
    PORTD &= ~_BV(buzzer);
  }
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Macro for buzzer beep battery alarm
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void beep () {

  if ((bitRead(pulseBuzzer, 0) && bitRead(pulseBuzzer, 5) && bitRead(pulseBuzzer, 1))) {
    PORTD |= _BV(buzzer);
  }
  else {
    PORTD &= ~_BV(buzzer);
  }
  ++pulseBuzzer;
}
