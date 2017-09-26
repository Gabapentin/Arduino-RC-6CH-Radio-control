// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Set library for SSD1306 SPI 0.96" OLED display TESTED OK! (128 bytes buffer)
// U8G2_SSD1306_128X64_NONAME_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);

// Set library for SH1106 SPI 1.3" OLED display TESTED OK! (128 bytes buffer)
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

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// config PPM settings
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define servoCenter 1500             // Servo center value (us)
#define ppmLong 18500                // ppm frame (us) for 6 channels
#define ppmPulse 300                 // ppm pulse width (us)
#define onState 0                    // Polarity: 0 neg / 1 pos
#define sigPin PD7                   // ppm output pin
#define ppmMin 1000                  // Min ppm output to be mapped (us)
#define ppmMax 2000                  // Max ppm output to be mapped (us)

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Config global TX param
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define CHANNELS 6                   // Number of channels
#define MODELS 5                     // Total memory models 
#define potMin 0                     // Pot Min reference value
#define potMax 1023                  // Pot Max reference value
#define numBytesPerModel 32          // Maximum bytes for data storage per model

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Config pin assignment
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define swMix 0                      // V-Tail mixer switch (active LOW)
#define swTC 4                       // Throttle Cut pin (active LOW)
#define swDR 5                       // Dual Rate pin (active LOW)
//#define batPin A7                    // Battery pin (over 5V need voltage divider and value conversion to display)
#define buzzer PD6                   // Buzzer pin (buzzer with standalone sound generator)
#define buttonSelect 1               // Button for Menu/Select functions
#define buttonUp 2                   // Button for Up/Prev functions
#define buttonDown 3                 // Button for Down/Next functions
#define swA A6                       // Switch A Analog pin (active 0)
#define swB 12                       // Switch B Digital pin (active LOW)

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define minBat 70                    // Minimum battery voltage for alarm (70 = 7.0V)
#define maxBat 82                    // Maximum battery voltage (10V) (82 = 8.2V)
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// XBM image size for Model Selection screen
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define airplane_width 55            // Pixel width of airplane image
#define airplane_height 46           // Pixel height of airplane image
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

unsigned char chanTh = 2;                               // Throttle channel ID
unsigned char sw_status = 0x00;                         // 0:TC 1:DR
unsigned short int ppm[CHANNELS];                       // ppm output array
unsigned short int pots[CHANNELS];                      // Input ADC data array
unsigned short int calibration[CHANNELS][2];            // Min/Max value array for read channels and store calibration
unsigned short int raw_Pots[CHANNELS];                  // Read raw input pots array

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Menu management variables
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
unsigned char menuActual = 0;
unsigned char menuSubActual = 0;
unsigned char modelActual = 0;                // Added for Model Menu management
unsigned char menuSubModel = 0;               // Added for Model Menu management
unsigned char screen = 0;

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Servo management parameters
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
unsigned char drSelection = 0xFF;             // Default value for Dual Rates
unsigned char servoReverse;                   // Reading bit status
unsigned char dual_rate_low[CHANNELS];        // Dual Rates LOW value array
unsigned char dual_rate_hi[CHANNELS];         // Dual Rates HIGH value array
unsigned short int NO_DR_VRA;                 // For pot VRA normal mapping else not working(???)
unsigned short int NO_DR_VRB;                 // For pot VRB normal mapping else not working(???)

unsigned short int  deadBand = 50;            // Deadband center stick value (25 suggested value)
unsigned short int  subTrim[CHANNELS];        // Sub Trim channel array
unsigned short int  subTrimSelection = 512;   // Default value for Sub Trim

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Checking conditions control status for warning switch and calibration
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
boolean warnSwitch = 1;                 // One beep control for switch and throttle warning
boolean onStartup = 1;                  // Status for checking switch and throttle position
boolean statusCalib = 1;                // Checking status to enter in calibration procedure
boolean battStatus = 1;
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

byte pulseBuzzer;                       // Sound pulse for battery alarm

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Battery voltage calculation, checking status for alarm, drawing battery bar  
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
const int numReadings = 8;           // Number of reading to calculate average
int readings[numReadings];           // Readings from the analog input (battery input pin A7)
int index = 0;                       // Index of the current reading
int total = 0;                       // Running total
int average = 0;                     // Average value

float pin_batt_read;                 // Raw input battery pin value from average reading
float batt_volt;                     // Battery voltage calculated
float res_calc;                      // Calculation of voltage divider to be applied to the conversion
float R1 = 10000;                    // If you need more precision, measure voltage divider resistors value
float R2 = 10000;                    // with digital multimeter and put them in to R1 R2 variables
unsigned char perc_batt;             // Battery bar level indicator (20 pixel) only Min to Max values

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Using PROGMEM for characters and text strings to save RAM memory
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Array text strings for drawing menu
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
const char menu_0[] PROGMEM = "SERVO DIRECTION";
const char menu_1[] PROGMEM = "DUAL RATES";
const char menu_2[] PROGMEM = "MODEL SELECTION";
const char menu_3[] PROGMEM = "SAVE MODEL DATA";
const char menu_4[] PROGMEM = "SUB TRIM SETUP";
const char menu_5[] PROGMEM = "SUB TRIM";

