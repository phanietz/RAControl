#include "SendtoDisplay.h"
#include "ATcommands.h"

Display::Display(){};

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
void Display::refresh(){
  
  Serial2.print("page SPASsearching");
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);
  
};

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
void Display::SendSystemsAvailables(bool mode, class AT &Blue){
  //AT Blue;
  Serial2.print("page SPASOn");
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff); 

  for(i=0; i<5; i++){
    if(mode==true){      
      Serial.print(Blue.SystemsON[i][0]);
      Serial.print(Blue.SystemsON[i][1]);
      Serial.println(Blue.SystemsON[i][2]);
    }
    if(Blue.SystemsON[i][1].compareTo("-ON")==0){
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

};

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
void Display::connect(int send){
  ////////////////////================================= AT+LINK=XX,X,XXXXX
  
  if(send == 1){
    Serial2.flush();
    Serial2.print("page SPASConDis");
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);
  }
  if(send == 2){
    Serial2.flush();
    Serial2.print("page SPASmotor1");
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);
  }
};

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
void Display::disconnect(int send){

  if(send == 1){
    Serial2.flush();
    Serial2.print("page SPASConDis");
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);  
  }

  if(send == 2){
    Serial2.print("page SPASOn");
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);  
  }
};

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
void Display::ProgressBar(int value){
  switch(value){
    case 0:
      Serial2.print("ProgressBar.val=0");  
    break;
    case 10:
      Serial2.print("ProgressBar.val=10");
    break;   
    case 25:
      Serial2.print("ProgressBar.val=25");
    break;     
    case 35:
      Serial2.print("ProgressBar.val=35");
    break;
    case 50:
      Serial2.print("ProgressBar.val=50"); 
    break;
    case 70:
      Serial2.print("ProgressBar.val=70"); 
    break;
    case 75:
      Serial2.print("ProgressBar.val=75");
    break;
    case 100:
      Serial2.print("ProgressBar.val=100"); 
    break;
  }

  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);

};

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
void Display::menu(int state, class AT &Blue){

  if(state==0){
    Serial2.print("page SPASsearching");  
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);   
    Serial2.print("tsw m0,1");  
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff); 

    if(Blue.error=2){
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
};
