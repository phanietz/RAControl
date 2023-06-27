/*
 * Pinout:
 * Key/ENABLE --> pin 9
 * VCC --> Vin
 * TXD --> RX SERIAL2
 * RXD --> TX SERIAL2
 * GND --> GND
 */


 //PUT SERIAL MONITOR: BOTH NL & CR
 //PUT 9600 BAUD TO SERIAL MONITOR
 //PUT 38400 BAUD TO SERIAL BLUETOOTH

void setup()
{
  pinMode(9, OUTPUT);
  digitalWrite(9, HIGH);
  Serial.begin(9600);

  pinMode(15, INPUT_PULLUP);
  Serial.println("Enter AT commands:");
  Serial3.begin(38400);  // HC-05 default speed in AT command mode

}
 
void loop()
{
  // Read from HC05 and send to Arduino
  if (Serial3.available())
    Serial.write(Serial3.read());
 
  // Read from serial monitor and send to HC05
  if (Serial.available()){
    Serial3.write(Serial.read()); 
  }
    
}
