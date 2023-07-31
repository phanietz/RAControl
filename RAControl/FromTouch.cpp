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
    connected=Bluetooth.axisReceiving(); //receive axis from box and then COPY
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
      SendToDisplay.firstDataUpdate(Bluetooth.axisStr[0], Bluetooth.axisStr[1], Bluetooth.axisStr[2]);
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
    case 1:
            aux2=Bluetooth.axisNum[1]-Bluetooth.step;
            Serial.println(aux2);
            if(aux2<0){
              Serial.println("es menor a 0");
            }else{
              Box.x1();
              Bluetooth.axisNum[1]=Bluetooth.axisNum[1]+Bluetooth.step;
              Bluetooth.axisStr[1]=String(Bluetooth.axisNum[1]);
              Bluetooth.axisStr[1].replace(".","0");               
              SendToDisplay.dataPosition(2,Bluetooth.axisStr[1]);
              Serial.println(Bluetooth.axisNum[1]);
            }

    break;

    case 2:
            aux2=Bluetooth.axisNum[1]-Bluetooth.step;
            Serial.println(aux2);
            if(aux2<0){
              Serial.println("es menor a 0");
            }else{
              Box.x2();
              Bluetooth.axisNum[1]=Bluetooth.axisNum[1]-Bluetooth.step;
              Bluetooth.axisStr[1]=String(Bluetooth.axisNum[1]);
              Bluetooth.axisStr[1].replace(".","0");               
              SendToDisplay.dataPosition(2,Bluetooth.axisStr[1]);
              Serial.println(Bluetooth.axisNum[1]);
            }
    break;

    case 3:
            aux2=Bluetooth.axisNum[2]-Bluetooth.step;
            Serial.println(aux2);
            if(aux2<0){
              Serial.println("es menor a 0");
            }else{            
              Box.y1();
              Bluetooth.axisNum[2]=Bluetooth.axisNum[2]+Bluetooth.step;
              Bluetooth.axisStr[2]=String(Bluetooth.axisNum[2]);
              Bluetooth.axisStr[2].replace(".","0");               
              SendToDisplay.dataPosition(3,Bluetooth.axisStr[2]);
              Serial.println(Bluetooth.axisNum[2]);
            }
    break;        

    case 4:
            aux2=Bluetooth.axisNum[2]-Bluetooth.step;
            Serial.println(aux2);
            if(aux2<0){
              Serial.println("es menor a 0");
            }else{            
              Box.y2();
              Bluetooth.axisNum[2]=Bluetooth.axisNum[2]-Bluetooth.step;
              Bluetooth.axisStr[2]=String(Bluetooth.axisNum[2]);
              Bluetooth.axisStr[2].replace(".","0");               
              SendToDisplay.dataPosition(3,Bluetooth.axisStr[2]);
              Serial.println(Bluetooth.axisNum[2]);
            }
    break;    

    case 5:
            aux2=Bluetooth.axisNum[0]-Bluetooth.step;
            Serial.println(aux2);
            if(aux2<0){
              Serial.println("es menor a 0");
            }else{            
              Box.z1();
              Bluetooth.axisNum[0]=Bluetooth.axisNum[0]+Bluetooth.step;
              Bluetooth.axisStr[0]=String(Bluetooth.axisNum[0]);
              Bluetooth.axisStr[0].replace(".","0");           
              SendToDisplay.dataPosition(1,Bluetooth.axisStr[0]);
              Serial.println(Bluetooth.axisNum[0]);
            }
    break;    

    case 6:
            aux2=Bluetooth.axisNum[0]-Bluetooth.step;
            Serial.println(aux2);
            if(aux2<0){
              Serial.println("es menor a 0");
            }else{            
              Box.z2();
              Bluetooth.axisNum[0]=Bluetooth.axisNum[0]-Bluetooth.step;
              Bluetooth.axisStr[0]=String(Bluetooth.axisNum[0]);
              Bluetooth.axisStr[0].replace(".","0");
              SendToDisplay.dataPosition(1,Bluetooth.axisStr[0]);
              Serial.println(Bluetooth.axisNum[0]);
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
      SendToDisplay.picture("68");
    }else if(motorIndex==2){
      SendToDisplay.picture("73");
    }else if(motorIndex==3){
      SendToDisplay.picture("78");
    }    
    SendToDisplay.progressBar(30);
    Box.reboot();

    //wait position 0
    do{
      exit=Bluetooth.waitCopy();
    }while(exit==false);


    SendToDisplay.progressBar(60);
    if(motorIndex==1){
      SendToDisplay.picture("69");
    }else if(motorIndex==2){
      SendToDisplay.picture("74");
    }else if(motorIndex==3){
      SendToDisplay.picture("79");
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