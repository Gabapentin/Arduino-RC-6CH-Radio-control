# Arduino-RC-6CH-Radio-control

----------------------------------------------------------------------------------------------------------

![alt text](https://github.com/Gabapentin/Arduino-RC-6CH-Radio-control/blob/master/Docs/Images/Screen_0.png)
![alt text](https://github.com/Gabapentin/Arduino-RC-6CH-Radio-control/blob/master/Docs/Images/Menu.png)
![alt text](https://github.com/Gabapentin/Arduino-RC-6CH-Radio-control/blob/master/Docs/Images/Servo_Direction.png)
![alt text](https://github.com/Gabapentin/Arduino-RC-6CH-Radio-control/blob/master/Docs/Images/Dual_Rates.png)


![alt text](https://github.com/Gabapentin/Arduino-RC-6CH-Radio-control/blob/master/Docs/Images/Model.png)
![alt text](https://github.com/Gabapentin/Arduino-RC-6CH-Radio-control/blob/master/Docs/Images/Sub_Trim.png)
![alt text](https://github.com/Gabapentin/Arduino-RC-6CH-Radio-control/blob/master/Docs/Images/Switch_Warning.png)
![alt text](https://github.com/Gabapentin/Arduino-RC-6CH-Radio-control/blob/master/Docs/Images/Calibration.png)



| FS-CT6B |       | HK-T6A |
| ------- | :---: | ------ |
| ![alt text](https://github.com/Gabapentin/Arduino-RC-6CH-Radio-control/blob/master/Docs/Images/FS_CT6B_160.png) | ![alt text](https://github.com/Gabapentin/Arduino-RC-6CH-Radio-control/blob/master/Docs/Images/RX6CH240.png) | ![alt text](https://github.com/Gabapentin/Arduino-RC-6CH-Radio-control/blob/master/Docs/Images/HK_T6A_160.png) |    


Arduino project to upgrade cheap Hobbyking HK-T6A or Flysky FS-CT6B transmitters or any other old RC TX    

This code is based on ardutx v1.3 sketch (2014) from Patricio Reinoso www.patolin.com    

**You can see discussion on RCGroups Forum site**
https://www.rcgroups.com/forums/showthread.php?1817699-Hobbyking-6ch-tx-arduino-computer-update


**If you want discuss this project I have already open thread on RCGroups**    
https://www.rcgroups.com/forums/showthread.php?2959179-Arduino-radio-for-HK-T6A-or-Flysky-FS-CT6B-(ArduTX-rebirth) 

----------------------------------------------------------------------------------------------------------

A great Patolin idea to upgrade cheap HK-T6A or FS-CT6B transmitters or any other old RC TX

These transmitters offer a configuration through a serial port with an external PC software program

and no space to store more than one model.

----------------------------------------------------------------------------------------------------------

The code has been modified to work with SH1106 SPI 1.3" or SSD1306 SPI 0.96" OLED LCD through U8g2 library

Have been used all possible pins of ATmega328 (Arduino UNO, Nano, Pro Mini)

Every text strings has been stored in flash memory to mantain a lot of free RAM memory

----------------------------------------------------------------------------------------------------------

Added features to original Patolin code:

- 3 digital buttons input for menu (Enter/Select) (Down/Next) (Up/Previous) [previously analogs]

- Switches checking status with sound alert at power ON and display which is not in default position 

- Throttle warning with display status of switches and sound alert

- Hidden data default restoring by pressing and hold Down/Next button on power on

- Hidden calibration procedure by pressing and hold Menu/Select button on power on

- Low battery control in real time whith buzzer sound(buzzer with stand alone sound generator)

- N°2 supplementary switch, they can be configured instead of VRA or VRB pots (SWA, SWB)(Servo Direction Menu)

- Basic two channel mixer with switch activation V-Mix (Ch1/ch2)

- Basic four channels Sub Trim setting (ch1, ch2, ch3, ch4)

*************************************************************************************************************

Recommended Arduino **[Arduino IDE 1.8.2](https://www.arduino.cc/en/main/software)**    
(Seems to be optimized and stable compiler. Less Flash and RAM used)

Arduino AVR Boards 1.6.18

#include <U8g2lib.h>               Using U8G2 library **[UG2 Github link](https://github.com/olikraus/U8g2_Arduino)**

#include <SPI.h>                   Arduino standard library

#include <EEPROM.h>                Arduino standard library    

### **[I'm working to make a Wiki pages](https://github.com/Gabapentin/Arduino-RC-6CH-Radio-control/wiki)**
