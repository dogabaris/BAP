#include <SoftwareSerial.h>
#include <String.h>
SoftwareSerial gprsSerial(8,7);
String readSIM()
{
    String buffer;

    while (gprsSerial.available())
    {
        char c = gprsSerial.read();
        buffer.concat(c);
        //delay(10);
    }

    return buffer;
}
void RecieveMessage(){
  //gprsSerial.print("AT+CMGF=1\r");
  //delay(100);
  gprsSerial.println("AT+CNMI=2,2,0,0,0"); // "AT+CMGL=\"REC UNREAD\""
  delay(100);
  gprsSerial.print((char)26);
  //delay(100);
  //gprsSerial.println("\r");
  delay(1000);
}

void SendTextMessage()
{
  Serial.println("Sending Text...");
  //gprsSerial.print("AT+CMGF=1\r"); // Set the shield to SMS mode
  //delay(100);
  gprsSerial.println("AT+CMGS=\"+905432814906\"");
  delay(100);
  gprsSerial.println("How are you today?"); //the content of the message
  delay(100);
  gprsSerial.print((char)26);//the ASCII code of the ctrl+z is 26 (required according to the datasheet)
  delay(100);
  gprsSerial.println();
  Serial.println("Text Sent.");
}
void setup()
{
  gprsSerial.begin(19200); // GPRS shield baud rate 
  Serial.begin(19200);   
  delay(500);
  //gprsSerial.print("AT+CMGF=1\r"); // sms modu
  //delay(100);
  //gprsSerial.print("AT+CMGD=1,4\r");// önceki tüm smsleri siler
  //delay(100);
  //gprsSerial.print("AT+CPMS=\"SM\"\r"); //smsleri kabul eder
  //delay(100);
  gprsSerial.print("AT+CNMI=2,2,0,0,0"); //canlı sms receive yapar
  delay(100);
}
 
void loop()
{
  String buffer = readSIM();
  if (buffer.startsWith("\r\n+CMT: "))
  {
      Serial.println("*** RECEIVED SMS ***");
      
      // Remove first 51 characters
      buffer.remove(0, 56);
      int len = buffer.length();
      // Remove \r\n from tail
      buffer.remove(len - 2, 2);
      Serial.print("b");
      Serial.print(buffer);
      Serial.print("b");
      if(buffer=="1")
        Serial.println("A1 role devrede");
      
      Serial.println("*** END SMS ***");
  }
  delay(100);
    
  //if (gprsSerial.available()){ // if the shield has something to say
  //  Serial.write(gprsSerial.read()); // display the output of the shield
  //  char c = SIM900A.read();
  //      buffer.concat(c);
   //     delay(10);
  //}

    
}
 

