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
#include "ATcommands.h"
//#include "string.h"

String data_from_nextion="", aux="";

void send(String command);
void variables(String data_from_nextion);
void conectar();
void desconectar();

AT Bluetooth;

void setup(){
  pinMode(ENABLE, OUTPUT);
  digitalWrite(ENABLE, HIGH);
  Serial.begin(9600);

  pinMode(15, INPUT_PULLUP); //RX Serial 2 Bluetooth
  pinMode(17, INPUT_PULLUP); //RX Serial 3 NEXTION

  Serial3.begin(38400);  // HC-05 default speed in AT command mode
  Serial2.begin(9600);  // NEXTION
  Serial2.flush();
  //delay(1000);
  Serial.println("screen info:");
  Serial.println("testing");
}
 
void loop(){
  
  if (Serial2.available()){
    //Serial.println(aux);
    aux = char(Serial2.read());
    if(aux.endsWith(".")){

      variables(data_from_nextion);   
      data_from_nextion ="";

    }else{
      //Serial.print("DATO: ");
      //Serial.println(aux);
      data_from_nextion += aux;
    }
  }

}

/*
void send(String text){
  char a;
  text=" fr";
  Serial3.write("hhh");
  while(Serial3.available()){
    if(Serial3.available()>0){
      a = char(Serial3.read());
      responseAT += a;    
    }
  }
  Serial.println(responseAT);
  Serial.println(responseAT.length());
  Serial.print(responseAT);
  Serial.print(responseAT.substring(0, responseAT.length()-2)); //remove \r\n from command string  
  responseAT="";
  delay(1000);

}
*/


void desconectar(){

  
  /*
    Serial3.write("at+state?\r\n");
    while(Serial3.available()>0){
      char b = Serial3.read();
      Serial.write(b);
    }  
    delay(1000);


    Serial3.write("at+disc\r\n");
    while(Serial3.available()>0){
      char a = Serial3.read();
      Serial.write(a);
    }
    delay(1000);


    Serial3.write("at+state?\r\n");
    while(Serial3.available()>0){
      char b = Serial3.read();
      Serial.write(b);
    }  
    delay(1000);
  */
}


void variables(String data_from_nextion){

  Serial.println(data_from_nextion);
  Serial.println(data_from_nextion.length());                                                                                                                                                         

  if(data_from_nextion.indexOf("connect")!=-1){
    Bluetooth.config();
    Bluetooth.refresh();
  }

  if(data_from_nextion.indexOf("refresh")!=-1){
    Bluetooth.refresh();
  }

  if(data_from_nextion.indexOf("Systems available")!=-1){
    Bluetooth.SendSystemsAvailbles();
  }

  if(data_from_nextion.compareTo("connect201")==0){
    Bluetooth.connect(201);
  }
  if(data_from_nextion.compareTo("connect202")==0){
    Bluetooth.connect(202);
  }
  if(data_from_nextion.compareTo("connect205")==0){
    Bluetooth.connect(205);
  }

  if(data_from_nextion.compareTo("disconnect201")==0){
    Bluetooth.disconnect();
  }

  Serial.println("TERMINA----------");
}