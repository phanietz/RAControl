#include "FromTouch.h"
#include "SendtoDisplay.h"
#include "SendtoBox.h"
#include "ATcommands.h"

HMI SendToDisplay;
BOX Box;
AT Bluetooth;

TOUCH::TOUCH(){};


void TOUCH::intoSPASystem(int search){

  SendToDisplay.refresh();
  SendToDisplay.progressBar(10);
  if(search==1){
    responseAT=Bluetooth.inqm("1,9,8");
  }
  SendToDisplay.progressBar(25);
  responseAT=Bluetooth.reset();
  responseAT=Bluetooth.init();
  SendToDisplay.progressBar(50);
  responseAT=Bluetooth.inq(false, SendToDisplay); //progress 50, 75 and 100

  SendToDisplay.progressBar(75);
  if(responseAT.compareTo("Yes")==0){
    SendToDisplay.sendSystemsAvailables(true, Bluetooth);
  }  
}


void TOUCH::intoBox(String fromNextion, int cell){
  Serial.println("CONNECTING... TO ");
  SendToDisplay.connect(0);
  SendToDisplay.progressBar(25);

  switch(cell){
    case 201:
            Bluetooth.system=201;
            responseAT=Bluetooth.link(201);
    break;

    case 202:
            Bluetooth.system=202;
            responseAT=Bluetooth.link(202);
    break;

    case 204:
            Bluetooth.system=204;
            responseAT=Bluetooth.link(204);
    break;

    case 205:
            Bluetooth.system=205;
            responseAT=Bluetooth.link(205);
    break;
    case 209:
            Bluetooth.system=209;
            responseAT=Bluetooth.link(209);
    break;

    case 210:
            Bluetooth.system=210;
            responseAT=Bluetooth.link(210);
    break;

    case 211:
            Bluetooth.system=211;
            responseAT=Bluetooth.link(211);
    break;

    case 212:
            Bluetooth.system=212;
            responseAT=Bluetooth.link(212);
    break;         
  }
  
  SendToDisplay.progressBar(50);
  if(responseAT.indexOf("errorLINK")!=-1){
    Serial.println("ERROR trying to LINK, wrong response");
  }
  
  connected=Bluetooth.axisReceiving(); //receive axis from box and then COPY

  if(connected==true){
    Serial.println("SUCCESSFUL CONNECTION");
    SendToDisplay.progressBar(75);
    Box.motor1();
    SendToDisplay.firstDataUpdate(Bluetooth.axisStr[0], Bluetooth.axisStr[1], Bluetooth.axisStr[2]);
    SendToDisplay.connect(1);
    Bluetooth.step=0.50;

  }else{
    Serial.println("ERROR trying to connect, System offline");
    int i=0;
    for(i=0; i<5; i++){
      if(Bluetooth.SystemsON[i][2].indexOf(String(Bluetooth.system))!=-1){
        Bluetooth.SystemsON[i][1]="-OFF";
      }
    };
    SendToDisplay.sendSystemsAvailables(false, Bluetooth); 
  }
}

void TOUCH::exitBox(){
  Serial.println("------DESCONECTAR-----");
  Box.exit();
  SendToDisplay.disconnect(1);
  SendToDisplay.progressBar(25);
  responseAT=Bluetooth.disc();
  SendToDisplay.progressBar(50);
  SendToDisplay.disconnect(2);
  SendToDisplay.sendSystemsAvailables(false, Bluetooth); 
  SendToDisplay.progressBar(75);  
}

void TOUCH::exitMenu(int e){

  switch(e){
    case 1:
            SendToDisplay.menu(0, Bluetooth);
    break;

    case 2:
            SendToDisplay.sendSystemsAvailables(true, Bluetooth);
    break;
  }

}

void TOUCH::motor(int m){

  switch(m){
    case 1:
            Box.motor1();
    break;

    case 2:
            Box.motor2();
    break;

    case 3:
            Box.motor3();
    break;        
  }
  
}

void TOUCH::move(int m, int motor){

  switch(m){
    case 1:
            Box.x1();
            Bluetooth.axisNum[1]=Bluetooth.axisNum[1]+Bluetooth.step;
            Bluetooth.axisStr[1]=String(Bluetooth.axisNum[1]);
            Bluetooth.axisStr[1].replace(".","0");               
            SendToDisplay.dataPosition(2,Bluetooth.axisStr[1]);
            Serial.println(Bluetooth.axisNum[1]);
    break;

    case 2:
            Box.x2();
            Bluetooth.axisNum[1]=Bluetooth.axisNum[1]-Bluetooth.step;
            Bluetooth.axisStr[1]=String(Bluetooth.axisNum[1]);
            Bluetooth.axisStr[1].replace(".","0");               
            SendToDisplay.dataPosition(2,Bluetooth.axisStr[1]);
            Serial.println(Bluetooth.axisNum[1]);
    break;

    case 3:
            Box.y1();
            Bluetooth.axisNum[2]=Bluetooth.axisNum[2]+Bluetooth.step;
            Bluetooth.axisStr[2]=String(Bluetooth.axisNum[2]);
            Bluetooth.axisStr[2].replace(".","0");               
            SendToDisplay.dataPosition(3,Bluetooth.axisStr[2]);
            Serial.println(Bluetooth.axisNum[2]);
    break;        

    case 4:
            Box.y2();
            Bluetooth.axisNum[2]=Bluetooth.axisNum[2]-Bluetooth.step;
            Bluetooth.axisStr[2]=String(Bluetooth.axisNum[2]);
            Bluetooth.axisStr[2].replace(".","0");               
            SendToDisplay.dataPosition(3,Bluetooth.axisStr[2]);
            Serial.println(Bluetooth.axisNum[2]);
    break;    

    case 5:
            Box.z1();
            Bluetooth.axisNum[0]=Bluetooth.axisNum[0]+Bluetooth.step;
            Bluetooth.axisStr[0]=String(Bluetooth.axisNum[0]);
            Bluetooth.axisStr[0].replace(".","0");           
            SendToDisplay.dataPosition(1,Bluetooth.axisStr[0]);
            Serial.println(Bluetooth.axisNum[0]);
    break;    

    case 6:
            Box.z2();
            Bluetooth.axisNum[0]=Bluetooth.axisNum[0]-Bluetooth.step;
            Bluetooth.axisStr[0]=String(Bluetooth.axisNum[0]);
            Bluetooth.axisStr[0].replace(".","0");
            SendToDisplay.dataPosition(1,Bluetooth.axisStr[0]);
            Serial.println(Bluetooth.axisNum[0]);
    break;    
         
  }

  if(Bluetooth.step==1.00){
    delay(200);
  }
}

void TOUCH::length(int l){
  switch(l){
    case 1:
            Box.step(1); 
            Bluetooth.step=0.25;
    break;

    case 2:
            Box.step(2);
            Bluetooth.step=0.50;
    break;

    case 3:
            Box.step(3);
            Bluetooth.step=1.00;
    break;        
  }
}

void TOUCH::reboot(String fromNextion){
  endwait=false;
  do{
    if (Serial2.available()){    
      aux = char(Serial2.read());
      if(aux.endsWith(".")){
        if(fromNextion.indexOf("Yes")!=-1){
          Box.reboot();
          endwait=true;
        }
        if(fromNextion.indexOf("No")!=-1){
          endwait=true;
        }          
        fromNextion ="";
      }else{
        fromNextion += aux;
      }
    }
  }while(endwait==false);
  Serial.println("Exit reboot");
}