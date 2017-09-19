//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Macro for directly read ADC value on all analog ports
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
unsigned short int read_adc(unsigned char channel) {

  ADMUX &= 0xF0;                        //Clear the older channel that was read
  ADMUX |= channel;                     //Defines the new ADC channel to be read
  ADCSRA |= (1 << ADSC);                //Starts a new conversion
  while (ADCSRA & (1 << ADSC));         //Wait until the conversion is done
  return ADCW;                          //Returns the ADC value of the chosen channel
}
