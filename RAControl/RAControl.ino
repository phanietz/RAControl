/*
-------BLUETOOTH INTERFACE-------

////////BOXES//////////////
Name: SPAS_201, SPAS_202, SPAS_204, ...
ADDR: xx:x:xxxxx
PSWD: rt4w23
Role: 0 (slave)

////////CONTROL//////////////
Name: Master
ADDR: xx:x:xxxxx
PSWD: rt4w23master
Role: 1 (master)
*/
/*
 * Pinout:
 * Key/ENABLE --> pin 9
 * VCC --> Vin
 * TXD --> RX SERIAL3
 * RXD --> TX SERIAL3
 * GND --> GND
 */

/*Control remote has 2 kind of comunications TX-RX
1. Arduino MEGA with nextion Display (TX2, RX2)
2. Arduino MEGA with bluetooth interface (TX3, RX3)
*/

 //PUT SERIAL MONITOR: BOTH NL & CR
 //PUT 9600 BAUD TO SERIAL MONITOR
 //PUT 38400 BAUD TO SERIAL BLUETOOTH

#include "FromTouch.h"
#define SIGNALINK 53

String data_from_nextion, aux;
void main2(String data_from_nextion);
TOUCH ReceivingFromTouch;

void setup(){
  pinMode(9, OUTPUT);
  digitalWrite(9, HIGH);
  Serial.begin(9600);

  pinMode(15, INPUT_PULLUP); //RX Serial 2 Bluetooth
  pinMode(17, INPUT_PULLUP); //RX Serial 3 NEXTION
  pinMode(SIGNALINK, INPUT_PULLUP); //LED 3 NEXTION

  Serial3.begin(38400);  // HC-05 default speed in AT command mode
  Serial2.begin(9600);  // NEXTION
  Serial2.flush();
  //delay(1000);
  Serial.println("screen info:");
  Serial.println("testing");
}
 
void loop(){
  
  if (Serial2.available()){    
    aux = char(Serial2.read());
    //Serial.println(aux);
    if(aux.endsWith(".")){

      main2(data_from_nextion);   
      data_from_nextion ="";      

    }else{
      //Serial.print("DATO: ");
      //Serial.println(aux);
      data_from_nextion += aux;
    }
  }
  //data_from_nextion =""; 

}

void main2(String data_from_nextion){

  Serial.print("From Display: ");
  Serial.println(data_from_nextion);                                                                                                                                                   

  if(data_from_nextion.indexOf("connectSPAS")!=-1){
    ReceivingFromTouch.intoSPASystem(1);
  }

  if(data_from_nextion.indexOf("Refresh")!=-1){
    ReceivingFromTouch.intoSPASystem(0);
  }

  if(data_from_nextion.indexOf("SPAS2")!=-1){
    Serial.println(data_from_nextion.substring(data_from_nextion.indexOf("SPAS")+4, data_from_nextion.indexOf("SPAS")+7));
    ReceivingFromTouch.intoBox(data_from_nextion, (data_from_nextion.substring(data_from_nextion.indexOf("SPAS")+4, data_from_nextion.indexOf("SPAS")+7)).toInt());
  }

  if(data_from_nextion.indexOf("Disconnect")!=-1){
    ReceivingFromTouch.exitBox();
  }

  if(data_from_nextion.indexOf("exitMenu-access0")!=-1){
    ReceivingFromTouch.exitMenu(1);
  }
  
  if(data_from_nextion.indexOf("exitMenu-access1")!=-1){
    ReceivingFromTouch.exitMenu(2);
  }
  
  if(data_from_nextion.indexOf("SPASmotor1")!=-1){
    ReceivingFromTouch.motor(1);
  }

  if(data_from_nextion.indexOf("SPASmotor2")!=-1){
    ReceivingFromTouch.motor(2);
  }

  if(data_from_nextion.indexOf("SPASmotor3")!=-1){
    ReceivingFromTouch.motor(3);
  }

  if(data_from_nextion.indexOf("+x")!=-1){
    ReceivingFromTouch.move(1,2);
  }

  if(data_from_nextion.indexOf("-x")!=-1){
    ReceivingFromTouch.move(2,2); 
  }

  if(data_from_nextion.indexOf("+y")!=-1){
    ReceivingFromTouch.move(3,3); 
  }

  if(data_from_nextion.indexOf("-y")!=-1){
    ReceivingFromTouch.move(4,3); 
  }

  if(data_from_nextion.indexOf("+z")!=-1){
    ReceivingFromTouch.move(5,1); 
  }

  if(data_from_nextion.indexOf("-z")!=-1){
    ReceivingFromTouch.move(6,1); 
  }

  if(data_from_nextion.indexOf("StepL")!=-1){
    ReceivingFromTouch.length(1);
  }

  if(data_from_nextion.indexOf("StepM")!=-1){
    ReceivingFromTouch.length(2);
  }

  if(data_from_nextion.indexOf("StepH")!=-1){
    ReceivingFromTouch.length(3);
  }

  if(data_from_nextion.indexOf("calibrate")!=-1){
    ReceivingFromTouch.reboot(data_from_nextion);
  }        
}