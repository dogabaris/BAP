//#include <SoftwareSerial.h>
#include "Nextion.h"
#include <String.h>
#include <SHT1x.h>
#define dataPin 10
#define clockPin 11
//SoftwareSerial gprsSerial(8,7);
SHT1x sht1x(dataPin, clockPin);
//SoftwareSerial HMISerial(13, 12);

NexNumber n2 = NexNumber(0, 14, "n2");
NexNumber n3 = NexNumber(0, 15, "n3");
NexText t16 = NexText(0, 27, "t16");
NexText t0 = NexText(0, 28, "t0");
NexText t17 = NexText(0, 29, "t17");
NexText t18 = NexText(0, 30, "t18");
NexText g0 = NexText(0, 26, "g0");
NexText t19 = NexText(0, 31, "t19");
NexText t20 = NexText(0, 32, "t20");
NexText t21 = NexText(0, 33, "t21");
NexText t11 = NexText(0, 21, "t11");

NexText t9 = NexText(0, 19, "t9");
NexText t10 = NexText(0, 20, "t10");
NexText t6 = NexText(0, 16, "t6");
NexText t13 = NexText(0, 23, "t13");
NexText t7 = NexText(0, 17, "t7");
NexText t12 = NexText(0, 22, "t12");
NexButton b0 = NexButton(0, 5, "b0");
NexButton b1 = NexButton(0, 6, "b1");
NexButton b2 = NexButton(0, 7, "b2");
NexButton b3 = NexButton(0, 8, "b3");

