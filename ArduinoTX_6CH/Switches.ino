// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Macro for read switch status definitions (sw_status)
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void readSwitches() {

  if (bit_is_clear(PIND, swTC)) {
    bitWrite(sw_status, 0, 0);
  }
  else {
    bitWrite(sw_status, 0, 1);
  }
  if (bit_is_clear(PIND, swDR)) {
    bitWrite(sw_status, 1, 0);
  }
  else {
    bitWrite(sw_status, 1, 1);
  }
  // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // Added status for supplementary switches SWA, SWB (sw_status)
  // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

  // Read value directly from analog port
  if (read_adc(6) < 1000) {
    bitWrite(sw_status, 2, 0);
  }
  else {
    bitWrite(sw_status, 2, 1);
  }
  // Read bit status on swB digital pin 12
  if (bit_is_clear(PINB, 4)) {
    bitWrite(sw_status, 3, 0);
  }
  else {
    bitWrite(sw_status, 3, 1);
  }
  
  // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // Added status for Throttle position (sw_status)
  // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

  unsigned int thr_check;

  // Checking if Throttle is reversed and remapping ppm value
  if (bitRead(servoReverse, chanTh) == 1) {
    thr_check = map(read_adc(chanTh), calibration[chanTh][0], calibration[chanTh][1], ppmMax, ppmMin);
  }
  else {
    thr_check = map(read_adc(chanTh), calibration[chanTh][0], calibration[chanTh][1], ppmMin, ppmMax);
  }
  // ppm stick value for Throttle alarm
  if (thr_check >= 1050) {
    bitWrite(sw_status, 4, 0);
  }
  else {
    bitWrite(sw_status, 4, 1);
  }
  
  // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // Added status for swMix switch position (sw_status)
  /// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

  // Read bit status on swMix digital pin 0
  if (bit_is_clear(PIND, 0)) {
    bitWrite(sw_status, 5, 0);
  }
  else {
    bitWrite(sw_status, 5, 1);
  }
}
