// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// This macro will be used to clear eeprom and set default parameters if necessary
// Hold button "Down" on power ON to erase all memory data (NOT Calibration Data)
// NOTE: SHOULD BE USED FOR THE FIRST TIME AFTER CALIBRATION !!!
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void resetEeprom() {

  // For write Servo Direction and Dual Rates position
  unsigned int eepromPos;

  // For read/write Sub Trim values position
  unsigned int posEeprom;

  // Writing default model[0]
  EEPROM.write(511, 0);

  // Start writing default values for every model memory bank
  for (int j = 0; j < MODELS; j++) {

    // Define start position for Eeprom storing (32 * [0,1,2,3,4])
    eepromPos = numBytesPerModel * j;

    // Writing Servo Direction default value in first address of start position
    EEPROM.write(eepromPos, 0x00);

    // Writing Sub Trim center stick values for four channels in every model memory bank
    for (int i = 0; i < CHANNELS - 2; i++) {

      // Writing Sub Trim stick values for every channels
      posEeprom = (eepromPos + 10) + (i * 4);
      EEPROMWriteInt(posEeprom, 512);
    }

    eepromPos++; // +1 counting increment

    // Writing Dual Rates values for every channels in every model memory bank
    // Writing values will start after first address of start position
    for (int i = 0; i < CHANNELS - 2; i++) {

      // Writing default LOW rates for channels
      EEPROM.write(eepromPos, 75);

      eepromPos++; // +1 counting increment

      // Writing default HIGH rates for channels
      EEPROM.write(eepromPos, 100);

      eepromPos++; // +1 counting increment
    }
  }
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Macro for read selected model data from Eeprom
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
unsigned char storedDataEeprom(unsigned char mod) {

  // For Eeprom position reference
  unsigned int eepromBase;

  // Read which model to upload data
  if (mod == 255) {
    mod = EEPROM.read(511);
  }

  // Define start position for Eeprom read (32 * [0,1,2,3,4])
  eepromBase = numBytesPerModel * mod;

  unsigned int eepromPos = eepromBase;

  // Read Sub Trim center stick values for selected model
  for (int i = 0; i < CHANNELS - 2; i++) {

    subTrim[i] = EEPROMReadInt((eepromPos + 10) + (i * 4));
  }

  // Read Servo Direction from first position
  servoReverse = EEPROM.read(eepromPos);

  eepromPos++; // +1 counting increment

  // Read Dual Rates values after first position
  for (int i = 0; i < CHANNELS - 2; i++) {

    // Read LOW rates for channels
    dual_rate_low[i] = EEPROM.read(eepromPos);

    eepromPos++; // +1 counting increment

    // Read HIGH rates for channels
    dual_rate_hi[i] = EEPROM.read(eepromPos);

    eepromPos++; // +1 counting increment
  }

  unsigned int posEeprom;

  // Read Min Max calibration values from Eeprom
  for (int i = 0; i < CHANNELS; i++) {

    // Read Min calibration values for channels
    posEeprom = 400 + (i * 4);
    calibration[i][0] = EEPROMReadInt(posEeprom);

    // Read Max calibration values for channels
    posEeprom += 2;
    calibration[i][1] = EEPROMReadInt(posEeprom);
  }
  return mod;
}

// ++++++++++++++++++++++++++++++++++++++++++++++
// Write, Read and Update Eeprom data format
// ++++++++++++++++++++++++++++++++++++++++++++++
void EEPROMWriteInt(int p_address, int p_value) {

  // Write a 16bit value in Eeprom
  byte Byte1 = ((p_value >> 0) & 0xFF);
  byte Byte2 = ((p_value >> 8) & 0xFF);

  EEPROM.write(p_address, Byte1);
  EEPROM.write(p_address + 1, Byte2);
}

int EEPROMReadInt(int p_address) {

  // Read a 16 bit value in Eeprom
  byte Byte1 = EEPROM.read(p_address);
  byte Byte2 = EEPROM.read(p_address + 1);

  long firstTwoBytes = ((Byte1 << 0) & 0xFF) + ((Byte2 << 8) & 0xFF00);
  return (firstTwoBytes);
}

// will update only changed value to preserve eeprom writing cycles
void EEPROMUpdateInt(int p_address, int p_value) {

  // Write a 16bit value in Eeprom
  byte Byte1 = ((p_value >> 0) & 0xFF);
  byte Byte2 = ((p_value >> 8) & 0xFF);

  EEPROM.update(p_address, Byte1);
  EEPROM.update(p_address + 1, Byte2);
}


