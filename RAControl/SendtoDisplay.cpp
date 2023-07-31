#include "SendtoDisplay.h"
#include "ATcommands.h"

HMI::HMI(){};

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
void HMI::refresh(){
  
  Serial2.print("page SPASsearching");
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);
  
};

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
void HMI::sendSystemsAvailables(bool mode, class AT &Blue, int error){
  //AT Blue;
  Serial2.print("page SPASOn");
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff); 

  if(error==1 or error==3 or error==4){
    Serial2.print("p0.pic=49");
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff); 
    Serial2.print("vis errornumber,1");
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);      
    Serial2.print("vis msgerror,1");
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);      
  }else if(error==2){
    Serial2.print("p0.pic=51");
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);    
  }else if(error==0){
    Serial2.print("p0.pic=53");
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);       
  }

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

  switch(error){
    case 0:
      Serial2.print("error=0");  
      Serial2.write(0xff);
      Serial2.write(0xff);
      Serial2.write(0xff);   
      Serial2.print("vis errornumber,0");
      Serial2.write(0xff);
      Serial2.write(0xff);
      Serial2.write(0xff);                
      Serial2.print("vis msgerror,0");
      Serial2.write(0xff);
      Serial2.write(0xff);
      Serial2.write(0xff);        
    break;

    case 1:
      Serial.println("SYSTEM OFF but system ON on display");
      Serial2.print("error=1");  
      Serial2.write(0xff);
      Serial2.write(0xff);
      Serial2.write(0xff);
      Serial2.print("errornumber.txt=\"ERROR (1):\"");
      Serial2.write(0xff);
      Serial2.write(0xff);
      Serial2.write(0xff);
      Serial2.print("msgerror.txt=\"SYSTEM is OFF but on display looked as ON\"");
      Serial2.write(0xff);
      Serial2.write(0xff);
      Serial2.write(0xff);                 
    break;

    case 2:
      Serial.println("ALL SYSTEMS ARE OFF");  
      Serial2.print("error=2");  
      Serial2.write(0xff);
      Serial2.write(0xff);
      Serial2.write(0xff);  
      Serial2.print("vis errornumber,0");
      Serial2.write(0xff);
      Serial2.write(0xff);
      Serial2.write(0xff);                   
      Serial2.print("vis msgerror,0");
      Serial2.write(0xff);
      Serial2.write(0xff);
      Serial2.write(0xff);
    break;

    case 3:
      Serial.println("Data incomplete broken");   
      Serial2.print("error=3");  
      Serial2.write(0xff);
      Serial2.write(0xff);
      Serial2.write(0xff);         
      Serial2.print("errornumber.txt=\"ERROR (3):\"");
      Serial2.write(0xff);
      Serial2.write(0xff);
      Serial2.write(0xff);
      Serial2.print("msgerror.txt=\"Data incomplete broken\"");
      Serial2.write(0xff);
      Serial2.write(0xff);
      Serial2.write(0xff);                                                 
    break;

    case 4:
      Serial.println(Blue.error);
      Serial.println("Many time trying to link");
      Serial2.print("error=4");  
      Serial2.write(0xff);
      Serial2.write(0xff);
      Serial2.write(0xff);            
      Serial2.print("errornumber.txt=\"ERROR (4):\"");
      Serial2.write(0xff);
      Serial2.write(0xff);
      Serial2.write(0xff);
      Serial2.print("msgerror.txt=\"Many time trying to link\"");
      Serial2.write(0xff);
      Serial2.write(0xff);
      Serial2.write(0xff);         
    break;
  }

  for(i=0; i<5; i++){
    if(mode==true){      
      Serial.print(Blue.SystemsON[i][0]);
      Serial.print(Blue.SystemsON[i][1]);
      Serial.println(Blue.SystemsON[i][2]);
    }
    if(Blue.SystemsON[i][1].compareTo("-ON")==0){
      Serial2.flush();
      Serial2.print("vis s20"+String(i+1)+"off,0");
      Serial2.write(0xff);
      Serial2.write(0xff);
      Serial2.write(0xff);                
      Serial2.print("vis s20"+String(i+1)+"on,1");
      Serial2.write(0xff);
      Serial2.write(0xff);
      Serial2.write(0xff);
      Serial2.print("tsw t20"+String(i+1)+",1");  
      Serial2.write(0xff);
      Serial2.write(0xff);
      Serial2.write(0xff);    
    }else if(Blue.SystemsON[i][1].compareTo("-ERR")==0){
      Serial2.flush();
      Serial2.print("vis s20"+String(i+1)+"on,0");
      Serial2.write(0xff);
      Serial2.write(0xff);
      Serial2.write(0xff);                
      Serial2.print("vis s20"+String(i+1)+"off,1");
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
      Serial2.print("vis s20"+String(i+1)+"off,0");
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
void HMI::connect(int send){

  /*if(send == 0){
    Serial2.flush();
    Serial2.print("page SPASConDis");
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);
  }
  if(send == 1){
    Serial2.flush();
    Serial2.print("page SPASmotor1");
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);
  }*/

  Serial2.flush();
  switch(send){
    case 0:
      Serial2.print("page SPASConDis");
    break;

    case 1:
      Serial2.print("page SPASmotor1");
    break; 

    case 2:
      Serial2.print("page SPASmotor2");
    break;     

    case 3:
      Serial2.print("page SPASmotor3");
    break;         
  }
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);

};

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
void HMI::disconnect(int send){

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
void HMI::progressBar(int value){
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

    case 30:
      Serial2.print("ProgressBar.val=30");
    break; 

    case 35:
      Serial2.print("ProgressBar.val=35");
    break;
    case 50:
      Serial2.print("ProgressBar.val=50"); 
    break;
    case 60:
      Serial2.print("ProgressBar.val=60");
    break;       
    case 70:
      Serial2.print("ProgressBar.val=70"); 
    break;
    case 75:
      Serial2.print("ProgressBar.val=75");
    break;
    case 90:
      Serial2.print("ProgressBar.val=90");
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
void HMI::menu(int state, class AT &Blue){

  if(state==0){
    Serial2.print("page SPASsearching");  
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);   
    Serial2.print("tsw m0,1");  
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff); 

    if(Blue.error==2){
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


/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
void HMI::firstDataUpdate(String a, String b, String c){
  String test="";
  if(a.length()==5){
    Serial2.print("dec1=2");
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);    
  }
  if(b.length()==5){
    Serial2.print("dec2=2");
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);    
  }
  if(c.length()==5){
    Serial2.print("dec3=2");
    Serial2.write(0xff);
    Serial2.write(0xff);
    Serial2.write(0xff);    
  }

  test="Az="+a;
  Serial2.print(test);
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);    
  test="Ax="+b;
  Serial2.print(test);
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);   
  test="Ay="+c;
  Serial2.print(test);
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);               

};

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
void HMI::dataPosition(int send, String axis){
  //1 = motor 1
  //2 = motor 2
  //3 = motor 3
  //Serial.println(axis); //example format: 27.25 = 27025
  Serial2.flush();
  String test="";

  switch(send){
    case 1:
            test="Az="+axis;
            Serial2.print(test);
            Serial2.write(0xff);
            Serial2.write(0xff);
            Serial2.write(0xff);
            if(axis.length()==4){
              Serial2.print("M1.vvs0=1");
              Serial2.write(0xff);
              Serial2.write(0xff);
              Serial2.write(0xff);    
              Serial2.print("dec1=1");
              Serial2.write(0xff);
              Serial2.write(0xff);
              Serial2.write(0xff);  
            }else{
              Serial2.print("M1.vvs0=2");
              Serial2.write(0xff);
              Serial2.write(0xff);
              Serial2.write(0xff);    
              Serial2.print("dec1=2");    
              Serial2.write(0xff);
              Serial2.write(0xff);
              Serial2.write(0xff);                
            }
            Serial2.print("M1.val=Az");
            Serial2.write(0xff);
            Serial2.write(0xff);
            Serial2.write(0xff);                            
    break;

    case 2:
            test="Ax="+axis;
            Serial2.print(test);
            Serial2.write(0xff);
            Serial2.write(0xff);
            Serial2.write(0xff);  
            if(axis.length()==4){
              Serial2.print("M2.vvs0=1");
              Serial2.write(0xff);
              Serial2.write(0xff);
              Serial2.write(0xff);    
              Serial2.print("dec2=1");
              Serial2.write(0xff);
              Serial2.write(0xff);
              Serial2.write(0xff);  
            }else{
              Serial2.print("M2.vvs0=2");
              Serial2.write(0xff);
              Serial2.write(0xff);
              Serial2.write(0xff);    
              Serial2.print("dec2=2");    
              Serial2.write(0xff);
              Serial2.write(0xff);
              Serial2.write(0xff);                
            }
            Serial2.print("M2.val=Ax");
            Serial2.write(0xff);
            Serial2.write(0xff);
            Serial2.write(0xff);        
    break;

    case 3:
            String test="Ay="+axis;
            Serial2.print(test);
            Serial2.write(0xff);
            Serial2.write(0xff);
            Serial2.write(0xff);  
            if(axis.length()==4){
              Serial2.print("M3.vvs0=1");
              Serial2.write(0xff);
              Serial2.write(0xff);
              Serial2.write(0xff);    
              Serial2.print("dec3=1");
              Serial2.write(0xff);
              Serial2.write(0xff);
              Serial2.write(0xff);  
            }else{
              Serial2.print("M3.vvs0=2");
              Serial2.write(0xff);
              Serial2.write(0xff);
              Serial2.write(0xff);    
              Serial2.print("dec3=2");    
              Serial2.write(0xff);
              Serial2.write(0xff);
              Serial2.write(0xff);                
            }
            Serial2.print("M3.val=Ay");
            Serial2.write(0xff);
            Serial2.write(0xff);
            Serial2.write(0xff);        
    break;
  }

};

void HMI::setDatafromMemorytoDisplay(){

};

void HMI::picture(String pic){
  pic = "p0.pic="+pic;
  Serial2.print(pic);
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff); 
};