byte ter[3] = {255,255,255};
bool sendMessageActive = true;
float sicaklik = 0;
float nem = 0;
float sni = 0;
unsigned long lastTime = 0;
unsigned long sendMessagelastTime = 0;
float calculateSni(int sicaklik, int nem){
  sni = (1.8*sicaklik + 32)-(0.55 - 0.0055 * nem)*(1.8*sicaklik-26);
  char sniStr[8];
  dtostrf(sni, 6, 3, sniStr);
  t11.setText(sniStr);
}
float tempC = 0;
float humidity = 0;
void SendTextMessage(int stresSeviyesi)
{
  if(sendMessageActive==true){
    ////Serial.println("Sending Text...");
    Serial1.print("AT+CMGF=1\r"); // Set the shield to SMS mode
    delay(100);
    Serial1.println("AT+CMGS=\"+905325798234\"");
    delay(100);
    if(stresSeviyesi==1)
      Serial1.println("Alarm");
    else if(stresSeviyesi==2)
      Serial1.println("Tehlike baslangici");
    else if(stresSeviyesi==3)
      Serial1.println("Acil durum");
    delay(100);
    Serial1.print((char)26);//the ASCII code of the ctrl+z is 26 (required according to the datasheet)
    delay(100);
    Serial1.println();
    //Serial.println("Text Sent.");
    sendMessageActive = false;
  }
}
String readSIM()
{
    String buffer;
    while (Serial1.available())
    {
        char c = Serial1.read();
        buffer.concat(c);
        delay(10);
    }
    return buffer;
}
void sendData(){
  
  Serial1.println("AT+SAPBR=3,1,\"APN\",\"internet\"");
  delay(1000);
  //Serial1.println("AT+SAPBR=1,1");
  //delay(1500);
  Serial1.println("AT+HTTPINIT");
  delay(1000);
  Serial1.println("AT+HTTPPARA=\"CID\",1");
  delay(1000);
  
  //Serial1.println("AT+HTTPPARA=\"URL\",\"http://www.unalkizil.com/index.php?sicaklik=21&nem=22&sni=23\"");
  //Serial1.println("AT+HTTPACTION=0");
  
  char valA[10], valB[10], valC[10];
  
  dtostrf(tempC, 3, 3, valA);
  dtostrf(humidity, 3, 3, valB);
  dtostrf(sni, 3, 3, valC);
  char urlData[180];
  
  strcpy(urlData, "AT+HTTPPARA=\"URL\",\"http://www.unalkizil.com/index.php?");
  strcat(urlData, "sicaklik=");
  strcat(urlData, valA);
  strcat(urlData, "&nem=");
  strcat(urlData, valB);
  strcat(urlData, "&sni=");
  strcat(urlData, valC);
  strcat(urlData, "\"");
  
  Serial.print("Payload: ");      //
  Serial.print(urlData);          // Print to serial console to see what is inside the payload
  Serial.println(" EOL");
  delay(1000);
  Serial1.println(urlData);
  delay(1000);
  Serial1.println("AT+HTTPACTION=0");
  Serial.println("Data sent!");
}
void setup(void)
{
  Serial.begin(115200);
  //HMISerial.begin(115200);
  Serial1.begin(115200);
  Serial2.begin(115200);
  delay(500);
  pinMode(A1, OUTPUT);
  analogWrite(A1, 255);
  pinMode(A2, OUTPUT);
  analogWrite(A2, 255);
  pinMode(A3, OUTPUT);
  analogWrite(A3, 255);
  pinMode(A4, OUTPUT);
  analogWrite(A4, 255);
  //Serial1.print("AT+CMGF=1\r"); // Set the shield to SMS mode
  //delay(100);
  Serial1.print("AT+CNMI=2,2,0,0,0"); //canlı sms receive yapar
  delay(100);
  Serial1.print("AT+IPR=115200");//otomatik baud
  nexInit();
  Serial.println("Başladı.");
  Serial2.print("vis t9,0");
  Serial2.write(ter,3);
  Serial2.print("vis t10,0");
  Serial2.write(ter,3);
  Serial2.print("vis t6,0");
  Serial2.write(ter,3);
  Serial2.print("vis t13,0");
  Serial2.write(ter,3);
  Serial2.print("vis t7,0");
  Serial2.write(ter,3);
  Serial2.print("vis t12,0");
  Serial2.write(ter,3);
  
  Serial2.print("vis t16,0");
  Serial2.write(ter,3);
  Serial2.print("vis t0,0");
  Serial2.write(ter,3);
  Serial2.print("vis t17,0");
  Serial2.write(ter,3);
  Serial2.print("vis t18,0");
  Serial2.write(ter,3);
  Serial2.print("vis g0,0");
  Serial2.write(ter,3);
  Serial2.print("vis t19,0");
  Serial2.write(ter,3);
  Serial2.print("vis t20,0");
  Serial2.write(ter,3);
  Serial2.print("vis t21,0");
  Serial2.write(ter,3);
}
void setSicaklik(float sicaklik){
  n2.setValue(sicaklik);
}
void setNem(float nem){
  n3.setValue(nem);
}
void setScreenSniStates(){
  if(sni<72){
    Serial2.print("vis t16,1");
    Serial2.write(ter,3);
    Serial2.print("vis g0,1");
    Serial2.write(ter,3);
    Serial2.print("vis t0,0");
    Serial2.write(ter,3);
    Serial2.print("vis t19,0");
    Serial2.write(ter,3);
    Serial2.print("vis t17,0");
    Serial2.write(ter,3);
    Serial2.print("vis t20,0");
    Serial2.write(ter,3);
    Serial2.print("vis t18,0");
    Serial2.write(ter,3);
    Serial2.print("vis t21,0");
    Serial2.write(ter,3);
  }
  else if(sni>=72 && sni<80){
    Serial2.print("vis t16,0");
    Serial2.write(ter,3);
    Serial2.print("vis g0,0");
    Serial2.write(ter,3);
    Serial2.print("vis t0,1");
    Serial2.write(ter,3);
    Serial2.print("vis t19,1");
    Serial2.write(ter,3);
    Serial2.print("vis t17,0");
    Serial2.write(ter,3);
    Serial2.print("vis t20,0");
    Serial2.write(ter,3);
    Serial2.print("vis t18,0");
    Serial2.write(ter,3);
    Serial2.print("vis t21,0");
    Serial2.write(ter,3);
    SendTextMessage(1);
  }
  else if(sni>=80 && sni<90){
    Serial2.print("vis t16,0");
    Serial2.write(ter,3);
    Serial2.print("vis g0,0");
    Serial2.write(ter,3);
    Serial2.print("vis t0,0");
    Serial2.write(ter,3);
    Serial2.print("vis t19,0");
    Serial2.write(ter,3);
    Serial2.print("vis t17,1");
    Serial2.write(ter,3);
    Serial2.print("vis t20,1");
    Serial2.write(ter,3);
    Serial2.print("vis t18,0");
    Serial2.write(ter,3);
    Serial2.print("vis t21,0");
    Serial2.write(ter,3);
    SendTextMessage(2);
  }
  else if(sni>=90){
    Serial2.print("vis t16,0");
    Serial2.write(ter,3);
    Serial2.print("vis g0,0");
    Serial2.write(ter,3);
    Serial2.print("vis t0,0");
    Serial2.write(ter,3);
    Serial2.print("vis t19,0");
    Serial2.write(ter,3);
    Serial2.print("vis t17,0");
    Serial2.write(ter,3);
    Serial2.print("vis t20,0");
    Serial2.write(ter,3);
    Serial2.print("vis t18,1");
    Serial2.write(ter,3);
    Serial2.print("vis t21,1");
    Serial2.write(ter,3);
    SendTextMessage(3);
  }
}
void loop(void)
{
  String buffer = readSIM();
  if(buffer!="")
    Serial.println(buffer);
  if (buffer.startsWith("\r\n+CMT: "))
  {
      Serial.println(buffer);
      Serial.println("RECEIVED SMS");
      buffer.remove(0, 49);
      int len = buffer.length();
      //buffer.remove(len - 2, 2);
      Serial.print("*");
      Serial.print(buffer);
      Serial.print("*");
      if(buffer.indexOf("role1") > 0)
        analogWrite(A1, 0);
      if(buffer.indexOf("role2") > 0)
        analogWrite(A2, 0);
      if(buffer.indexOf("role3") > 0)
        analogWrite(A3, 0);
      if(buffer.indexOf("role4") > 0)
        analogWrite(A4, 0);
      if(buffer.indexOf("kaparole1") > 0)
        analogWrite(A1, 255);
      if(buffer.indexOf("kaparole2") > 0)
        analogWrite(A2, 255);
      if(buffer.indexOf("kaparole3") > 0)
        analogWrite(A3, 255);
      if(buffer.indexOf("kaparole4") > 0)
        analogWrite(A4, 255);
  }
  delay(100);
  setScreenSniStates();
  if(millis() - sendMessagelastTime > 900000){//15dk 900000
    sendMessageActive = true;
    sendMessagelastTime = millis();
  }
  if (millis() - lastTime > 60000)//1dk
  {
    tempC = sht1x.readTemperatureC();
    humidity = sht1x.readHumidity();
    Serial.println(tempC);
    Serial.println(humidity);
    setSicaklik(tempC);
    setNem(humidity);
    calculateSni(tempC, humidity);
    sendData();
    lastTime = millis();
  }
}
