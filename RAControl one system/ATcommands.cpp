//FROM ARDUINO TO NEXTION:
//https://www.youtube.com/watch?v=mEbGVZJjKbs

//FROM NEXTION TO ARDUINO:
//https://www.youtube.com/watch?v=sNe-lkVQhqI

#include "ATcommands.h"
#define ENABLE 9

AT::AT(){};

void AT::config(){
  ////////////////////================================= Send to Blue: AT+INQM=1,9,8
  Serial.println("AT+INQM=1,9,8"); 
  do{
    responseAT="";
    Serial3.write("AT+INQM=1,9,8\r\n");
    Serial3.flush();
    delay(100);
    while(Serial3.available()>0){
      a = char(Serial3.read());
      responseAT += a;
    }
    //Serial.println(responseAT.length());
  }while(responseAT.compareTo("OK\r\n")!=0); //4 para OK
  responseAT=responseAT.substring(0, responseAT.length()-2); //remove \r\n from command string  
  Serial.println(responseAT);
  responseAT="";

  ////////////////////================================= Send to Blue: AT+RESET
  Serial.println("AT+RESET"); 
  do{
    responseAT="";
    Serial3.write("AT+RESET\r\n");
    Serial3.flush();
    delay(200);
    while(Serial3.available()>0){
      a = char(Serial3.read());
      responseAT += a; 
    }
    //Serial.println(responseAT.length());
  }while(responseAT.compareTo("OK\r\n")!=0); //4 para OK
  responseAT=responseAT.substring(0, responseAT.length()-2); //remove \r\n from command string  
  Serial.println(responseAT);
  responseAT="";

};


void AT::refresh(){
  Serial.println("------REFRESH-----");
  ////////////////////================================= Send to Display: page Refreshing
  Serial2.print("page Refreshing");
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.print("j0.val=0");
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);
  ////////////////////================================= Send to Blue: AT+INIT
  Serial.println("AT+INIT");
  do{
    responseAT="";
    Serial3.write("AT+INIT\r\n");
    Serial3.flush();
    delay(500);
    while(Serial3.available()>0){
      a = char(Serial3.read());
      responseAT += a;    
    }
    //Serial.println(responseAT.length());
    if(responseAT.length()==12){
      //Serial.println(responseAT);
      responseAT="OK\r\n";
    }
  }while(responseAT.compareTo("OK\r\n")!=0); //4 para OK
  responseAT=responseAT.substring(0, responseAT.length()-2); //remove \r\n from command string  
  Serial.println(responseAT);
  responseAT="";

  Serial2.print("j0.val=25");
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);
  ////////////////////================================= Send to Blue: AT+INQ
  Serial.println("AT+INQ"); 
  Serial3.write("AT+INQ\r\n");
  Serial3.flush();
  delay(500);
  text="";
  refreshCount=0;
  Serial.println("looking for ON devices ...");
  do{        
    responseAT="";
    auxEndMarker="";
    while(Serial3.available()>0){
      a = char(Serial3.read());
      responseAT += a;
      //Serial3.flush();
      delay(50);
    }
    
    if(responseAT.length()==0){
      cycle = cycle + 1; //for the machine cycles
    }else{
      text += responseAT;   //concat data received
      auxEndMarker=text.substring(text.length()-5,text.length());
      Serial.println("**************************************************");
      Serial.println(text);
    }
    if(responseAT.compareTo("OK\r\n")==0){ //when detect OK
      Serial.println("entra ok");
      Serial2.print("j0.val=50");
      Serial2.write(0xff);
      Serial2.write(0xff);
      Serial2.write(0xff);
      refreshCount++;
      if(auxEndMarker.compareTo("\nOK\r\n")!=0){ //verify if OK is before and after an \n
        if(refreshCount>=3){
          Serial.println("****Maximum search cycle time was reached****");
          Serial2.print("page No systems");
          Serial2.write(0xff);
          Serial2.write(0xff);
          Serial2.write(0xff);
          break; //finish search for bluetooth issue
        }else{
          Serial.println("****EndMarker no corresponde, Refresh again****");
          Serial.println(text); //show data received
          Serial.println(auxEndMarker);
          Serial.println(refreshCount);
          text="";        
          Serial3.write("AT+INQ\r\n");
          Serial3.flush();
          delay(500);
        }
      }else{//finish search with the correct data
        //Serial.println(cycle);
        //Serial.println(text.length());
        Serial2.print("j0.val=75");
        Serial2.write(0xff);
        Serial2.write(0xff);
        Serial2.write(0xff);        
      }
    }
  }while(auxEndMarker.compareTo("\nOK\r\n")!=0); //if is 4 means only read OK and not full data
  

  Serial.println("*********manipular Y GUARDAR datos EN ARRAY**********");
  text.trim();
  Serial.println(text);
  Serial.println("On devices:");
  text=text.substring(0,text.length()-4); //remove "\nOK\r\n"
  
  int i=0, j=0;
  for(i=0; i<5; i++){
    index = text.indexOf(SystemsON[i][0]);
    if(index==-1){
      SystemsON[i][1]="-OFF";
    }else{
      SystemsON[i][1]="-ON";
    }
  };


  ////////////////////================================= Send to Display: page Select System
  Serial2.print("j0.val=100");
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);  
  Serial2.print("page Select System");
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);
  SendSystemsAvailbles();

};

