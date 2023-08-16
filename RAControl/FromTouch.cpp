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

  float aux2;
  switch(m){
    case 1: //UP:z1:switch1:motor1     switches[0][0]=z1         switches[0][1]=z2
      Box.z1();
      if(Bluetooth.endOfpinWheel()==true){
        Serial.print("Z1: ");
        Serial.println(Bluetooth.switches[0][0]);
        Serial.print("Z2: ");
        Serial.println(Bluetooth.switches[0][1]);
        if(Bluetooth.switches[0][0]==0){ //z1 open?
          Serial.println("switch open z1...");
          //changing number on touch screen ************************************
          Bluetooth.axisNum[0]=Bluetooth.axisNum[0]+Bluetooth.step;
          Bluetooth.axisStr[0]=String(Bluetooth.axisNum[0]);
          Bluetooth.axisStr[0].replace(".","0");     
          SendToDisplay.dataPosition(1,Bluetooth.axisStr[0]); //put new number on touch screen
          //Any change in switche state? ***************************************
          SendToDisplay.activeDeactiveTouch("plusZ", true);
          if(Bluetooth.switches[0][1]==0){ //z2 open?
            SendToDisplay.endOfWheel(0,1); //z1 and z2 opened, motor1
            SendToDisplay.activeDeactiveTouch("lessZ", true); //button z2 activated
          }else{ //z2 closed
            Serial.println("switch close lessZ Z2");
            SendToDisplay.endOfWheel(2,1); //z1 opened and z2 closed, motor1
            SendToDisplay.activeDeactiveTouch("lessZ", false); //button z2 blocked
          }
        }else{ //z1 closed
          Serial.println("switch close plusZ Z1");
          if(Bluetooth.switches[0][1]==0){ //z2 open?
            SendToDisplay.endOfWheel(1,1); //z1 closed and z2 open, motor1
            SendToDisplay.activeDeactiveTouch("plusZ", false); //button z1 blocked
          }else{ //z2 closed
            //z1 and z2 closed, motor1 -----ERROR
            Serial.println("switch close z1 and z2"); //shouldn't happen
          }
        }
      }else{ //Comunication error
        Serial.println("BLUETOOTH COMUNICATION ERROR: UP");
      }   
    break;    

    case 2: //DOWN:z2:switch2:motor1     switches[0][0]=z1         switches[0][1]=z2
      Box.z2();
      if(Bluetooth.endOfpinWheel()==true){
        Serial.print("Z1: ");
        Serial.println(Bluetooth.switches[0][0]);
        Serial.print("Z2: ");
        Serial.println(Bluetooth.switches[0][1]);
        if(Bluetooth.switches[0][1]==0){ //z2 open?
          Serial.println("switch open z2...");
          //changing number on touch screen ************************************
          Bluetooth.axisNum[0]=Bluetooth.axisNum[0]-Bluetooth.step;
          Bluetooth.axisStr[0]=String(Bluetooth.axisNum[0]);
          Bluetooth.axisStr[0].replace(".","0");
          SendToDisplay.dataPosition(1,Bluetooth.axisStr[0]);
          //Any change in switche state? ***************************************
          SendToDisplay.activeDeactiveTouch("lessZ", true);
          if(Bluetooth.switches[0][0]==0){ //z1 open?
            SendToDisplay.endOfWheel(0,1);  //z1 and z2 opened, motor1
            SendToDisplay.activeDeactiveTouch("plusZ", true); //button z1 activated
          }else{ //z1 closed
            Serial.println("switch close plusZ Z1");
            SendToDisplay.endOfWheel(1,1); //z1 closed and z2 open, motor1
            SendToDisplay.activeDeactiveTouch("plusZ", false); //button z1 blocked
          }
        }else{ //z2 closed
          Serial.println("switch close lessZ Z2");
          if(Bluetooth.switches[0][0]==0){ //z1 open?
            SendToDisplay.endOfWheel(2,1); //z1 open and z2 closed, motor1
            SendToDisplay.activeDeactiveTouch("lessZ", false); //button z2 blocked
          }else{ //z1 closed
            //z1 and z2 closed, motor1 -----ERROR
            Serial.println("switch close Z1 and Z2"); //shouldn't happen
          }
        }
      }else{ //Comunication error
        Serial.println("BLUETOOTH COMUNICATION ERROR: DOWN");  
      }  
    break;    

    case 3: //FORWARD:x1:switch1:motor2     switches[1][0]=x1         switches[1][1]=x2
      Box.x1();      
      if(Bluetooth.endOfpinWheel()==true){
        Serial.print("X1: ");
        Serial.println(Bluetooth.switches[1][0]);
        Serial.print("X2: "); 
        Serial.println(Bluetooth.switches[1][1]);       
        if(Bluetooth.switches[1][0]==0){ //x1 open?
          Serial.println("switch open x1...");
          //changing number on touch screen ************************************
          Bluetooth.axisNum[1]=Bluetooth.axisNum[1]+Bluetooth.step;
          Bluetooth.axisStr[1]=String(Bluetooth.axisNum[1]);
          Bluetooth.axisStr[1].replace(".","0");               
          SendToDisplay.dataPosition(2,Bluetooth.axisStr[1]);
          //Any change in switche state? ***************************************
          SendToDisplay.activeDeactiveTouch("plusX", true);
          if(Bluetooth.switches[1][1]==0){ //x2 open?
            SendToDisplay.endOfWheel(0,2); //x1 and x2 opened, motor2
            SendToDisplay.activeDeactiveTouch("lessX", true); //button x2 activated
          }else{
            Serial.println("switch close lessX X2");
            SendToDisplay.endOfWheel(4,2); //x1 opened and x2 closed, motor2
            SendToDisplay.activeDeactiveTouch("lessX", false); //button x2 blocked
          }
        }else{ //x1 closed
          Serial.println("switch close plusX X1");
          if(Bluetooth.switches[1][1]==0){ //x2 open?
            SendToDisplay.endOfWheel(3,2); //x1 closed x2 open, motor2
            SendToDisplay.activeDeactiveTouch("plusX", false); //button x1 blocked
          }else{ //x2 closed
            //x1 and x2 closed, motor2 -----ERROR
            Serial.println("switch close x1 and x2"); //shouldn't happen
          }
        }  
      }else{ //Comunication error
        Serial.println("BLUETOOTH COMUNICATION ERROR: FORWARD");
      }  
    break;

    case 4: //BACKWARD:x2:switch2:motor2     switches[1][0]=x1         switches[1][1]=x2
      Box.x2();
      if(Bluetooth.endOfpinWheel()==true){
        Serial.print("X1: ");
        Serial.println(Bluetooth.switches[1][0]);
        Serial.print("X2: ");
        Serial.println(Bluetooth.switches[1][1]);
        if(Bluetooth.switches[1][1]==0){ //x2 open?
          Serial.println("switch open x2...");
          //changing number on touch screen ************************************
          Bluetooth.axisNum[1]=Bluetooth.axisNum[1]-Bluetooth.step;
          Bluetooth.axisStr[1]=String(Bluetooth.axisNum[1]);
          Bluetooth.axisStr[1].replace(".","0");               
          SendToDisplay.dataPosition(2,Bluetooth.axisStr[1]);
          //Any change in switche state? ***************************************
          SendToDisplay.activeDeactiveTouch("lessX", true);
          if(Bluetooth.switches[1][0]==0){ //x1 open?
            SendToDisplay.endOfWheel(0,2);  //x1 and x2 opened, motor2
            SendToDisplay.activeDeactiveTouch("plusX", true); //button x1 activated
          }else{ //x1 closed
            Serial.println("switch close plusX X1");
            SendToDisplay.endOfWheel(3,2); //x1 closed and x2 open, motor2
            SendToDisplay.activeDeactiveTouch("plusX", false); //button x1 blocked
          }
        }else{ //x2 closed
          Serial.println("switch close lessX X2");
          if(Bluetooth.switches[1][0]==0){ //x1 open?
            SendToDisplay.endOfWheel(4,2); //x1 open and x2 closed, motor2
            SendToDisplay.activeDeactiveTouch("lessX", false); //button x2 blocked
          }else{ //x1 closed
            //x1 and x2 closed, motor2 -----ERROR
            Serial.println("switch close x1 and x2"); //shouldn't happen
          }
        }
      }else{//Comunication error
        Serial.println("BLUETOOTH COMUNICATION ERROR: BACKWARD"); 
      }
    break;

    case 5: //LEFT:y1:switch1:motor3    switches[2][0]=y1         switches[2][1]=y2
      Box.y1();
      if(Bluetooth.endOfpinWheel()==true){
        Serial.print("Y1: ");
        Serial.println(Bluetooth.switches[2][0]);
        Serial.print("Y2: ");
        Serial.println(Bluetooth.switches[2][1]);
        if(Bluetooth.switches[2][0]==0){ //y1 open?
          Serial.println("switch open y1...");
          //changing number on touch screen ************************************
          Bluetooth.axisNum[2]=Bluetooth.axisNum[2]+Bluetooth.step;
          Bluetooth.axisStr[2]=String(Bluetooth.axisNum[2]);
          Bluetooth.axisStr[2].replace(".","0");               
          SendToDisplay.dataPosition(3,Bluetooth.axisStr[2]);
          //Any change in switche state? ***************************************
          SendToDisplay.activeDeactiveTouch("plusY", true);
          if(Bluetooth.switches[2][1]==0){ //y2 open?
            SendToDisplay.endOfWheel(0,3); //y1 and y2 opened, motor3
            SendToDisplay.activeDeactiveTouch("lessY", true); //button y2 activated
          }else{ //y2 closed
            Serial.println("switch close lessY Y2");
            SendToDisplay.endOfWheel(6,3); //y1 opened and y2 closed, motor3
            SendToDisplay.activeDeactiveTouch("lessY", false); //button y2 blocked
          }
        }else{ //y1 closed
          Serial.println("switch close plusY Y1");
          if(Bluetooth.switches[2][1]==0){ //y2 open?
            SendToDisplay.endOfWheel(5,3); //y1 closed and y2 open, motor3
            SendToDisplay.activeDeactiveTouch("plusY", false); //button y1 blocked
          }else{ //y2 closed
            //y1 and y2 closed, motor3 -----ERROR
            Serial.println("switch close y1 and y2"); //shouldn't happen
          }
        }
      }else{ //Comunication error
        Serial.println("BLUETOOTH COMUNICATION ERROR: LEFT");
      }
    break;        

    case 6: //RIGHT:y2:switch2:motor3    switches[2][0]=y1         switches[2][1]=y2
      Box.y2();
      if(Bluetooth.endOfpinWheel()==true){
        Serial.print("Y1: ");
        Serial.println(Bluetooth.switches[2][0]);
        Serial.print("Y2: ");
        Serial.println(Bluetooth.switches[2][1]);
        if(Bluetooth.switches[2][1]==0){ //y2 open?
          Serial.println("switch open y2...");
          //changing number on touch screen ************************************
          Bluetooth.axisNum[2]=Bluetooth.axisNum[2]-Bluetooth.step;
          Bluetooth.axisStr[2]=String(Bluetooth.axisNum[2]);
          Bluetooth.axisStr[2].replace(".","0");               
          SendToDisplay.dataPosition(3,Bluetooth.axisStr[2]);
          //Any change in switche state? ***************************************
          SendToDisplay.activeDeactiveTouch("lessY", true);
          if(Bluetooth.switches[2][0]==0){ //y1 open?
            SendToDisplay.endOfWheel(0,3);  //y1 and y2 opened, motor3
            SendToDisplay.activeDeactiveTouch("plusY", true); //button y1 activated
          }else{ //y1 closed
            Serial.println("switch close plusY Y1");
            SendToDisplay.endOfWheel(5,3); //y1 closed and y2 open, motor3
            SendToDisplay.activeDeactiveTouch("plusY", false); //button y1 blocked
          }
        }else{ //y2 closed
          Serial.println("switch close lessY Y2");
          if(Bluetooth.switches[2][0]==0){ //y1 open?
            SendToDisplay.endOfWheel(6,3); //y1 open and y2 closed, motor3
            SendToDisplay.activeDeactiveTouch("lessY", false); //button y2 blocked
          }else{ //y1 closed
            //y1 and y2 closed, motor3 -----ERROR
            Serial.println("switch close y1 and y2"); //shouldn't happen
          }
        }
      }else{ //Comunication error
        Serial.println("BLUETOOTH COMUNICATION ERROR: RIGHT");  
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