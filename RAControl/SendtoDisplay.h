//The communication with the DISPLAY is through Serial2

#ifndef SENDTODISPLAY_H
#define SENDTODISPLAY_H
#include <Arduino.h>

class HMI{
  private:
    String responseAT="", param="";
    int i=0;
  public:
    HMI();
    void refresh();
    void connect(int send);    
    void disconnect(int send);
    void sendSystemsAvailables(bool mode, class AT &Blue, int error);
    void progressBar(int value);
    void menu(int state, class AT &Blue);
    void firstDataUpdate(String a, String b, String c, int s[3][2]);
    void dataPosition(int send, String axis);
    void picture(String pic);
    void endOfWheel(int end);
    void activeDeactiveTouch(String button, bool state);
};

#endif