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

 //PAIR to SLAVE SPAS_202

#include "SendtoDisplay.h"
#include "SendtoBox.h"
#include "ATcommands.h"

#define SIGNALink 53

String data_from_nextion="", aux="";
String responseAT;
bool endwait=false;
void variables(String data_from_nextion);

Display DisplayTouch;
Box box;
AT Bluetooth;

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

  Serial.print("From Display: ");
  Serial.println(data_from_nextion);
  //Serial.println(data_from_nextion.length());                                                                                                                                                         


  if(data_from_nextion.indexOf("connectSPAS")!=-1 or data_from_nextion.indexOf("Refresh")!=-1){
    if(data_from_nextion.indexOf("connectSPAS")!=-1){
      responseAT=Bluetooth.INQM("1,9,8");
    }
    responseAT=Bluetooth.RESET();
    DisplayTouch.refresh();
    responseAT=Bluetooth.INIT();


    responseAT=Bluetooth.INQ(false, DisplayTouch); //progress 50, 75 and 100


    if(responseAT.compareTo("Yes")==0){
      DisplayTouch.SendSystemsAvailables(false, Bluetooth);
    }
  }


  if(data_from_nextion.indexOf("SPAS2")!=-1){
    Serial.println("------CONECTAR-----");
    DisplayTouch.connect(1);

    if(data_from_nextion.indexOf("SPAS201")!=-1){
      responseAT=Bluetooth.LINK(201);
    }else if(data_from_nextion.indexOf("SPAS202")!=-1){
      responseAT=Bluetooth.LINK(202);
    }else if(data_from_nextion.indexOf("SPAS204")!=-1){
      responseAT=Bluetooth.LINK(204);
    }else if(data_from_nextion.indexOf("SPAS205")!=-1){
      responseAT=Bluetooth.LINK(205);
    }else if(data_from_nextion.indexOf("SPAS209")!=-1){
      responseAT=Bluetooth.LINK(209);
    }else if(data_from_nextion.indexOf("SPAS210")!=-1){
      responseAT=Bluetooth.LINK(210);
    }else if(data_from_nextion.indexOf("SPAS211")!=-1){
      responseAT=Bluetooth.LINK(211);
    }else if(data_from_nextion.indexOf("SPAS212")!=-1){
      responseAT=Bluetooth.LINK(212);
    }
    
    DisplayTouch.connect(2);
    box.motor1();
  }


  if(data_from_nextion.indexOf("Disconnect")!=-1){
    Serial.println("------DESCONECTAR-----");
    box.exit();
    DisplayTouch.disconnect(1);
    responseAT=Bluetooth.DISC();
    DisplayTouch.disconnect(2);
    DisplayTouch.disconnect(3);
    DisplayTouch.SendSystemsAvailables(false, Bluetooth); 
  }


  if(data_from_nextion.indexOf("exitMenu-access0")!=-1){
    DisplayTouch.menu(0, Bluetooth);
  }else if(data_from_nextion.indexOf("exitMenu-access1")!=-1){
    DisplayTouch.SendSystemsAvailables(true, Bluetooth);
  }
  

  //////////////////////////////////////////
  ////////////INTO SPASystem////////////////
  //////////////////////////////////////////
  if(data_from_nextion.indexOf("SPASmotor1")!=-1){
    box.motor1();
  }

  if(data_from_nextion.indexOf("SPASmotor2")!=-1){
    box.motor2();
  }

  if(data_from_nextion.indexOf("SPASmotor3")!=-1){
    box.motor3();
  }

  if(data_from_nextion.indexOf("+x")!=-1){
    box.x1();
  }

  if(data_from_nextion.indexOf("-x")!=-1){
    box.x2();
  }

  if(data_from_nextion.indexOf("+y")!=-1){
    box.y1();
  }

  if(data_from_nextion.indexOf("-y")!=-1){
    box.y2();
  }

  if(data_from_nextion.indexOf("+z")!=-1){
    box.z1();
  }

  if(data_from_nextion.indexOf("-z")!=-1){
    box.z2();
  }

  if(data_from_nextion.indexOf("StepL")!=-1){
    box.step(1); 
  }

  if(data_from_nextion.indexOf("StepM")!=-1){
    box.step(2);
  }

  if(data_from_nextion.indexOf("StepH")!=-1){
    box.step(3);
  }

  if(data_from_nextion.indexOf("calibrate")!=-1){
    endwait=false;
    do{
      if (Serial2.available()){    
        aux = char(Serial2.read());
        if(aux.endsWith(".")){
          if(data_from_nextion.indexOf("Yes")!=-1){
            box.reboot();
            endwait=true;
          }
          if(data_from_nextion.indexOf("No")!=-1){
            endwait=true;
          }          
          data_from_nextion ="";
        }else{
          data_from_nextion += aux;
        }
      }
    }while(endwait==false);
    Serial.println("Exit reboot");
  }        
}