#include "SendtoDisplay.h"
#include "ATcommands.h"


AT Bluetooth;

Display::Display(){};

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
void Display::start(){
  ////////////////////================================= Send to Blue: AT+INQM=1,9,8
  responseAT=Bluetooth.INQM("1,9,8");

  ////////////////////================================= Send to Blue: AT+RESET
  responseAT=Bluetooth.RESET();

};

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
void Display::refresh(){
  Serial.println("------REFRESH-----");
  ////////////////////================================= Send to Display: 
  Serial2.print("page SPASsearching");
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.print("ProgressBar.val=0");
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);
  ////////////////////================================= Send to Blue: AT+RESET
  responseAT=Bluetooth.RESET();

  ////////////////////================================= Send to Display:
  Serial2.print("ProgressBar.val=25");
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);   

  ////////////////////================================= Send to Blue: AT+INIT
  responseAT=Bluetooth.INIT();

  ////////////////////================================= Send to Blue: AT+INQ
  responseAT=Bluetooth.INQ(false);

  SendSystemsAvailbles(true);
};

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
void Display::SendSystemsAvailbles(bool mode){
  Serial2.print("page SPASOn");
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff); 

  for(i=0; i<5; i++){
    if(mode==true){      
      Serial.print(Bluetooth.SystemsON[i][0]);
      Serial.print(Bluetooth.SystemsON[i][1]);
      Serial.println(Bluetooth.SystemsON[i][2]);
    }
    if(Bluetooth.SystemsON[i][1].compareTo("-ON")==0){
      Serial2.flush();
      
      Serial2.print("vis s20"+String(i+1)+"on,1");
      Serial2.write(0xff);
      Serial2.write(0xff);
      Serial2.write(0xff);
      Serial2.print("vis t20"+String(i+1)+",1");  
      Serial2.write(0xff);
      Serial2.write(0xff);
      Serial2.write(0xff);    
    }else{
      Serial2.flush();
      Serial2.print("vis s20"+String(i+1)+"on,0");
      Serial2.write(0xff);
      Serial2.write(0xff);
      Serial2.write(0xff);
      Serial2.print("vis t20"+String(i+1)+",0");
      Serial2.write(0xff);
      Serial2.write(0xff);
      Serial2.write(0xff);    
    }
  }

  //activar boton refresh
  Serial2.flush();
  Serial2.print("vis m0,1");
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);

}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
void Display::connect(int cell){
  ////////////////////================================= AT+LINK=XX,X,XXXXX
  Serial.println("------CONECTAR-----");
  responseAT=Bluetooth.RESET();

  responseAT=Bluetooth.INIT();

  responseAT=Bluetooth.INQ(false);

  responseAT=Bluetooth.LINK(cell);

};

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
void Display::disconnect(){

  Serial.println("------DESCONECTAR-----");
  Serial2.print("j0.val=25");  
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);   

  ////////////////////================================= AT+DISC 
  responseAT=Bluetooth.DISC();

  Serial2.print("j0.val=50");  
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);   

  Serial2.print("j0.val=75");  
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);  

  ////////////////////================================= AT+RESET
  responseAT=Bluetooth.RESET();

  Serial2.print("j0.val=100");  
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);   
};