const char* const menu_name[] PROGMEM = {
  menu_0,
  menu_1,
  menu_2,
  menu_3,
  menu_4,
  menu_5
};

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Array text strings for screen messages
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
const char Message_0[] PROGMEM = "SAVE DATA";
const char Message_1[] PROGMEM = "MODEL";
const char Message_2[] PROGMEM = "CALIBRATION";
const char Message_3[] PROGMEM = "Calibration Started";
const char Message_4[] PROGMEM = "Move the sticks Pots";
const char Message_5[] PROGMEM = "ERASE DATA";
const char Message_6[] PROGMEM = "WARNING !!!";
const char Message_7[] PROGMEM = "SET All SWITCH STICK";
const char Message_8[] PROGMEM = "TO DEFAULT POSITION!";

const char* const messages[] PROGMEM = {
  Message_0,
  Message_1,
  Message_2,
  Message_3,
  Message_4,
  Message_5,
  Message_6,
  Message_7,
  Message_8
};

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Array text strings (short) for Channel/Switch/Pots names, status, model for all screens
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
const char chName_0[] PROGMEM = "AIL";
const char chName_1[] PROGMEM = "ELE";
const char chName_2[] PROGMEM = "THR";
const char chName_3[] PROGMEM = "RUD";
const char chName_4[] PROGMEM = "CH5";
const char chName_5[] PROGMEM = "CH6";
const char chName_6[] PROGMEM = "SWA";
const char chName_7[] PROGMEM = "SWB";
const char chName_8[] PROGMEM = "MOD";
const char chName_9[] PROGMEM = "TC";
const char chName_10[] PROGMEM = "DR";
const char chName_11[] PROGMEM = "VRA";
const char chName_12[] PROGMEM = "VRB";
const char chName_13[] PROGMEM = "NOR";
const char chName_14[] PROGMEM = "REV";
const char chName_15[] PROGMEM = "ON";
const char chName_16[] PROGMEM = "OFF";
const char chName_17[] PROGMEM = "MIX";
const char chName_18[] PROGMEM = "MODEL";
const char chName_19[] PROGMEM = "PPM";

const char* const channel_name[] PROGMEM = {
  chName_0,
  chName_1,
  chName_2,
  chName_3,
  chName_4,
  chName_5,
  chName_6,
  chName_7,
  chName_8,
  chName_9,
  chName_10,
  chName_11,
  chName_12,
  chName_13,
  chName_14,
  chName_15,
  chName_16,
  chName_17,
  chName_18,
  chName_19
};

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Array for single characters
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
const char character_0[] PROGMEM = "A";
const char character_1[] PROGMEM = "E";
const char character_2[] PROGMEM = "T";
const char character_3[] PROGMEM = "R";
const char character_4[] PROGMEM = "N";
const char character_5[] PROGMEM = "H";
const char character_6[] PROGMEM = "L";
const char character_7[] PROGMEM = "M";
const char character_8[] PROGMEM = "[";
const char character_9[] PROGMEM = "]";
const char character_10[] PROGMEM = "=";
const char character_11[] PROGMEM = "(";
const char character_12[] PROGMEM = ")";
const char character_13[] PROGMEM = "*";
const char character_14[] PROGMEM = ">";

const char* const one_char[] PROGMEM = {
  character_0,
  character_1,
  character_2,
  character_3,
  character_4,
  character_5,
  character_6,
  character_7,
  character_8,
  character_9,
  character_10,
  character_11,
  character_12,
  character_13,
  character_14
};

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Airplane XBM bitmap for Model Selection screen
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
static const unsigned char airplane[] U8X8_PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0xFF, 0x3F, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x22, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x63,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x6B, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x77, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x63, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x63, 0x00,
  0x00, 0x00, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x3F, 0x02, 0x00, 0x00,
  0x41, 0x00, 0x00, 0x20, 0x02, 0x00, 0x00, 0x41, 0x00, 0x00, 0x20, 0x02,
  0x00, 0x00, 0x41, 0x00, 0x00, 0x20, 0x02, 0x00, 0x00, 0x41, 0x00, 0x00,
  0x20, 0x02, 0x00, 0x00, 0x41, 0x00, 0x00, 0x20, 0x02, 0x00, 0x00, 0x41,
  0x00, 0x00, 0x20, 0x06, 0xF0, 0x1F, 0x41, 0xFC, 0x07, 0x30, 0xFC, 0x11,
  0x70, 0x7F, 0x07, 0xC4, 0x1F, 0x00, 0x1F, 0x40, 0x63, 0x01, 0x7C, 0x00,
  0x00, 0xF0, 0x9F, 0xC1, 0xFC, 0x07, 0x00, 0x00, 0x00, 0xF0, 0xE3, 0x07,
  0x00, 0x00, 0x00, 0x00, 0x80, 0xE3, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x77, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1C,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xFF, 0xF7, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x77, 0x7C, 0x00, 0x00,
  0x00, 0x00, 0x03, 0x77, 0x60, 0x00, 0x00, 0x00, 0x00, 0x01, 0x36, 0x40,
  0x00, 0x00, 0x00, 0x00, 0x03, 0x36, 0x60, 0x00, 0x00, 0x00, 0x00, 0xFF,
  0xF7, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1C,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
