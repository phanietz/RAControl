//FROM ARDUINO TO NEXTION:
//https://www.youtube.com/watch?v=mEbGVZJjKbs

//FROM NEXTION TO ARDUINO:
//https://www.youtube.com/watch?v=sNe-lkVQhqI

#include "ATcommands.h"
#define ENABLE 9
#define SIGNALink 53

AT::AT(){};

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
String AT::INQM(String param){
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
  return String(Serial.println(responseAT));
};

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
String AT::RESET(){
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
  return String(Serial.println(responseAT));
};

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
String AT::INIT(){
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
  return String(Serial.println(responseAT));
};

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
String AT::INQ(bool mode){
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
      //Serial3.flush();
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
      Serial2.print("ProgressBar.val=50"); //progress bar: Refresh
      Serial2.write(0xff);
      Serial2.write(0xff);
      Serial2.write(0xff);
      refreshCount++;
      if(auxEndMarker.compareTo("\nOK\r\n")!=0){ //verify that the sentence of the structure is correct
        if(refreshCount>=2){
          if(mode==true){
            Serial.println("****Maximum search cycle time was reached****");
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
        Serial2.print("ProgressBar.val=75"); //progress bar: Refresh
        Serial2.write(0xff);
        Serial2.write(0xff);
        Serial2.write(0xff);        
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

  responseAT=responseAT.substring(0, responseAT.length()-2); //remove \r\n from command string
  Serial.println(responseAT);

  if(on==0){
    //No ON systems
    Serial2.print("p0.pic=51");
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);   
    Serial2.print("tsw m0,1");
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);  
    Serial2.print("vis ProgressBar,0");  
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);      
    Serial2.print("vis p1,1");  
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.print("vis Menu,1");  
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);    
    Serial2.print("error=2");  
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff); 
    error=2;    
    responseAT="No";
    
  }else{
    ////////////////////================================= Send to Display: page SPASOn
    Serial2.print("ProgressBar.val=100");
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff); 
    responseAT="Yes";
  }

  return responseAT;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
String AT::LINK(int cell){
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
  
  Serial3.flush();
  do{
    responseAT="";
    delay(100);
    while(Serial3.available()>0){
      a = char(Serial3.read());
      responseAT += a;
    }
      //Serial doesn't write after conection
      /*
      Serial.println(responseAT.length());
      Serial.println(responseAT);
      Serial3.write("Master conected\r\n");
      */

  }while(responseAT.indexOf("OK")!=-1); //4 para OK


  while(digitalRead(SIGNALink)==false){
  }
  
  //responseAT=responseAT.substring(0, responseAT.length()-2); //remove \r\n from command string
  return String(Serial.println("OK"));

};

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
String AT::DISC(){
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
    /*if(responseAT.length()!=0){
      Serial.println("------");
      Serial.println(responseAT.length());
      Serial.println(responseAT);
      Serial.println(aux);
      Serial.println("------");
      Serial.println(responseAT.indexOf("OK"));
    }*/
    
  }while(responseAT.indexOf("OK")==0); //4 para OK
  responseAT=responseAT.substring(0, responseAT.length()-2); //remove \r\n from command string  
  return String(Serial.println(responseAT));  
};

