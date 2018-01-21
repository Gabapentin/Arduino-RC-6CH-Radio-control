### **Please remember at this time is only prototype** ###    
This directory contains all files for Arduino Nano one layer PCB shield (60x40mm dimension)   
#### This shield will provide directly battery power to TX module like **[Multiprotocol](https://github.com/pascallanger/DIY-Multiprotocol-TX-Module)** or FRSKY/FLYSKY external module     

### **DO NOT USE this PCB for Hobbyking HK-T6A or Flysky FS-CT6B original Transmitter module**
#### NOTES:
* #### Arduino Nano must be mounted on a female header socket to permit capacitors mounting under Nano module    
* #### Maximum battery voltage is 10V (2S Lion/LiPo 8.4V is enough for long time operation)
* #### For 3S 12.6V battery voltage is necessary to modify R1 to 20K and some parameters on sketch
* #### PPM voltage is reduced from voltage divider to 2.7Vpp    


![alt text](https://github.com/Gabapentin/Arduino-RC-6CH-Radio-control/blob/master/Docs/Images/ARDUINO_RC_Battery_Powered_PCB.png)
