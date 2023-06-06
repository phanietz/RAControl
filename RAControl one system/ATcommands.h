#ifndef ATCOMMANDS_H
#define ATCOMMANDS_H
#include <Arduino.h>
#define ENABLE 9

class AT{
  private:
    String responseAT="", text="", link="";
    char a; 
    int refreshCount, index, countCach;
    String auxEndMarker="";
    long int cycle=0;
  public:
    String SystemsON[5][3]={
      {"14:3:50974","-OFF","-201"}, //REAL
      {"14:3:508D1","-OFF","-202"}, //REAL
      {"2017:7:251406","-OFF","-203"}, 
      {"14:7:518D1","-OFF","-204"}, 
      {"2016:7:251806","-OFF","-205"} //REAL
      };    
  public:
    AT();
    String INQM(String param);
    String RESET();
    String INIT();    
    String INQ(bool mode);
    String LINK(int cell);
    String DISC();
};

#endif