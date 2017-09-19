//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Macro to calculate average ADC value, checking battery voltage alarm and value for battery bar
// For minBat do not under default value in config file because arduino will be more unstable.
// About 16%(presumed)of battery is enough to landing ASAP   
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void batteryCheck() {

  // Subtract the last reading:
  total = total - readings[index];

  // Read from adc input:
  readings[index] = read_adc(7);

  // Add reading to total:
  total = total + readings[index];

  // Advance to the next position in the array:
  index = index + 1;

  // If we're at the end of the array...
  if (index >= numReadings)

    // ...wrap around to the beginning:
    index = 0;

  // Calculate the average:
  average = total / numReadings;

  // Mapping battery voltage from average from 0 to 5V
  pin_batt_read = map(average, 0, 1023, 0, 500);

  // Calculating correct voltage value through voltage divider 
  // Result = 2 digits with 2 decimal (eg 72 = 7.2V)
  batt_volt = pin_batt_read * res_calc / 10;
  
 // Serial.println (batt_volt);

  // Mapping voltage for drawing battery bar
  perc_batt = map(batt_volt, minBat, maxBat, 1, 20);
  perc_batt = constrain(perc_batt, 1, 20);


 //Serial.println (perc_batt);
  
  // Low battery beep alarm
  if (perc_batt <= 1 || battStatus == 0) {

    // Set byte 0 to mantain battery alarm even voltage floating +/- on minBat value
    battStatus = 0;

    // Set value to mantain battery bar to min value even voltage is under minBat
    perc_batt = 1;
    
    // Recall pulse for buzzer
    beep();
  }
  
  // Buzzer alarm OFF
  else {
    PORTD &= ~_BV(buzzer);
    }
}
