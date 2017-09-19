void Calibration() {

  // Set memory buffer for text strings
  char msg_buffer[21];
  char chName_buffer[6];

  // Setting default mid value reference for calibration
  for (int i = 0; i < CHANNELS; i++) {
    
    calibration[i][0] = 512;
    calibration[i][1] = 512;   
  }
  while (statusCalib == 1) {

    // Reading Min and Max value for every channel
    unsigned int temporaryReading;

    for (int i = 0; i < CHANNELS; i++) {

      // Get value from every ADC ports
      temporaryReading = read_adc(i);

      // Get Min values
      if (temporaryReading <= calibration[i][0]) {
        calibration[i][0] = temporaryReading;
      }
      // Get Max values
      if (temporaryReading >= calibration[i][1]) {
        calibration[i][1] = temporaryReading;
      }
    }
    // Save Min, Max, Center values in Eeprom
    int posEeprom = 0;

    // Save Min Max value for all 6 channels
    for (int i = 0; i < CHANNELS; i++) {

      // Save Min value
      posEeprom = 400 + (i * 4);
      EEPROMWriteInt(posEeprom, calibration[i][0]);

      // Save Max value
      posEeprom += 2;
      EEPROMWriteInt(posEeprom, calibration[i][1]);
    }
    //delay(10);

  // Start of print real time channels
  u8g2.firstPage();
  do {

    // Print header text
    strcpy_P(msg_buffer, (char*)pgm_read_word(&(messages[3])));
    u8g2.setCursor(10, 7);
    u8g2.print(msg_buffer);

    // Print footer text
    strcpy_P(msg_buffer, (char*)pgm_read_word(&(messages[4])));
    u8g2.setCursor(5, 64);
    u8g2.print(msg_buffer);

    // Drawing horizontal line under header and vertical/middle line
    u8g2.drawHLine(0, 10, 128);
    u8g2.drawVLine(64, 16, 35);

    // Print channel 1 name and value
    strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[0])));
    u8g2.setCursor(0, 23);
    u8g2.print(chName_buffer);
    u8g2.setCursor(30, 23);
    u8g2.print(read_adc(0));

    // Print channel 2 name and value
    strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[1])));
    u8g2.setCursor(75, 23);
    u8g2.print(chName_buffer);
    u8g2.setCursor(105, 23);
    u8g2.print(read_adc(1));

    // Print channel 3 name and value
    strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[2])));
    u8g2.setCursor(0, 37);
    u8g2.print(chName_buffer);
    u8g2.setCursor(30, 37);
    u8g2.print(read_adc(2));

    // Print channel 4 name and value
    strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[3])));
    u8g2.setCursor(75, 37);
    u8g2.print(chName_buffer);
    u8g2.setCursor(105, 37);
    u8g2.print(read_adc(3));

    // Print channel 5 name and value
    strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[11])));
    u8g2.setCursor(0, 50);
    u8g2.print(chName_buffer);
    u8g2.setCursor(30, 50);
    u8g2.print(read_adc(4));

    // Print channel 6 name and value
    strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[12])));
    u8g2.setCursor(75, 50);
    u8g2.print(chName_buffer);
    u8g2.setCursor(105, 50);
    u8g2.print(read_adc(5));

    delay(5);

    // Set statusCalib = 0 to exit calibration procedure by pressing ButtonUp
    if (readKeys() == 1) {
      statusCalib = 0;
    }
  } while (u8g2.nextPage());

  // End of drawing real time channels values screen for calibration
  }
  
// Start print calibration storing message
u8g2.firstPage();
do {

  // Changing font type
  u8g2.setFont(u8g2_font_9x15B_tr);

  // Print storing data calibration message
  strcpy_P(msg_buffer, (char*)pgm_read_word(&(messages[0])));
  u8g2.setCursor(25, 15);
  u8g2.print(msg_buffer);

  strcpy_P(msg_buffer, (char*)pgm_read_word(&(messages[2])));
  u8g2.setCursor(15, 50);
  u8g2.print(msg_buffer);

} while (u8g2.nextPage());

// End of print calibration storing message

delay(2000); // Screen message for 2sec

// Set default font characher
u8g2.setFont(u8g2_font_6x10_tr);
}

