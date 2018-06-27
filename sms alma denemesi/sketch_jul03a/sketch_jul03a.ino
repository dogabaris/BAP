#include <SoftwareSerial.h>
#include <String.h>
SoftwareSerial gprsSerial(8,7);
void setup()
{
  gprsSerial.begin(19200); // GPRS shield baud rate 
  Serial.begin(19200);   
  delay(500);
  pinMode(A1, OUTPUT);
  analogWrite(A1, 255);
  gprsSerial.print("AT+CMGF=1\r"); // Set the shield to SMS mode
  delay(100);
  gprsSerial.print("AT+CNMI=2,2,0,0,0"); //canlı sms receive yapar
  delay(100);
}
void loop()
{
  String buffer = gprsSerial.readString();
  if (buffer.startsWith("\r\n+CMT: "))
  {
      Serial.println("*** RECEIVED SMS ***");
      Serial.println(buffer);
      buffer.remove(0, 70);
      int len = buffer.length();
      buffer.remove(len - 2, 2);
      Serial.print(buffer);
      if(buffer=="1")
        analogWrite(A1, 0);
  }
  delay(100);
}
 

