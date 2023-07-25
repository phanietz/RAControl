//The communication with the BLUETOOTH is through Serial3
//ERROR MEANING
//error = 0   working good
//error = 1   CONNECTION FAILED, something is going wrong with the communication, Try one more time
//error = 2   NO SYSTEMS DETECTED, verify at least one of system is ON
//error = 3   INCOMPLETE RECEIVED DATA, Try one more time manually, means user can see loss conection and needs try again
#ifndef ATCOMMANDS_H
#define ATCOMMANDS_H
#include <Arduino.h>
#define ENABLE 9
#define SIGNALink 53

class AT{
  private:
    String responseAT="", text="";
    char a; 
    int refreshCount, index, countCach, i;
    String auxEndMarker="", data="";
    long int cycle=0;
    unsigned long findingCopy=0;
  public:
    String SystemsON[5][3]={
      {"14:3:50974","-OFF","-201"}, //REAL
      {"14:3:508D1","-OFF","-202"}, //REAL
      {"2017:7:251406","-OFF","-203"}, 
      {"14:7:518D1","-OFF","-204"}, 
      {"2016:7:251806","-OFF","-205"} //REAL
      };    
    int error=0, system=0;
    float step=0;
    float axisNum[3]={00.00,00.00,00.00};
    String axisStr[3]={"00.00","00.00","00.00"};
  public:
    AT();
    String inqm(String param);
    String reset();
    String init();    
    String inq(bool mode, class HMI &Dis);
    String link(int cell);
    String disc();
    bool axisReceiving();
    String waitData(int type);
    bool waitCopy();
};

#endif