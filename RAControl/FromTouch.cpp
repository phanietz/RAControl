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
  responseAT=Bluetooth.inq(false, SendToDisplay); //get errors: 0 or 2
  SendToDisplay.progressBar(75);
  SendToDisplay.sendSystemsAvailables(true, Bluetooth, Bluetooth.error);

}

void TOUCH::intoBox(String fromNextion, int cell){
  Serial.println("CONNECTING... TO ");
  SendToDisplay.connect(0);
  SendToDisplay.progressBar(25);

  //get errors: 0 or 4
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
  
  if(Bluetooth.error==0){ 
    SendToDisplay.progressBar(50); 
    //get errors: 0, 1 or 3
    connected=Bluetooth.axisReceiving(); //receive axis and pin wheel from box and then COPY
    if(Bluetooth.error==0){
      for(i=0; i<5; i++){
        if(Bluetooth.SystemsON[i][2].indexOf(String(Bluetooth.system))!=-1){
          Bluetooth.SystemsON[i][1]="-ON";
        }
      };
      Serial.println("SUCCESSFUL CONNECTION");
      SendToDisplay.progressBar(75);
      motorIndex=1;
      Box.motor1();
      SendToDisplay.firstDataUpdate(Bluetooth.axisStr[0], Bluetooth.axisStr[1], Bluetooth.axisStr[2], Bluetooth.switches);
      SendToDisplay.connect(1);
      Bluetooth.step=0.50;
    }
  }
  

  if(Bluetooth.error!=0){ //ERROR trying to LINK
      Serial.print("ERROR: ");
      Serial.println(Bluetooth.error);
      switch(Bluetooth.error){//*********************************
        case 1: //put system to color gray (OFF)
                for(i=0; i<5; i++){
                  if(Bluetooth.SystemsON[i][2].indexOf(String(Bluetooth.system))!=-1){
                    Bluetooth.SystemsON[i][1]="-OFF";
                  }
                };
                Serial.println("CONNECTION FAILED");
                //Bluetooth.disc();

        break;

        case 3: //put system to color red (ERROR)
                for(i=0; i<5; i++){
                  if(Bluetooth.SystemsON[i][2].indexOf(String(Bluetooth.system))!=-1){
                    Bluetooth.SystemsON[i][1]="-ERR";
                  }
                };                
                Serial.println("INCOMPLETE RECEIVED DATA");
                Bluetooth.disc();
        break;

        case 4:
                //put system to color red (ERROR)
                for(i=0; i<5; i++){
                  if(Bluetooth.SystemsON[i][2].indexOf(String(Bluetooth.system))!=-1){
                    Bluetooth.SystemsON[i][1]="-ERR";
                  }
                };                
                Serial.println("ERROR trying to LINK");
                Bluetooth.disc();
        break;
      }
      SendToDisplay.sendSystemsAvailables(false, Bluetooth, Bluetooth.error);     
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
  Bluetooth.error = 0;
  SendToDisplay.sendSystemsAvailables(false, Bluetooth, Bluetooth.error); 
  SendToDisplay.progressBar(75);  
}

void TOUCH::exitMenu(int e){

  switch(e){
    case 1:
            SendToDisplay.menu(0, Bluetooth);
    break;

    case 2:
            SendToDisplay.sendSystemsAvailables(true, Bluetooth, Bluetooth.error);
    break;
  }

}

void TOUCH::motor(int m){

  switch(m){
    case 1:
            motorIndex=1;
            Box.motor1();
    break;

    case 2:
            motorIndex=2;
            Box.motor2();
    break;

    case 3:
            motorIndex=3;
            Box.motor3();
    break;        
  }
  
}

void TOUCH::move(int m, int motor){
  Serial.print(Bluetooth.switches[0][0]); //m1 z1
  Serial.println(Bluetooth.switches[0][1]); //m1 z2
  Serial.print(Bluetooth.switches[1][0]); //m2 x1
  Serial.println(Bluetooth.switches[1][1]); //m2 x2
  Serial.print(Bluetooth.switches[2][0]); //m3 y1
  Serial.println(Bluetooth.switches[2][1]); //m3 y2

  float aux2;
  switch(m){
    case 1: //UP MOTOR 1     switches[0][0]=switch 1         switches[0][1]=switch 2
      Box.z1();
      if(Bluetooth.endOfpinWheel()==true){
        if(Bluetooth.switches[0][0]==0){ //z1 open
          Serial.println("switch open");
          //changing number on touch screen
          Bluetooth.axisNum[0]=Bluetooth.axisNum[0]+Bluetooth.step;
          Bluetooth.axisStr[0]=String(Bluetooth.axisNum[0]);
          Bluetooth.axisStr[0].replace(".","0");     
          SendToDisplay.dataPosition(1,Bluetooth.axisStr[0]); //put new number
////////////////////////////////////////////////////// im hereeeee
          //verify if there is any change in the switches state (both switches)
          SendToDisplay.activeDeactiveTouch("plusZ", true);
          if(Bluetooth.switches[0][1]==0){
            SendToDisplay.activeDeactiveTouch("lessZ", true); 
          }else{
            SendToDisplay.activeDeactiveTouch("lessZ", false); 
          }
          Serial.println(Bluetooth.axisNum[0]);
        }else{ //z1 closed
          Serial.println("switch close");
          SendToDisplay.endOfWheel(1);
          SendToDisplay.activeDeactiveTouch("plusZ", false);          
        }
      }else{ //Comunication error
        Serial.println("COMUNICATION ERROR");
      }   
    break;    

    case 2: //DOWN MOTOR 1     switches[0][0]=switch 1         switches[0][1]=switch 2
      Box.z2();
      if(Bluetooth.endOfpinWheel()==true){
        Serial.println("switch open");
        Bluetooth.axisNum[0]=Bluetooth.axisNum[0]-Bluetooth.step;
        Bluetooth.axisStr[0]=String(Bluetooth.axisNum[0]);
        Bluetooth.axisStr[0].replace(".","0");
        SendToDisplay.dataPosition(1,Bluetooth.axisStr[0]);
        SendToDisplay.activeDeactiveTouch("lessZ", true);
        Serial.println(Bluetooth.axisNum[0]);
      }else{
        Serial.println("switch close");
        SendToDisplay.endOfWheel(2);
        SendToDisplay.activeDeactiveTouch("lessZ", false);
      }  
    break;    

    case 3: //FORWARD MOTOR 2     switches[1][0]=switch 1         switches[1][1]=switch 2
      Box.x1();
      if(Bluetooth.endOfpinWheel()==true){
        Serial.println("switch open");
        Bluetooth.axisNum[1]=Bluetooth.axisNum[1]+Bluetooth.step;
        Bluetooth.axisStr[1]=String(Bluetooth.axisNum[1]);
        Bluetooth.axisStr[1].replace(".","0");               
        SendToDisplay.dataPosition(2,Bluetooth.axisStr[1]);
        SendToDisplay.activeDeactiveTouch("plusX", true);
        Serial.println(Bluetooth.axisNum[1]);      
      }else{
        Serial.println("switch close");
        SendToDisplay.endOfWheel(3);
        SendToDisplay.activeDeactiveTouch("plusX", false);
      }  
    break;

    case 4: //BACKWARD MOTOR 2     switches[1][0]=switch 1         switches[1][1]=switch 2
      Box.x2();
      if(Bluetooth.endOfpinWheel()==true){
        Serial.println("switch open");
        if(Bluetooth.axisNum[1]-Bluetooth.step>=0){
          Bluetooth.axisNum[1]=Bluetooth.axisNum[1]-Bluetooth.step;
          Bluetooth.axisStr[1]=String(Bluetooth.axisNum[1]);
          Bluetooth.axisStr[1].replace(".","0");               
          SendToDisplay.dataPosition(2,Bluetooth.axisStr[1]);
          SendToDisplay.activeDeactiveTouch("lessX", true);
          Serial.println(Bluetooth.axisNum[1]);
        }
      }else{
        Serial.println("switch close");
        SendToDisplay.endOfWheel(4);
        SendToDisplay.activeDeactiveTouch("lessX", false);
      }
    break;

    case 5: //LEFT MOTOR 3     switches[2][0]=switch 1         switches[2][1]=switch 2
      Box.y1();
      if(Bluetooth.endOfpinWheel()==true){
        Serial.println("switch open");
        Bluetooth.axisNum[2]=Bluetooth.axisNum[2]+Bluetooth.step;
        Bluetooth.axisStr[2]=String(Bluetooth.axisNum[2]);
        Bluetooth.axisStr[2].replace(".","0");               
        SendToDisplay.dataPosition(3,Bluetooth.axisStr[2]);
        SendToDisplay.activeDeactiveTouch("plusY", true);
        Serial.println(Bluetooth.axisNum[2]);
      }else{
        Serial.println("switch close");
        SendToDisplay.endOfWheel(5);
        SendToDisplay.activeDeactiveTouch("plusY", false);
      }
    break;        

    case 6: //RIGHT MOTOR 3     switches[2][0]=switch 1         switches[2][1]=switch 2
      Box.y2();
      if(Bluetooth.endOfpinWheel()==true){
        Serial.println("switch open");
        if(Bluetooth.axisNum[2]-Bluetooth.step>=0){
          Bluetooth.axisNum[2]=Bluetooth.axisNum[2]-Bluetooth.step;
          Bluetooth.axisStr[2]=String(Bluetooth.axisNum[2]);
          Bluetooth.axisStr[2].replace(".","0");               
          SendToDisplay.dataPosition(3,Bluetooth.axisStr[2]);
          SendToDisplay.activeDeactiveTouch("lessY", true);
          Serial.println(Bluetooth.axisNum[2]);
        }
      }else{
        Serial.println("switch close");
        SendToDisplay.endOfWheel(6);
        SendToDisplay.activeDeactiveTouch("lessY", false);
      }
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

  aux="";
  fromNextion="";
  do{
    if (Serial2.available()){    
      aux = char(Serial2.read());
      fromNextion += aux;
    }
  }while(aux.endsWith(".")==false);

  if(fromNextion.indexOf("Yes")!=-1){
    if(motorIndex==1){
      SendToDisplay.picture("80");
    }else if(motorIndex==2){
      SendToDisplay.picture("85");
    }else if(motorIndex==3){
      SendToDisplay.picture("90");
    }    
    SendToDisplay.progressBar(30);
    Box.reboot();

    //wait position 0
    do{
      exit=Bluetooth.waitCopy();
    }while(exit==false);


    SendToDisplay.progressBar(60);
    if(motorIndex==1){
      SendToDisplay.picture("81");
    }else if(motorIndex==2){
      SendToDisplay.picture("86");
    }else if(motorIndex==3){
      SendToDisplay.picture("91");
    }   

    //wait signal finish moving 
    do{
      exit=Bluetooth.waitCopy();
    }while(exit==false);

    SendToDisplay.progressBar(90);

    if(motorIndex==1){
      SendToDisplay.connect(1);
    }else if(motorIndex==2){
      SendToDisplay.connect(2);
    }else if(motorIndex==3){
      SendToDisplay.connect(3);
    }
  }
}