void AT::SendSystemsAvailbles(){
  Serial.println("*********MOSTRAR EN EL DISPLAY LOS SISTEMAS PRENDIDOS**********");
  Serial.print(SystemsON[0][0]);
  Serial.print(SystemsON[0][1]);
  Serial.println(SystemsON[0][2]);
  if(SystemsON[0][1].compareTo("-ON")==0){
    Serial2.flush();
    Serial2.print("vis s201,1");
    //Serial2.print("vis m1,1");
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);
  }else{
    Serial2.flush();
    Serial2.print("vis s201,0");
    //Serial2.print("vis m1,0");
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);
  }


  Serial.print(SystemsON[1][0]);
  Serial.print(SystemsON[1][1]);
  Serial.println(SystemsON[1][2]);
  if(SystemsON[1][1].compareTo("-ON")==0){
    Serial2.flush();
    Serial2.print("vis s202,1");
    //Serial2.print("vis m2,1");
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);
  }else{
    Serial2.flush();
    Serial2.print("vis s202,0");
    //Serial2.print("vis m2,0");
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);
  }


  Serial.print(SystemsON[2][0]);
  Serial.print(SystemsON[2][1]);
  Serial.println(SystemsON[2][2]);
  if(SystemsON[2][1].compareTo("-ON")==0){
    Serial2.flush();
    Serial2.print("vis s203,1");
    //Serial2.print("vis m3,1");
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);
  }else{
    Serial2.flush();
    Serial2.print("vis s203,0");
    //Serial2.print("vis m3,0");
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);
  }


  Serial.print(SystemsON[3][0]);
  Serial.print(SystemsON[3][1]);
  Serial.println(SystemsON[3][2]);  
  if(SystemsON[3][1].compareTo("-ON")==0){
    Serial2.flush();
    Serial2.print("vis s204,1");
    //Serial2.print("vis m4,1");
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);
  }else{
    Serial2.flush();
    Serial2.print("vis s204,0");
    //Serial2.print("vis m4,0");
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);
  }


  Serial.print(SystemsON[4][0]);
  Serial.print(SystemsON[4][1]);
  Serial.println(SystemsON[4][2]);  
  if(SystemsON[4][1].compareTo("-ON")==0){
    Serial2.flush();
    Serial2.print("vis s205,1");
    //Serial2.print("vis m5,1");
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);
  }else{
    Serial2.flush();
    Serial2.print("vis s205,0");
    //Serial2.print("vis m5,0");
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);
  }

  //limpiar buffer
  while(Serial3.available()>0){
    a = char(Serial3.read());
  }

}

void AT::connect(int cell){
  ////////////////////================================= AT+LINK=14,3,50974
  Serial.println("------CONECTAR-----");
  Serial3.write("AT+RESET\r\n");
  Serial3.flush();
  delay(1000);
  Serial3.write("AT+INIT\r\n");
  Serial3.flush();
  delay(1000);
  Serial3.write("AT+INQ\r\n");
  Serial3.flush();
  delay(1000);
  if(cell==201){
    Serial.println(cell);
    Serial3.write("AT+LINK=14,3,50974\r\n");
    Serial3.flush();
  }
  if(cell==202){
    Serial.println(cell);
    Serial3.write("AT+LINK=14,3,508D1\r\n");
    Serial3.flush();
  }  
  if(cell==205){
    Serial.println(cell);
    Serial3.write("AT+LINK=2016,7,251806\r\n");
    Serial3.flush();
  }  
};

void AT::disconnect(){
  ////////////////////================================= AT+DISC
  Serial.println("------DESCONECTAR-----");
  digitalWrite(ENABLE, LOW);
  delay(500);
  digitalWrite(ENABLE, HIGH);
  delay(500);

  Serial3.write("AT+DISC\r\n");
  Serial3.flush();
  delay(500);

  ////////////////////================================= AT+RESET
  Serial.println("AT+RESET"); 
  
  do{
    responseAT="";
    do{    
      Serial3.write("AT+RESET\r\n");
      Serial3.flush();
      while(Serial3.available()){
        if(Serial3.available()>0){
          a = char(Serial3.read());
          responseAT += a;    
        }
      }
    }while(responseAT.length()==0); //4 para OK
    Serial.println(responseAT.length());
    delay(1000);
  }while(responseAT.compareTo("OK\r\n")!=0); //4 para OK
  responseAT=responseAT.substring(0, responseAT.length()-2); //remove \r\n from command string  
  Serial.println(responseAT);
  responseAT="";
};



void AT::receive(){};

void AT::send(String cell){};