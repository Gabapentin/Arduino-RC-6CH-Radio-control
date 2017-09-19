// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Percentage calculation for Dual Rates (Endpoints)
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int setDR(int value, float dr, unsigned char chan, unsigned char chanTh){

  // Dual Rate calculation
  int output, temp;
  float mult;
  mult = dr;

  if (chan == chanTh) {
    output = (int)value * mult / 100;
  }
  else {
    temp = value - 512;
    output = (int)((temp * mult / 100) + 512);
  }
  output = constrain(output, 0, 1023);

  return output;
}
