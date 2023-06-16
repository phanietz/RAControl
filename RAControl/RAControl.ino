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
1. Arduino MEGA with nextion display (TX2, RX2)
2. Arduino MEGA with bluetooth interface (TX3, RX3)
*/

 //PUT SERIAL MONITOR: BOTH NL & CR
 //PUT 9600 BAUD TO SERIAL MONITOR
 //PUT 38400 BAUD TO SERIAL BLUETOOTH

 //PAIR to SLAVE SPAS_202

#include "SendtoDisplay.h"
#include "SendtoBox.h"
#define SIGNALink 53

String data_from_nextion="", aux="";
void variables(String data_from_nextion);

Display display;
Box box;

void setup(){
  pinMode(9, OUTPUT);
  digitalWrite(9, HIGH);
  Serial.begin(9600);

  pinMode(15, INPUT_PULLUP); //RX Serial 2 Bluetooth
  pinMode(17, INPUT_PULLUP); //RX Serial 3 NEXTION
  pinMode(SIGNALink, INPUT_PULLUP); //LED 3 NEXTION

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

      variables(data_from_nextion);   
      data_from_nextion ="";      

    }else{
      //Serial.print("DATO: ");
      //Serial.println(aux);
      data_from_nextion += aux;
    }
  }
  //data_from_nextion =""; 

}


void variables(String data_from_nextion){

  Serial.println(data_from_nextion);
  //Serial.println(data_from_nextion.length());                                                                                                                                                         

//////////////////DISPLAY/////////////////////////
  if(data_from_nextion.indexOf("connectSPAS")!=-1){
    display.start();
    display.refresh();
  }

  if(data_from_nextion.indexOf("Refresh")!=-1){
    display.refresh();
  }

  if(data_from_nextion.indexOf("SPAS201")!=-1){
    display.connect(201);
  }

  if(data_from_nextion.indexOf("SPAS202")!=-1){
    display.connect(202);
  }

  if(data_from_nextion.indexOf("SPAS205")!=-1){
    display.connect(205);
  }

  //exit
  if(data_from_nextion.indexOf("exitMenu-access0")!=-1){
    display.menu(0);
  }else if(data_from_nextion.indexOf("exitMenu-access1")!=-1){
    display.SendSystemsAvailbles(true);
  }else if(data_from_nextion.indexOf("exit")!=-1){
    display.disconnect();
    display.SendSystemsAvailbles(true);
  }
  
  /////////////////////////////////////////////////
  /////////////////////BOX/////////////////////////
  if(data_from_nextion.indexOf("SPASmotor1")!=-1){
    //send to box
    box.motor1();
    //sen to display
  }
  if(data_from_nextion.indexOf("SPASmotor2")!=-1){
    //box
    box.motor2();
    //display    
  }
  if(data_from_nextion.indexOf("SPASmotor3")!=-1){
    //box
    box.motor3();
    //display    
  }

  if(data_from_nextion.indexOf("+x")!=-1){
    //send to box
    box.x1();

    //sen to display
  }
  if(data_from_nextion.indexOf("-x")!=-1){
    //box
    box.x2();
    //display    
  }

  if(data_from_nextion.indexOf("+y")!=-1){
    //send to box
    box.y1();

    //sen to display
  }
  if(data_from_nextion.indexOf("-y")!=-1){
    //box
    box.y2();
    //display    
  }

  if(data_from_nextion.indexOf("+z")!=-1){
    //send to box
    box.z1();

    //sen to display
  }
  if(data_from_nextion.indexOf("-z")!=-1){
    //box
    box.z2();
    //display    
  }
}