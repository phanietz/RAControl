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
    void firstDataUpdate(String a, String b, String c);
    void dataPosition(int send, String axis);
    void setDatafromMemorytoDisplay();
};

#endif