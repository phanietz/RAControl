#ifndef ATCOMMANDS_H
#define ATCOMMANDS_H
#include <Arduino.h>
#define ENABLE 9

class AT{
  private:
    long int cycle=0;
    String param="";
    char* responseATarray="", textChar="";
    String responseAT="", text="";
    //char* SystemsAvailable[3]={"","",""};
    //String SystemsAvailable[9]={};
    String SystemsON[5][3]={
      {"14:3:50974","-OFF","-201"}, //REAL
      {"14:3:508D1","-OFF","-202"}, //REAL
      {"2017:7:251406","-OFF","-203"}, 
      {"14:7:518D1","-OFF","-204"}, 
      {"2016:7:251806","-OFF","-205"} //REAL
      };
    int index, refreshCount;
    char a; 
    //int numChars=100;
    //char received[100];
    //byte ndx=0;
    String endMarker="OK\r\n", auxEndMarker="";
    //char rc;    
    //boolean newData=false;
  public:
    AT();
    void config();
    void refresh();
    void connect(int cell);    
    void disconnect();
    void receive();
    void send(String cell);
    void SendSystemsAvailbles();
};

#endif