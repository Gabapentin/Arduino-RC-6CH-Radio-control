// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Drawing Main Screen display
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void Screen_0() {

  // Set memory buffer for text strings
  char chName_buffer[6];
  char char_buffer[2];

  // Start of main screen display informations "0" ************************
  u8g2.firstPage();
  do {

    readPots(); // Recall macro again for stable ppm pulse

    // Print "MOD" text string
    strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[8])));
    u8g2.setCursor(0, 7);
    u8g2.print(chName_buffer);

    // Print number of which model in use
    u8g2.setCursor(23, 7);
    u8g2.print(modelActual + 1);

    // Throttle Cut switch status checking
    if (bitRead(sw_status, 0) == 0) {

      //Print Throttle Cut status only if active (TC)
      strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[9])));
      u8g2.setCursor(37, 7);
      u8g2.print(chName_buffer);
    }
    // V-Mix switch status checking
    if (bitRead(sw_status, 5) == 0) {

      //Print V-Mix status only if active (MIX)
      strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[17])));
      u8g2.setCursor(57, 7);
      u8g2.print(chName_buffer);
    }
    // Dual Rates switch status checking
    if (bitRead(sw_status, 1) == 0) {

      // Print Dual Rates status only if active (DR)
      strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[10])));
      u8g2.setCursor(82, 7);
      u8g2.print(chName_buffer);
    }
    // Battery simbol contruction
    u8g2.drawFrame(102, 0, 24, 8);             // Battery box
    u8g2.drawBox(104, 2, perc_batt, 4);        // level bar
    u8g2.drawBox(126, 2, 2, 3);                // Battery nipple plus pole

    // Print first 4 channels reference with input pots order
    // A0(A), A1(E), A2(T), A3(R)

    // Print "A" character
    strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[0])));
    u8g2.setCursor(0, 20);
    u8g2.print(char_buffer);

    // Print "E" character
    strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[1])));
    u8g2.setCursor(0, 30);
    u8g2.print(char_buffer);

    // Print "T" character
    strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[2])));
    u8g2.setCursor(0, 40);
    u8g2.print(char_buffer);

    // Print "R" character
    strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[3])));
    u8g2.setCursor(0, 50);
    u8g2.print(char_buffer);

    //Drawing vertical middle/center separation line
    u8g2.drawVLine(65, 14, 64);

    // Drawing only first 4 channels
    for (int i = 0; i <= 3; i++) {

      // Define value bar reference
      unsigned int valBar;

      // Value bars subdivision (ppm value / 100)
      valBar = map(ppm[i], ppmMin, ppmMax, 0 , 100);

      // Draw boxes/frames for every channel
      u8g2.drawFrame(13, 13 + (i * 10), 105, 8);

      // Drawing cursor in every channel bar
      u8g2.drawBox(13 + valBar, 14 + (i * 10), 5, 6);

      if (bitRead(servoReverse, i) == 1) {

        // Print "R"everse channel status for every channel
        strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[3])));
        u8g2.setCursor(123, 20 + i * 11);
        u8g2.print(char_buffer);
      }
      else {

        // Print "N"ormal channel status for every channel
        strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[4])));
        u8g2.setCursor(123, 20 + i * 11);
        u8g2.print(char_buffer);
      }
    }
    //If switch SWA print ON/OFF state
    if (bitRead(servoReverse, 4) == 1) {

      // Print "SWA" text
      strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[6])));
      u8g2.setCursor(13, 64);
      u8g2.print(chName_buffer);

      if (read_adc(6) < 500) {

        // Print SWA "ON" text
        strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[15])));
        u8g2.setCursor(40, 64);
        u8g2.print(chName_buffer);
      }
      else {

        // Print SWA "OFF" text
        strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[16])));
        u8g2.setCursor(40, 64);
        u8g2.print(chName_buffer);
      }
    }
    else {

      //If pot VRA print value in %
      unsigned int VRA;
      VRA = map(pots[4], 0, potMax, 0, 100);

      // Print "VRA" text
      strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[11])));
      u8g2.setCursor(13, 64);
      u8g2.print(chName_buffer);

      // Print VRA value
      u8g2.setCursor(40, 64);
      u8g2.print(VRA);
    }

    //If switch SWB print ON/OFF state
    if (bitRead(servoReverse, 5) == 1) {

      // Print "SWB" text
      strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[7])));
      u8g2.setCursor(73, 64);
      u8g2.print(chName_buffer);

      if (digitalRead(swB) == 0) {

        // Print SWB "ON" text
        strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[15])));
        u8g2.setCursor(98, 64);
        u8g2.print(chName_buffer);
      }
      else {

        // Print SWB "OFF" text
        strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[16])));
        u8g2.setCursor(98, 64);
        u8g2.print(chName_buffer);
      }
    }
    else {

      //If pot VRB print value in %
      unsigned int VRB;
      VRB = map(pots[5], 0, potMax, 0, 100);

      // Print "VRB" text
      strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[12])));
      u8g2.setCursor(75, 64);
      u8g2.print(chName_buffer);

      // Print VRB value
      u8g2.setCursor(100, 64);
      u8g2.print(VRB);
    }

  } while (u8g2.nextPage());

  // End of main screen display informations "0" **************************
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Drawing Menu Selection screen display
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void Screen_1 () {

  // Set memory buffer for text strings
  char menu_buffer[18];
  char char_buffer[2];

  // Start of menu selection screen
  u8g2.firstPage();
  do {

    readPots(); // Recall macro for stable ppm pulse

    for (int i = 0; i < CHANNELS; i++) {

      // Print main menu items
      strcpy_P(menu_buffer, (char*)pgm_read_word(&(menu_name[i - 1])));

      if (i == menuSubActual) {

        // Print selection cursor character "*"
        strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[13])));
        u8g2.setCursor(0, i * 12);
        u8g2.print(char_buffer);

        // Print main menu items
        u8g2.setCursor(15, i * 12);
        u8g2.print(menu_buffer);
      }
      else {

        // Print main menu items
        u8g2.setCursor(25, i * 12);
        u8g2.print(menu_buffer);
      }
    }
  } while (u8g2.nextPage());
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Drawing Servo Direction screen display
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void Menu_1 () {

  // Set memory buffer for text strings
  char chName_buffer[6];
  char char_buffer[2];

  // Start of Servo direction setting screen "2" "1" **********************
  u8g2.firstPage();
  do {

    readPots(); // Recall macro for stable ppm pulse

    // Print Servo Direction channels screen list
    for (int i = 0; i < CHANNELS; i++) {

      // Print channel name list
      strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[i])));

      if (i == menuSubActual) {

        // Print selection cursor "["
        strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[8])));
        u8g2.setCursor(0, 7 + i * 11);
        u8g2.print(char_buffer);

        // Print character "=");
        strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[10])));
        u8g2.setCursor(28, 7 + i * 11);
        u8g2.print(char_buffer);

        // Print selection cursor character "]" for selected item
        strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[9])));
        u8g2.setCursor(57, 7 + i * 11);
        u8g2.print(char_buffer);

      }

      // Print channel items name
      u8g2.setCursor(8, 7 + i * 11);
      u8g2.print(chName_buffer);

      if (bitRead(servoReverse, i) == 1) {

        // Print(REV)erse status
        strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[14])));
        u8g2.setCursor(37, 7 + i * 11);
        u8g2.print(chName_buffer);
      }
      else {

        // Print(NOR)mal status
        strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[13])));
        u8g2.setCursor(37, 7 + i * 11);
        u8g2.print(chName_buffer);
      }
      if (bitRead(servoReverse, 4) == 1) {
        // Print SWA if REVerse status
        strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[6])));
        u8g2.setCursor(37, 51);
        u8g2.print(chName_buffer);
      }
      else {

        // Print VRA if NORmal status
        strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[11])));
        u8g2.setCursor(37, 51);
        u8g2.print(chName_buffer);
      }
      if (bitRead(servoReverse, 5) == 1) {
        // Print SWB if REVerse status
        strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[7])));
        u8g2.setCursor(37, 62);
        u8g2.print(chName_buffer);
      }
      else {

        // Print VRB if NORmal status
        strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[12])));
        u8g2.setCursor(37, 62);
        u8g2.print(chName_buffer);
      }
    }

    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // Drawing dynamic graphics items
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    // Drawing status of CH5 if VRA(POT) or SWA(SWITCH)
    if (bitRead(servoReverse, 4) == 1) {
      u8g2.drawRFrame(74, 43, 11, 20, 3);               // Draw SWA box
      u8g2.drawDisc(79, 48, 3, U8G2_DRAW_ALL);          // Draw SWA round point
    }
    else {
      u8g2.drawCircle(78, 51, 9, U8G2_DRAW_ALL);        // Draw VRA
      u8g2.drawBox(66, 50, 9, 3);                       // Draw VRA cursor box
      u8g2.drawDisc(78, 51, 5, U8G2_DRAW_ALL);          // Draw SWB round point
    }

    // Drawing status of CH6 if VRB(POT) or SWB(SWITCH)
    if (bitRead(servoReverse, 5) == 1) {
      u8g2.drawRFrame(107, 43, 11, 20, 3);              // Draw SWB box
      u8g2.drawDisc(112, 48, 3, U8G2_DRAW_ALL);         // Draw SWB round point
    }
    else {
      u8g2.drawCircle(112, 51, 9, U8G2_DRAW_ALL);       // Draw VRB
      u8g2.drawBox(100, 50, 9, 3);                      // Draw VRB cursor box
      u8g2.drawDisc(112, 51, 5, U8G2_DRAW_ALL);         // Draw VRB round point
    }

    u8g2.drawFrame(66, 11, 26, 26);                     // Left Stick squared box

    // Drawing left stick position CH4/CH3
    u8g2.drawDisc(map(ppm[3], 1000 , 2000, 70, 88), map(ppm[2], 1000 , 2000, 32, 15), 2);

    u8g2.drawFrame(100, 11, 26, 26);                    // Right Stick squared box

    // Drawing left stick position CH1/CH2
    u8g2.drawDisc(map(ppm[0], 1000 , 2000, 104, 121), map(ppm[1], 1000 , 2000, 32, 15), 2);

    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    // Print "*" text character
    strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[13])));
    u8g2.setCursor(71, 7);
    u8g2.print(char_buffer);

    // Print "MODEL" text string
    strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[18])));
    u8g2.setCursor(81, 7);
    u8g2.print(chName_buffer);

    // Print number of model
    u8g2.setCursor(116, 7);
    u8g2.print(modelActual + 1);

  } while (u8g2.nextPage());
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Drawing Dual Rates screen display
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void Menu_2 () {

  // Set memory buffer for text strings
  char chName_buffer[6];
  char char_buffer[2];
  char menu_buffer[18];

  // Start of Dual Rates setting screen ***********************************
  u8g2.firstPage();
  do {

    readPots(); // Recall macro for stable ppm pulse

    unsigned char counterTemp = 0;

    // Print Dual Rates channels list
    for (int i = 0; i < CHANNELS - 2; i++) {

      // Print channel name list
      strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[i])));
      u8g2.setCursor(0, 25 + i * 13);
      u8g2.print(chName_buffer);

      if (menuSubActual == counterTemp) {
        if (drSelection == counterTemp) {

          // Print "[" character for item selected for LOW value
          strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[8])));
          u8g2.setCursor(24, 25 + i * 13);
          u8g2.print(char_buffer);

          // Print "]" character for item selected for LOW value
          strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[9])));
          u8g2.setCursor(68, 25 + i * 13);
          u8g2.print(char_buffer);
        }
        else {

          // Print selection cursor character ">" for LOW value
          strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[14])));
          u8g2.setCursor(24, 25 + i * 13);
          u8g2.print(char_buffer);
        }
      }

      // Print "L" character for LOW value
      strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[6])));
      u8g2.setCursor(31, 25 + i * 13);
      u8g2.print(char_buffer);

      // Print character "=");
      strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[10])));
      u8g2.setCursor(40, 25 + i * 13);
      u8g2.print(char_buffer);

      // Print LOW value
      u8g2.setCursor(49, 25 + i * 13);
      u8g2.print(dual_rate_low[i]);

      counterTemp++;

      if (menuSubActual == counterTemp) {
        if (drSelection == counterTemp) {

          // Print "[" character for item selected for HIGH value
          strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[8])));
          u8g2.setCursor(78, 25 + i * 13);
          u8g2.print(char_buffer);

          // Print "]" character for item selected for HIGH value
          strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[9])));
          u8g2.setCursor(123, 25 + i * 13);
          u8g2.print(char_buffer);
        }
        else {

          // Print selection cursor character ">" for HIGH value
          strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[14])));
          u8g2.setCursor(78, 25 + i * 13);
          u8g2.print(char_buffer);
        }
      }

      // Print "H" text character for HIGH value
      strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[5])));
      u8g2.setCursor(85, 25 + i * 13);
      u8g2.print(char_buffer);

      // Print character "=");
      strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[10])));
      u8g2.setCursor(95, 25 + i * 13);
      u8g2.print(char_buffer);

      // Print HIGH value
      u8g2.setCursor(104, 25 + i * 13);
      u8g2.print(dual_rate_hi[i]);

      counterTemp++;
    }

    // Print "*" text character
    strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[13])));
    u8g2.setCursor(0, 7);
    u8g2.print(char_buffer);

    // Print "MODEL" text string
    strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[18])));
    u8g2.setCursor(10, 7);
    u8g2.print(chName_buffer);

    // Print number of actual model
    u8g2.setCursor(45, 7);
    u8g2.print(modelActual + 1);

    // Print "DUAL RATES" text string
    strcpy_P(menu_buffer, (char*)pgm_read_word(&(menu_name[1])));
    u8g2.setCursor(68, 7);
    u8g2.print(menu_buffer);

  } while (u8g2.nextPage());
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Drawing Model Selection screen display
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void Menu_3 () {

  // Set memory buffer for text strings
  char char_buffer[2];
  char chName_buffer[6];

  // Start of Model Selection screen ***********************************

  u8g2.firstPage();
  do {

    readPots(); // Recall macro for stable ppm pulse

    // Print Model Selection list
    for (int i = 0; i <= MODELS - 1; i++) {

      u8g2.setCursor(15, 7 + i * 14);
      u8g2.print(i + 1);

      if (i == menuSubModel) {

        // Print model selection cursor

        strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[8])));
        u8g2.setCursor(0, 7 + i * 14);
        u8g2.print(char_buffer);

        strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[9])));
        u8g2.setCursor(30, 7 + i * 14);
        u8g2.print(char_buffer);
      }
    }

    // Print "MODEL" text string
    strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[18])));
    u8g2.setCursor(62, 7);
    u8g2.print(chName_buffer);

    u8g2.setCursor(103, 7);
    u8g2.print(modelActual + 1);

    // Drawing vertical line before airplane XBM image
    u8g2.drawBox(45, 0, 3, 64);

    // Print airplane XBM image
    u8g2.drawXBMP(58, 16, airplane_width, airplane_height, airplane);

    // Drawing vertical line after airplane XBM image
    u8g2.drawBox(124, 0, 3, 64);

  } while (u8g2.nextPage());
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Drawing Save Model Data screen display
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void Menu_4 () {

  // For Eeprom position reference
  unsigned int eepromBase;

  // For write/Update Sub Trim stored values
  unsigned int posEeprom;

  // Define start position for Eeprom write/update (32 * [0,1,2,3,4])
  eepromBase = numBytesPerModel * modelActual;

  // For write/Update Servo Direction and Dual Rates position
  unsigned int eepromPos = eepromBase;

  // Save Actual Model data
  EEPROM.update(511, modelActual);

  // Save Servo Direction data
  EEPROM.update(eepromPos, servoReverse);

  // Save Sub Trim center stick values for four channels in every model memory bank
  for (int i = 0; i < CHANNELS - 2; i++) {

    // Save center stick values for every channels
    posEeprom = (eepromPos + 10) + (i * 4);
    EEPROMWriteInt(posEeprom, subTrim[i]);
  }

  eepromPos++; // +1 counting increment

  // Save Dual Rates data
  for (int i = 0; i < CHANNELS - 2; i++) {

    EEPROM.update(eepromPos, dual_rate_low[i]);
    eepromPos++;

    EEPROM.update(eepromPos, dual_rate_hi[i]);
    eepromPos++;
  }

  // Start of Save Data message screen ************************
  u8g2.firstPage();
  do {

    // Changing fot type
    u8g2.setFont(u8g2_font_9x15B_tr);

    // Set memory buffer for text strings
    char msg_buffer[21];

    strcpy_P(msg_buffer, (char*)pgm_read_word(&(messages[0])));
    u8g2.setCursor(25, 15);
    u8g2.print(msg_buffer);

    strcpy_P(msg_buffer, (char*)pgm_read_word(&(messages[1])));
    u8g2.setCursor(35, 45);
    u8g2.print(msg_buffer);

    u8g2.setCursor(90, 45);
    u8g2.print(modelActual + 1);

  } while (u8g2.nextPage());

  // End of Save Data message screen **************************

  delay(2000); // Screen message for 2sec

  // Changing to default font type
  u8g2.setFont(u8g2_font_6x10_tr);

  screen--;
  menuActual = 1;
  menuSubActual = 1;
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Drawing Sub Trim Setup screen display
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void Menu_5 () {

  // Set memory buffer for text strings
  char chName_buffer[6];
  char char_buffer[2];
  char menu_buffer[18];

  // Start Sub Trim Setup screen ***********************************
  u8g2.firstPage();
  do {

    readPots(); // Recall macro for stable ppm pulse

    unsigned char temp_Counter = 0;

    // Print Sub Trim channels list
    for (int i = 0; i < CHANNELS - 2; i++) {

      // Print channel name items
      strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[i])));
      u8g2.setCursor(8, 25 + i * 13);
      u8g2.print(chName_buffer);

      if (menuSubActual == temp_Counter) {
        if (subTrimSelection == temp_Counter) {

          // Print selection cursor character ">" for channel
          strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[14])));
          u8g2.setCursor(0, 25 + i * 13);
          u8g2.print(char_buffer);

          // Print selection cursor character "[" for selected item
          strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[8])));
          u8g2.setCursor(28, 25 + i * 13);
          u8g2.print(char_buffer);

          // Print selection cursor character "]" for selected item
          strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[9])));
          u8g2.setCursor(54, 25 + i * 13);
          u8g2.print(char_buffer);
        }
        else {

          // Print selection cursor character ">" for channel
          strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[14])));
          u8g2.setCursor(0, 25 + i * 13);
          u8g2.print(char_buffer);
        }
      }

      // Print Sub Trim value
      u8g2.setCursor(35, 25 + i * 13);
      u8g2.print(subTrim[i]);

      // Print character "=");
      strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[10])));
      u8g2.setCursor(62, 25 + i * 13);
      u8g2.print(char_buffer);

      // Print "PPM" text string
      strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[19])));
      u8g2.setCursor(71, 25 + i * 13);
      u8g2.print(chName_buffer);

      // Print ppm real time ppm value
      u8g2.setCursor(94, 25 + i * 13);
      u8g2.print(ppm[i]);

      temp_Counter++;
    }

    // Print "*" text character
    strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[13])));
    u8g2.setCursor(0, 7);
    u8g2.print(char_buffer);

    // Print "MODEL" text string
    strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[18])));
    u8g2.setCursor(10, 7);
    u8g2.print(chName_buffer);

    // Print number of actual model
    u8g2.setCursor(45, 7);
    u8g2.print(modelActual + 1);

    // Print "SUB TRIM" text string
    strcpy_P(menu_buffer, (char*)pgm_read_word(&(menu_name[5])));
    u8g2.setCursor(71, 7);
    u8g2.print(menu_buffer);

  } while (u8g2.nextPage());

  // End Sub Trim Setup screen ***********************************
}
