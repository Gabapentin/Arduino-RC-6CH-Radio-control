//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Macro for read sticks, pots, switches values and applying calibration and rules
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void readPots() {

  for (int i = 0; i < CHANNELS; i++) {

    raw_Pots[i] = read_adc(i);

    // Applying calibration mapping
    pots[i] = map(raw_Pots[i], calibration[i][0], calibration[i][1], potMin, potMax);

    // Assign pot VRA calibrated value 
    // for NO Dual Rates else not working(???)
    NO_DR_VRA = pots[4];

    // Assign pot VRB calibrated value 
    // for NO Dual Rates else not working(???)
    NO_DR_VRB = pots[5];

    // Check Servo Reversing and applying Reverse value if necessary
    if (bitRead(servoReverse, i) == 1) {
      pots[i] = potMax - pots[i];
    }

    // Checking Throttle Cut switch status, 
    // if TC activated throttle stick not working
    if (i == chanTh && bitRead(sw_status, 0) == 0) {
      pots[i] = 0;
    }

    // Checking Dual Rates switch status, 
    // if DR activated limitation of the preset percentage value will be applied
    if (bitRead(sw_status, 1) == 0) {

      // LOW rates calculation
      pots[i] = setDR(pots[i], dual_rate_low[i], i, chanTh);

      // NO Dual rates for pots
      pots[4] = NO_DR_VRA;
      pots[5] = NO_DR_VRB;
    }
    else {

      // HIGH rates calculation
      pots[i] = setDR(pots[i], dual_rate_hi[i], i, chanTh);

      // NO Dual rates for pots
      pots[4] = NO_DR_VRA;
      pots[5] = NO_DR_VRB;
    }

    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // ppm mapping ch1/ch2 values if V-mix switch is enabled
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    if (bitRead(sw_status, 5) == 0) {

      //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
      // V-Mix variables
      //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
      unsigned int ppm_CH1;
      unsigned int ppm_CH2;
      unsigned int mixVal;

      // ppm mapping CH3/CH4 only with deadband
      for (int i = 2; i < CHANNELS - 2; i++) {

        if (pots[i] < (subTrim[i] - deadBand)) {
          ppm[i] =  map(pots[i], potMin, subTrim[i] - deadBand, ppmMin, servoCenter);
        }
        else if (pots[i] > subTrim[i] + deadBand) {
          ppm[i] =  map(pots[i], subTrim[i] + deadBand, potMax, servoCenter, ppmMax);
        }
        else {
          
          // Auto centering if in deadband
          ppm[i] = servoCenter;
        }

        // Temporary mapping CH1/CH2 with deadband
        if (pots[0] < (subTrim[0] - deadBand)) {
          ppm_CH1 =  map(pots[0], potMin, subTrim[0] - deadBand, ppmMin, servoCenter);
        }
        else if (pots[0] > subTrim[0] + deadBand) {
          ppm_CH1 =  map(pots[0], subTrim[i] + deadBand, potMax, servoCenter, ppmMax);
        }
        else {

          // Auto centering if in deadband
          ppm_CH1 = servoCenter;
        }
        if (pots[1] < (subTrim[1] - deadBand)) {
          ppm_CH2 =  map(pots[1], potMin, subTrim[1] - deadBand, ppmMin, servoCenter);
        }
        else if (pots[1] > subTrim[1] + deadBand) {
          ppm_CH2 =  map(pots[1], subTrim[1] + deadBand, potMax, servoCenter, ppmMax);
        }
        else {

          // Auto centering if in deadband
          ppm_CH2 = servoCenter;
        }

        // Mapping ppm CH1/CH2 values for V-mix
        mixVal = (ppm_CH1 - servoCenter);

        // applying the result on CH1/CH2 ppm output  
        ppm[0] = ppm_CH2 - mixVal;
        ppm[1] = ppm_CH2 + mixVal;
      }
    }
    else {

      // ppm mapping CH1/CH2/CH3/CH4 with deadband
      for (int i = 0; i < CHANNELS - 2; i++) {

        if (pots[i] < (subTrim[i] - deadBand)) {
          ppm[i] =  map(pots[i], potMin, subTrim[i] - deadBand, ppmMin, servoCenter);
        }
        else if (pots[i] > subTrim[i] + deadBand) {
          ppm[i] =  map(pots[i], subTrim[i] + deadBand, potMax, servoCenter, ppmMax);
        }
        else {

          // Auto centering if in deadband
          ppm[i] = servoCenter;
        }
      }

    }

    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // ppm mapping pot VRA to Switch SWA
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    // SWA switch ppm mapping on analog input (CH5)
    if (bitRead(servoReverse, 4) == 1) {
      ppm[4] = map(read_adc(6), potMin, potMax, ppmMax, ppmMin);
    }
    else {
      // VRA pot mapping (CH5)
      ppm[4] = map(read_adc(4), potMin, potMax, ppmMin, ppmMax);
    }

    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // ppm mapping pot VRB to Switch SWB
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    // SWB switch ppm mapping on digital input (CH6)
    if (bitRead(servoReverse, 5) == 1) {
      ppm[5] = map(digitalRead(swB), 1, 0, ppmMin, ppmMax);
    }
    else {
      // VRB pot mapping (CH6)
      ppm[5] = map(read_adc(5), potMin, potMax, ppmMin, ppmMax);
    }
  }
}
