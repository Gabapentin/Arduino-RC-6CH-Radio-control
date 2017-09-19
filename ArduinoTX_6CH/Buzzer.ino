//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Macro for buzzer beep battery alarm
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void beep () {

  if ((bitRead(pulseBuzzer, 0) && bitRead(pulseBuzzer, 5) && bitRead(pulseBuzzer, 1))) {
    PORTD |= _BV(buzzer);
  }
  else {
    PORTD &= ~_BV(buzzer);
  }
  ++pulseBuzzer;
}
