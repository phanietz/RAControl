#ifndef SENDTODISPLAY_H
#define SENDTODISPLAY_H
#include <Arduino.h>

//methods in this class that sends commands to Display:
//+INQ
//find with: Serial2.print
class Display{
  private:
    String responseAT="", param="";
    int i=0;
  public:
    Display();
    void start();
    void refresh();
    void connect(int cell);    
    void disconnect();
    void SendSystemsAvailbles(bool mode);
    void menu(int state);
};

#endif