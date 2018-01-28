## **Sketch file for calibrate battery and testing low battery alarm with OLED display**

![alt text](https://github.com/Gabapentin/Arduino-RC-6CH-Radio-control/blob/master/Docs/Images/Battcheckoled.png)

This sketch want provide help for testing and calibrate battery value from Arduino pin    
with low battery alarm functionality through OLED display (without connecting buzzer)    

**NOTES and safety WARNING:**

   - Use linear voltage variable power supply to simulate voltage battery, when it possible.
   
   - If you want a correct visualization of values after uploading sketch in Arduino    
     is necessary remove USB programming cable from PC because it makes readings unstable.  
   - **[Must be used R1-10K/R2-10K voltage divider for 2S lion/LiPo battery](https://github.com/Gabapentin/Arduino-RC-6CH-Radio-control/wiki/4.6-(Battery-Monitor-Wiring))**    
   - **[Must be used R1-20K/R2-10K voltage divider for 3S lion/LiPo battery](https://github.com/Gabapentin/Arduino-RC-6CH-Radio-control/wiki/4.6-(Battery-Monitor-Wiring))**
   - **For 2S voltage divider do not over 10V, For 3S voltage divider do not over 15V.
     Otherwise Arduino pin will be fried.**
     
   - **WARNING: If you want reset ALARM OLED display indication!    
     is necessary to put battery voltage to the maximum and restart Arduino with reset button**  

  ## For testing you can modify these parameters:
   
   **Number of reading to calculate average    
   lower value means fast reaction, high value slow reaction (8 value is maximum)**
   
```javascript
   const int numReadings = 8;
   ```
   ```javascript
   #define minBat 70     // Minimum battery voltage for alarm (70 = 7.0V)for 2S LiOn/LiPo battery
   #define maxBat 82     // Maximum battery voltage (10V) (82 = 8.2V)for 2S LiOn/LiPo battery
  
   ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
   #define minBat 100    // Minimum battery voltage for alarm (100 = 10V)for 3S LiOn/LiPo battery
   #define maxBat 125    // Maximum battery voltage (125 = 12.5V)for 3S LiOn/LiPo battery
   ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++    
   ```

   **If you need more precision, measure voltage divider resistors value with digital multimeter    
   put them in to R1/R2 variables and trim these values**
   ```javascript
   float R1 = 10430;
   float R2 = 10055;
   ```
## **After ending test you need to put result values in config.h file**    


![alt text](https://github.com/Gabapentin/Arduino-RC-6CH-Radio-control/blob/master/Docs/Images/Multimeter_20K.png)
![alt text](https://github.com/Gabapentin/Arduino-RC-6CH-Radio-control/blob/master/Docs/Images/Multimeter_10K.png)
![alt text](https://github.com/Gabapentin/Arduino-RC-6CH-Radio-control/blob/master/Docs/Images/Multimeter_Voltage.png)
