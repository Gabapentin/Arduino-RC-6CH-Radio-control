//+++++++++++++++++++++++++++++++++++++++++++++++++
// Read input buttons status and send value to loop
//+++++++++++++++++++++++++++++++++++++++++++++++++
unsigned char readKeys() {

  // buttonSelect
  if ((PIND & (1 << 1)) == 0) {
    delay(20);
    return 2;
  }
  // buttonUp
  if ((PIND & (1 << 2)) == 0) {
    delay(20);
    return 1;
  }
  // buttonDown
  if ((PIND & (1 << 3)) == 0) {
    delay(20);
    return 3;
  }
  else {
    return 0;
  }
}
