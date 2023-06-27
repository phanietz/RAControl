//The communication with the DISPLAY is through Serial2

#ifndef SENDTODISPLAY_H
#define SENDTODISPLAY_H
#include <Arduino.h>

class Display{
  private:
    String responseAT="", param="";
    int i=0;
  public:
    Display();
    void refresh();
    void connect(int send);    
    void disconnect(int send);
    void SendSystemsAvailables(bool mode, class AT &Blue);
    void ProgressBar(int value);
    void menu(int state, class AT &Blue);
};

#endif