// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Macro to check switches and throttle status
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void WarnSwitch() {

  // Set memory buffer for text strings
  char msg_buffer[21];
  char chName_buffer[6];

  // One beep sound warning if something is not in default position
  if (warnSwitch == 1) {
    PORTD |= _BV(buzzer);
    delay(500);
    PORTD &= ~_BV(buzzer);
    warnSwitch = 0;
  }

  u8g2.firstPage();
  do {

    // Changing font type
    u8g2.setFont(u8g2_font_9x15B_tr);

    // Print warning message
    strcpy_P(msg_buffer, (char*)pgm_read_word(&(messages[6])));
    u8g2.setCursor(20, 15);
    u8g2.print(msg_buffer);

    // Changing font type
    u8g2.setFont(u8g2_font_6x10_tr);

    // Print warning message instructions 1
    strcpy_P(msg_buffer, (char*)pgm_read_word(&(messages[7])));
    u8g2.setCursor(5, 30);
    u8g2.print(msg_buffer);

    // Print warning message instructions 2
    strcpy_P(msg_buffer, (char*)pgm_read_word(&(messages[8])));
    u8g2.setCursor(5, 45);
    u8g2.print(msg_buffer);

    // Drawing line separation
    u8g2.drawHLine(0, 50, 128);

    // Changing font type
    u8g2.setFont(u8g2_font_6x13_tr);

    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // Print switches and throttle stick text name when not in default position
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    // Checking TC switch and print text string if active
    if (bitRead(sw_status, 0) == 0) {
      strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[9])));
      u8g2.setCursor(25, 64);
      u8g2.print(chName_buffer);
    }

    // Checking DR switch and print text string if active
    if (bitRead(sw_status, 1) == 0) {
      strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[10])));
      u8g2.setCursor(92, 64);
      u8g2.print(chName_buffer);
    }

    // Checking SWA switch and print text string if active
    if (bitRead(sw_status, 2) == 0) {
      strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[6])));
      u8g2.setCursor(0, 64);
      u8g2.print(chName_buffer);
    }

    // Checking SWB switch and print text string if active
    if (bitRead(sw_status, 3) == 0) {
      strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[7])));
      u8g2.setCursor(110, 64);
      u8g2.print(chName_buffer);
    }

    // Checking THR stick and print text string if active
    if (bitRead(sw_status, 4) == 0) {
      strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[2])));
      u8g2.setCursor(43, 64);
      u8g2.print(chName_buffer);
    }

    // Checking swMix switch and print text string if active
    if (bitRead(sw_status, 5) == 0) {
      strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[17])));
      u8g2.setCursor(67, 64);
      u8g2.print(chName_buffer);
    }
  } while (u8g2.nextPage());

  // Read sw_status switches value if status have been changed
  readSwitches();

  // restore default font characters
  u8g2.setFont(u8g2_font_6x10_tr);
}
