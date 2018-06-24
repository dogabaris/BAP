#include <SoftwareSerial.h>
#include "Nextion.h"
#include <String.h>
#include <SHT1x.h>
#define dataPin 10
#define clockPin 11
SoftwareSerial gprsSerial(8,7);
SHT1x sht1x(dataPin, clockPin);
SoftwareSerial HMISerial(13, 12);

NexNumber n0 = NexNumber(0, 9, "n0");
NexNumber n1 = NexNumber(0, 10, "n1");
NexNumber n2 = NexNumber(0, 14, "n2");
NexNumber n3 = NexNumber(0, 15, "n3");
NexText t16 = NexText(0, 27, "t16");
NexText t0 = NexText(0, 31, "t0");
NexText t17 = NexText(0, 32, "t17");
NexText t18 = NexText(0, 33, "t18");
NexText g0 = NexText(0, 26, "g0");
NexText g1 = NexText(0, 28, "g1");
NexText g2 = NexText(0, 29, "g2");
NexText g3 = NexText(0, 30, "g3");
NexText t11 = NexText(0, 21, "t11");
NexButton b0 = NexButton(0, 5, "b0");
NexButton b1 = NexButton(0, 6, "b1");
NexButton b2 = NexButton(0, 7, "b2");
NexButton b3 = NexButton(0, 8, "b3");

byte ter[3] = {255,255,255};
bool sendMessageActive = true;
//NexTouch *nex_listen_list[] = { &b0,&b1,&b2,&b3, NULL};
int sicaklik = 0;
int nem = 0;
int inekSayisi = 0;
int toplamSut = 0;
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

void b0PopCallback(void *ptr)
{
  dbSerialPrintln("b0PopCallback");
  dbSerialPrint("ptr=");
  if(inekSayisi!=0)
    inekSayisi = inekSayisi-1;
  n0.setValue(inekSayisi);
}
void b1PopCallback(void *ptr)
{
  dbSerialPrintln("b1PopCallback");
  dbSerialPrint("ptr=");
  inekSayisi = inekSayisi+1;
  n0.setValue(inekSayisi);
}
void b2PopCallback(void *ptr)
{
  dbSerialPrintln("b2PopCallback");
  dbSerialPrint("ptr=");
  if(toplamSut!=0)
    toplamSut = toplamSut-1;
  n1.setValue(toplamSut);
}
void b3PopCallback(void *ptr)
{
  dbSerialPrintln("b3PopCallback");
  dbSerialPrint("ptr=");
  toplamSut = toplamSut+1;
  n1.setValue(toplamSut);
}

