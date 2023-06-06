#ifndef SENDTODISPLAY_H
#define SENDTODISPLAY_H
#include <Arduino.h>

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
};

#endif