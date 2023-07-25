#ifndef FROMTOUCH_H
#define FROMTOUCH_H
#include <Arduino.h>

class TOUCH{
  private:
    String responseAT, aux="";
    bool endwait=false, connected=false, exit=false;
    int i=0, motorIndex=0;
  public:
    TOUCH();
    void intoSPASystem(int search);
    void intoBox(String fromNextion, int cell);
    void exitBox();
    void exitMenu(int e);
    void motor(int m);
    void move(int m, int motor);
    void length(int l);
    void reboot(String fromNextion);
};

#endif