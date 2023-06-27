//The communication with the BOX is through the Bluetooth device 
//it uses the UART protocol of the Serial3

#ifndef SENDTOBOX_H
#define SENDTOBOX_H
#include <Arduino.h>

class Box{
  private:
    String responseAT="";
  public:
    Box();
    motor1();
    motor2();
    motor3();
    x1();
    x2();
    y1();
    y2();        
    z1();
    z2();
    step(int s);
    reboot();
    enter();
    exit();
};

#endif