void SendTextMessage(int stresSeviyesi)
{
  if(sendMessageActive==true){
    Serial.println("Sending Text...");
    gprsSerial.print("AT+CMGF=1\r"); // Set the shield to SMS mode
    delay(100);
    gprsSerial.println("AT+CMGS=\"+905432814906\"");
    delay(100);
    if(stresSeviyesi==1)
      gprsSerial.println("Alarm");
    else if(stresSeviyesi==2)
      gprsSerial.println("Tehlike baslangici");
    else if(stresSeviyesi==3)
      gprsSerial.println("Acil durum");
    delay(100);
    gprsSerial.print((char)26);//the ASCII code of the ctrl+z is 26 (required according to the datasheet)
    delay(100);
    gprsSerial.println();
    Serial.println("Text Sent.");
    sendMessageActive = false;
  }
}
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
void setup(void)
{
  HMISerial.begin(19200);
  delay(500);
  gprsSerial.begin(19200);
  delay(500);
  Serial.begin(19200);
  delay(500);
  pinMode(A1, OUTPUT);
  analogWrite(A1, 255);
  pinMode(A2, OUTPUT);
  analogWrite(A2, 255);
  pinMode(A3, OUTPUT);
  analogWrite(A3, 255);
  pinMode(A4, OUTPUT);
  analogWrite(A4, 255);
  gprsSerial.print("AT+CNMI=2,2,0,0,0"); //canlı sms receive yapar
  delay(100);
  nexInit();
  n0.setValue(inekSayisi);
  n1.setValue(toplamSut);
  
  b0.attachPop(b0PopCallback, &b0);
  b1.attachPop(b1PopCallback, &b1);
  b2.attachPop(b2PopCallback, &b2);
  b3.attachPop(b3PopCallback, &b3);
  nexSerial.print("vis t16,0");
  nexSerial.write(ter,3);
  nexSerial.print("vis t0,0");
  nexSerial.write(ter,3);
  nexSerial.print("vis t17,0");
  nexSerial.write(ter,3);
  nexSerial.print("vis t18,0");
  nexSerial.write(ter,3);
  nexSerial.print("vis g0,0");
  nexSerial.write(ter,3);
  nexSerial.print("vis g1,0");
  nexSerial.write(ter,3);
  nexSerial.print("vis g2,0");
  nexSerial.write(ter,3);
  nexSerial.print("vis g3,0");
  nexSerial.write(ter,3);
}
void setSicaklik(float sicaklik){
  n2.setValue(sicaklik);
}
void setNem(float nem){
  n3.setValue(nem);
}
void setScreenSniStates(){
  if(sni<72){
    nexSerial.print("vis t16,1");
    nexSerial.write(ter,3);
    nexSerial.print("vis g0,1");
    nexSerial.write(ter,3);
    nexSerial.print("vis t0,0");
    nexSerial.write(ter,3);
    nexSerial.print("vis g1,0");
    nexSerial.write(ter,3);
    nexSerial.print("vis t17,0");
    nexSerial.write(ter,3);
    nexSerial.print("vis g2,0");
    nexSerial.write(ter,3);
    nexSerial.print("vis t18,0");
    nexSerial.write(ter,3);
    nexSerial.print("vis g3,0");
    nexSerial.write(ter,3);
  }
  else if(sni>=72 && sni<80){
    nexSerial.print("vis t16,0");
    nexSerial.write(ter,3);
    nexSerial.print("vis g0,0");
    nexSerial.write(ter,3);
    nexSerial.print("vis t0,1");
    nexSerial.write(ter,3);
    nexSerial.print("vis g1,1");
    nexSerial.write(ter,3);
    nexSerial.print("vis t17,0");
    nexSerial.write(ter,3);
    nexSerial.print("vis g2,0");
    nexSerial.write(ter,3);
    nexSerial.print("vis t18,0");
    nexSerial.write(ter,3);
    nexSerial.print("vis g3,0");
    nexSerial.write(ter,3);
    SendTextMessage(1);
  }
  else if(sni>=80 && sni<90){
    nexSerial.print("vis t16,0");
    nexSerial.write(ter,3);
    nexSerial.print("vis g0,0");
    nexSerial.write(ter,3);
    nexSerial.print("vis t0,0");
    nexSerial.write(ter,3);
    nexSerial.print("vis g1,0");
    nexSerial.write(ter,3);
    nexSerial.print("vis t17,1");
    nexSerial.write(ter,3);
    nexSerial.print("vis g2,1");
    nexSerial.write(ter,3);
    nexSerial.print("vis t18,0");
    nexSerial.write(ter,3);
    nexSerial.print("vis g3,0");
    nexSerial.write(ter,3);
    SendTextMessage(2);
  }
  else if(sni>=90){
    nexSerial.print("vis t16,0");
    nexSerial.write(ter,3);
    nexSerial.print("vis g0,0");
    nexSerial.write(ter,3);
    nexSerial.print("vis t0,0");
    nexSerial.write(ter,3);
    nexSerial.print("vis g1,0");
    nexSerial.write(ter,3);
    nexSerial.print("vis t17,0");
    nexSerial.write(ter,3);
    nexSerial.print("vis g2,0");
    nexSerial.write(ter,3);
    nexSerial.print("vis t18,1");
    nexSerial.write(ter,3);
    nexSerial.print("vis g3,1");
    nexSerial.write(ter,3);
    SendTextMessage(3);
  }
}
void loop(void)
{  
  String buffer = readSIM();
  if (buffer.startsWith("\r\n+CMT: "))
  {
      Serial.println("RECEIVED SMS");
      buffer.remove(0, 56);
      int len = buffer.length();
      buffer.remove(len - 2, 2);
      Serial.println(buffer);
      if(buffer=="1")
        analogWrite(A1, 0);
      if(buffer=="2")
        analogWrite(A2, 0);
      if(buffer=="3")
        analogWrite(A3, 0);
      if(buffer=="4")
        analogWrite(A4, 0);
  }
  //delay(100);
  setScreenSniStates();
  if(millis() - sendMessagelastTime > 900000){//15 dk
    sendMessageActive = true;
    sendMessagelastTime = millis();
  }
  if (millis() - lastTime > 1000)//1sn
  {
    tempC = sht1x.readTemperatureC();
    humidity = sht1x.readHumidity();
    setSicaklik(tempC);
    setNem(humidity);
    calculateSni(tempC, humidity);
    lastTime = millis();
  }
  //nexLoop(nex_listen_list);
}
