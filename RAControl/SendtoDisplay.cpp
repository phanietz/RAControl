#include "SendtoDisplay.h"
#include "ATcommands.h"
#include "SendtoBox.h"

AT Bluetooth;
Box tobox;

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
  Serial2.print("ProgressBar.val=0"); //progress bar: Refresh
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);
  ////////////////////================================= Send to Blue: AT+RESET
  responseAT=Bluetooth.RESET();

  ////////////////////================================= Send to Display:
  Serial2.print("ProgressBar.val=25"); //progress bar: Refresh
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);   

  ////////////////////================================= Send to Blue: AT+INIT
  responseAT=Bluetooth.INIT();

  ////////////////////================================= Send to Blue: AT+INQ
  responseAT=Bluetooth.INQ(false); //progress 50, 75 and 100
  
  if(responseAT.compareTo("Yes")==0){
    SendSystemsAvailbles(true);
  }
  
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
      Serial2.print("tsw t20"+String(i+1)+",1");  
      Serial2.write(0xff);
      Serial2.write(0xff);
      Serial2.write(0xff);    
    }else{
      Serial2.flush();
      Serial2.print("vis s20"+String(i+1)+"on,0");
      Serial2.write(0xff);
      Serial2.write(0xff);
      Serial2.write(0xff);
      Serial2.print("tsw t20"+String(i+1)+",0");
      Serial2.write(0xff);
      Serial2.write(0xff);
      Serial2.write(0xff);    
    }
  }

  //activar boton refresh
  Serial2.flush();
  Serial2.print("tsw m0,1");
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
  Serial2.flush();
  Serial2.print("page SPASConDis");
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);

  //responseAT=Bluetooth.RESET();

  //responseAT=Bluetooth.INIT();

  //responseAT=Bluetooth.INQ(false);

  responseAT=Bluetooth.LINK(cell);

  Serial2.flush();
  Serial2.print("page SPASmotor1");
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);

  tobox.motor1();
};

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
void Display::disconnect(){

  Serial.println("------DESCONECTAR-----");
  Serial2.flush();
  Serial2.print("page SPASConDis");
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);  

  Serial2.print("ProgressBar.val=25");  
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);   

  ////////////////////================================= AT+DISC 
  responseAT=Bluetooth.DISC();

  Serial2.print("ProgressBar.val=50");  
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);   

  Serial2.print("ProgressBar.val=75");  
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);  

  ////////////////////================================= AT+RESET
  responseAT=Bluetooth.RESET();

  Serial2.print("ProgressBar.val=100");  
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);   

  Serial2.print("page SPASOn");
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);  
};

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
void Display::menu(int state){

  if(state==0){
    Serial2.print("page SPASsearching");  
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);   
    Serial2.print("tsw m0,1");  
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff); 

    if(Bluetooth.error=2){
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
    }
  }
}