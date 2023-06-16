#include "SendtoBox.h"

Box::Box(){};

Box::motor1(){
  Serial.println("envia motor1");
  Serial3.write("16753245.\r\n");
  Serial3.flush();
  delay(200);
}

Box::motor2(){
  Serial.println("envia motor2");
  Serial3.write("16736925.\r\n");
  Serial3.flush();
  delay(200);
}

Box::motor3(){
  Serial.println("envia motor3");
  Serial3.write("16769565.\r\n");
  Serial3.flush();
  delay(200);
}

Box::x1(){
  Serial.println("envia +x");
  Serial3.write("16720605.\r\n");
  Serial3.flush();
  delay(200);
};

Box::x2(){
  Serial.println("envia -x");
  Serial3.write("16712445.\r\n");
  Serial3.flush();
  delay(200);
};

Box::y1(){
  Serial.println("envia +y");
  Serial3.write("16720605.\r\n");
  Serial3.flush();
  delay(200);
};

Box::y2(){
  Serial.println("envia -y");
  Serial3.write("16712445.\r\n");
  Serial3.flush();
  delay(200);
};

Box::z1(){
  Serial.println("envia +z");
  Serial3.write("16720605.\r\n");
  Serial3.flush();
  delay(200);
};

Box::z2(){
  Serial.println("envia -z");
  Serial3.write("16712445.\r\n");
  Serial3.flush();
  delay(200);
};

Box::slow(){
  Serial.println("envia slow");
  Serial3.write("16769055.\r\n");
  Serial3.flush();
  delay(200);
};

Box::fast(){
  Serial.println("envia fast");
  Serial3.write("16754775.\r\n");
  Serial3.flush();
  delay(200);
};

Box::reboot(){
  Serial.println("envia reboot");
  Serial3.write("16756815.\r\n");
  Serial3.flush();
  delay(200);
};

Box::enter(){
  Serial.println("envia enter");
  Serial3.write("16761405.\r\n");
  Serial3.flush();
  delay(200);
};

Box::exit(){
  Serial.println("envia exit");
  Serial3.write("16748655.\r\n");
  Serial3.flush();
  delay(200);
};