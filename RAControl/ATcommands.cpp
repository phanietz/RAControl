#include "ATcommands.h"
#include "SendtoDisplay.h"
#define ENABLE 9
#define SIGNALink 53
#define TIME_LINES 100

AT::AT(){};

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
String AT::inqm(String param){
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
  }while(responseAT.compareTo("OK\r\n")!=0); //if 0 means equal
  
  responseAT=responseAT.substring(0, responseAT.length()-2); //remove \r\n from command string  
  return String(Serial.println(responseAT));
};

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
String AT::reset(){
  digitalWrite(ENABLE, LOW);
  delay(500);
  digitalWrite(ENABLE, HIGH);
  delay(500);
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
  }while(responseAT.compareTo("OK\r\n")!=0); //if 0 means equal
  
  responseAT=responseAT.substring(0, responseAT.length()-2); //remove \r\n from command string    
  return String(Serial.println(responseAT));
};

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
String AT::init(){
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
  }while(responseAT.compareTo("OK\r\n")!=0); //if 0 means equal

  responseAT=responseAT.substring(0, responseAT.length()-2); //remove \r\n from command string    
  return String(Serial.println(responseAT));
};

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
String AT::inq(bool mode, class HMI &Dis){
  Serial.println("AT+INQ"); 
  Serial3.write("AT+INQ\r\n");
  Serial3.flush();
  delay(500);
  text="";
  countCach=0;
  refreshCount=0;
  if(mode==true){
    Serial.println("________________________________");
    Serial.println("looking for ON devices ...");
    Serial.println("________________________________");
  }
  do{        
    responseAT="";
    auxEndMarker="";
    while(Serial3.available()>0){
      a = char(Serial3.read());
      responseAT += a;
      delay(50);
    }
    
    if(responseAT.length()==0){
      cycle = cycle + 1; //for the machine cycles
    }else{
      text += responseAT;   //concat data received
      auxEndMarker=text.substring(text.length()-5,text.length());
      if(mode==true){
        countCach = countCach + 1;
        Serial.print(countCach);
        Serial.println("  *****************************");
        Serial.println(text);
      }
    }
    if(responseAT.compareTo("OK\r\n")==0){ //when detect OK

      Dis.progressBar(50);

      refreshCount++;
      if(auxEndMarker.compareTo("\nOK\r\n")!=0){ //verify that the sentence of the structure is correct
        if(refreshCount>=2){
          if(mode==true){
            Serial.println("****Maximum search cycle time was reached****");
            //ERROR 3
            //error=3; //It did not programed
          }
          break; //finish search for bluetooth issue
        }else{
          if(mode==true){
            Serial.println("****EndMarker no corresponde, Refresh again****");
            Serial.println(text); //show data received
            Serial.println(auxEndMarker);
            Serial.println(refreshCount);
          }
          text="";  
          countCach=0;      
          Serial3.write("AT+INQ\r\n");
          Serial3.flush();
          delay(500);
        }
      }else{//finish search with the correct data
        if(mode==true){
          Serial.println("END ____________________________");
          Serial.print("Total machine cycles null: ");
          Serial.println(cycle);
          Serial.print("Total bytes: ");
          Serial.println(text.length());
        }
        Dis.progressBar(75);   
      }
    }
  }while(auxEndMarker.compareTo("\nOK\r\n")!=0); //if is 4 means only read OK and not full data
  
  text.trim();
  if(mode==true){
    Serial.println("________________________________");
    Serial.println("manipular Y GUARDAR datos EN ARRAY ...");    
    Serial.println(text);
    Serial.println("________________________________");
    Serial.print("On devices: ");
  }
  text=text.substring(0,text.length()-4); //remove "\nOK\r\n"
  
  int i=0, j=0, on=0;
  for(i=0; i<5; i++){
    index = text.indexOf(SystemsON[i][0]);
    if(index==-1){
      SystemsON[i][1]="-OFF";
    }else{
      SystemsON[i][1]="-ON";
      on=on+1;
    }
  };

  if(mode==true){
    Serial.println(on);
  }

  if(on==0){
    //ALL systems are OFF
    error=2;
  }else{
    Dis.progressBar(100);
    error=0; //At least one system ON
  }

  responseAT=responseAT.substring(0, responseAT.length()-2); //remove \r\n from command string

  return String(Serial.println(responseAT));
};

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
String AT::link(int cell){
  Serial.println("AT+LINK");

  switch(cell){
    case 201:
      Serial3.write("AT+LINK=14,3,50974\r\n");    
      break;

    case 202:
      Serial3.write("AT+LINK=14,3,508D1\r\n");    
      break;

    case 203:
      Serial3.write("AT+LINK=2017,7,251406\r\n");  
      break;

    case 204:
      Serial3.write("AT+LINK=14,7,518D1\r\n");  
      break;

    case 205:
      Serial3.write("AT+LINK=2016,7,251806\r\n");  
      break;                  
  }
  
  findingCopy=0;
  Serial3.flush();
  do{
    responseAT="";
    delay(100);
    while(Serial3.available()>0){
      a = char(Serial3.read());
      responseAT += a;
    }
    //Serial.println(responseAT.length());
    //Serial.println(responseAT);
    //Serial3.write("Master conected\r\n");
    findingCopy++;
    if(findingCopy >= TIME_LINES){
      Serial.println("long time trying to link");
      error=4; 
      return "errorLINK OK";
    }    
  }while(responseAT.indexOf("OK")==-1); //if its >= 0 means "OK" was received

  error=0;
  Serial.print("Line cycles: ");
  Serial.println(findingCopy);

  return String(responseAT);
};

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
String AT::disc(){
  Serial.println("AT+DISC");
  digitalWrite(ENABLE, LOW);
  delay(500);
  digitalWrite(ENABLE, HIGH);
  delay(500);
  Serial3.write("AT+DISC\r\n");
  Serial3.flush();

  int aux=0;
  do{
    responseAT="";
    delay(200);
    while(Serial3.available()>0){
      a = char(Serial3.read());
      responseAT += a;
    }    
    //Serial.println(responseAT.length());
    Serial.println(responseAT);
  }while(responseAT.indexOf("OK")==-1); //if its >= 0 means "OK" was received
  
  responseAT=responseAT.substring(0, responseAT.length()-2); //remove \r\n from command string  
  return String(Serial.println(responseAT));  
};

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
bool AT::axisReceiving(){
  findingCopy=0;
  //time = millis();
  do{
    responseAT="";
    delay(100);
    while(Serial3.available()>0){
      a = char(Serial3.read());
      responseAT += a;    
    }

    findingCopy++;
    if(findingCopy >= TIME_LINES){
      Serial.println("Time_lines exceded");
      error=1;
      return false;
    }

  }while(responseAT.indexOf("COPY\r\n")==-1); //if 0 means equal

  Serial.print("Lyne cycles: ");
  Serial.println(findingCopy);
  Serial.println(responseAT);

  if(responseAT.indexOf("{[")!=-1 and responseAT.indexOf("]}")!=-1){
    for(i=0; i<3; i++){
      if(i==0){
        data=responseAT.substring(responseAT.indexOf("{")+1,responseAT.indexOf("}"));
      }else{
        data=data.substring(data.indexOf("][")+1,data.length());
      }
      axisStr[i]=data.substring(data.indexOf("[")+1,data.indexOf("]"));
      axisNum[i]=axisStr[i].toFloat();
      axisStr[i].replace(".","0");
    }
    error=0;
    return true;
  }else{
    error=3;
    return false;
  }
};

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
String AT::waitData(int type){
  findingCopy=0;
  //time = millis();
  do{
    responseAT="";
    delay(100);
    while(Serial3.available()>0){
      a = char(Serial3.read());
      responseAT += a;    
    }
    //Serial.println(responseAT.length());
    //Serial.println(responseAT);
    findingCopy++;
    
    if(findingCopy >= TIME_LINES){         ////////////////looooooook and change
      Serial.println("entra false");
      responseAT = "";
      responseAT += "COPY\r\n";
    }

  }while(responseAT.indexOf("COPY\r\n")==-1); //if 0 means equal

  Serial.print("Line cycles: ");
  Serial.println(findingCopy);

  if(type==2){
    data=responseAT.substring(responseAT.indexOf(".")-2,responseAT.indexOf(".")+3);
    Serial.println(data);
    data.replace(".","0");
    Serial.println(data);    
  }

  return data;
};

bool AT::waitCopy(){
  findingCopy=0;
  do{
    responseAT="";
    delay(100);
    while(Serial3.available()>0){
      a = char(Serial3.read());
      responseAT += a;
    }
    //Serial.println(responseAT.length());
    //Serial.println(responseAT);
    findingCopy++;
    
    if(findingCopy >= 500){         ////////////////looooooook and change
      Serial.println("entra false");
      return true;
    }
  }while(responseAT.indexOf("COPY\r\n")==-1); //if 0 means equal

  return true;
};