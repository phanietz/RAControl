#ifndef SENDTOBOX_H
#define SENDTOBOX_H
#include <Arduino.h>

class Box{
  private:
    String responseAT="";
    String SystemsON[5][3]={
      {"14:3:50974","-OFF","-201"}, //REAL
      {"14:3:508D1","-OFF","-202"}, //REAL
      {"2017:7:251406","-OFF","-203"}, 
      {"14:7:518D1","-OFF","-204"}, 
      {"2016:7:251806","-OFF","-205"} //REAL
      };

  public:
    Box();

};

#endif