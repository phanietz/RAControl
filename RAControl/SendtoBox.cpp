#include "SendtoBox.h"

BOX::BOX(){};

BOX::motor1(){
  Serial.print("To Box: ");
  Serial.println("16753245");
  Serial3.write("16753245.\r\n");
  Serial3.flush();
  delay(200);
}

BOX::motor2(){
  Serial.print("To Box: ");
  Serial.println("16736925");
  Serial3.write("16736925.\r\n");
  Serial3.flush();
  delay(200);
}

BOX::motor3(){
  Serial.print("To Box: ");
  Serial.println("16769565");
  Serial3.write("16769565.\r\n");
  Serial3.flush();
  delay(200);
}

BOX::x1(){
  Serial.print("To Box: ");
  Serial.println("16720605");
  Serial3.write("16720605.\r\n");
  Serial3.flush();
  delay(200);  
};

BOX::x2(){
  Serial.print("To Box: ");
  Serial.println("16712445");
  Serial3.write("16712445.\r\n");
  Serial3.flush();
  delay(200);
};

BOX::y1(){
  Serial.print("To Box: ");
  Serial.println("16720605");
  Serial3.write("16720605.\r\n");
  Serial3.flush();
  delay(200);
};

BOX::y2(){
  Serial.print("To Box: ");
  Serial.println("16712445");
  Serial3.write("16712445.\r\n");
  Serial3.flush();
  delay(200);
};

BOX::z1(){
  Serial.print("To Box: ");
  Serial.println("16720605");
  Serial3.write("16720605.\r\n");
  Serial3.flush();
  delay(200);
};

BOX::z2(){
  Serial.print("To Box: ");
  Serial.println("16712445");
  Serial3.write("16712445.\r\n");
  Serial3.flush();
  delay(200);
};

BOX::step(int s){
  if(s==1){
    Serial.print("To Box: ");
    Serial.println("1001");
    Serial3.write("1001.\r\n");
    Serial3.flush();
    delay(200);    
  }else if(s==2){
    Serial.print("To Box: ");
    Serial.println("1002");
    Serial3.write("1002.\r\n");
    Serial3.flush();
    delay(200);    
  }else if(s==3){
    Serial.print("To Box: ");
    Serial.println("1003");
    Serial3.write("1003.\r\n");
    Serial3.flush();
    delay(200);    
  }
};

BOX::reboot(){
  Serial.print("To Box: ");
  Serial.println("16756815");
  Serial3.write("16756815.\r\n");
  Serial3.flush();
  delay(200);
};

BOX::enter(){
  Serial.print("To Box: ");
  Serial.println("16761405");
  Serial3.write("16761405.\r\n");
  Serial3.flush();
  delay(200);
};

BOX::exit(){
  Serial.print("To Box: ");
  Serial.println("16748655");
  Serial3.write("16748655.\r\n");
  Serial3.flush();
  delay(200);
};

BOX::receiveDataOfMemory(){
  Serial3.write("0000.\r\n");
  Serial3.flush();
  delay(200);
};