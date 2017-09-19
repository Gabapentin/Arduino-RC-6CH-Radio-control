//+++++++++++++++++++++++++++++++++++++++++++++++++++++
// timer1 setup
//+++++++++++++++++++++++++++++++++++++++++++++++++++++
void configTimer1() {

  cli();
  TCCR1A = 0;
  TCCR1B = 0;

  OCR1A = 100;
  TCCR1B |= (1 << WGM12);
  TCCR1B |= (1 << CS11);     // prescaler 8: 0.5us - 16mhz

  TIMSK1 |= (1 << OCIE1A);
  sei();
}
//+++++++++++++++++++++++++++++++++++++++
// Timer interrupt settings
//+++++++++++++++++++++++++++++++++++++++
ISR(TIMER1_COMPA_vect) {

  static boolean state = true;

  TCNT1 = 0;

  if (state) {
    if (onState == 0) {
      PORTD &= ~_BV(sigPin); // set bit 0
    }
    else {
      PORTD |= _BV(sigPin);  // set bit 1
    }
    OCR1A = ppmPulse * 2;
    state = false;
  }
  else {
    static byte cur_chan_numb;
    static unsigned int calc_rest;

    PORTD ^= _BV(sigPin);    // toggle bit

    state = true;

    if (cur_chan_numb >= CHANNELS) {
      cur_chan_numb = 0;
      calc_rest = calc_rest + ppmPulse;
      OCR1A = (ppmLong - calc_rest) * 2;
      calc_rest = 0;
    }
    else {
      OCR1A = (ppm[cur_chan_numb] - ppmPulse) * 2;
      calc_rest = calc_rest + ppm[cur_chan_numb];
      cur_chan_numb++;
    }
  }
}
