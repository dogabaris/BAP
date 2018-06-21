#include "Nextion.h"
#include <String.h>
#include <SHT1x.h>
#define dataPin 12
#define clockPin 13
SHT1x sht1x(dataPin, clockPin);
SoftwareSerial HMISerial(10, 11);
SoftwareSerial gprsSerial(8,7);

NexText t0 = NexText(0, 2, "t0");
NexNumber n0 = NexNumber(0, 9, "n0");
NexNumber n1 = NexNumber(0, 10, "n1");
NexNumber n2 = NexNumber(0, 14, "n2");
NexNumber n3 = NexNumber(0, 15, "n3");
NexText t16 = NexText(0, 27, "t16");
NexText g0 = NexText(0, 26, "g0");
NexText t13 = NexText(0, 23, "t13");
NexText t12 = NexText(0, 22, "t12");
NexText t11 = NexText(0, 21, "t11");
NexText t10 = NexText(0, 20, "t10");
NexButton b0 = NexButton(0, 5, "b0");
NexButton b1 = NexButton(0, 6, "b1");
NexButton b2 = NexButton(0, 7, "b2");
NexButton b3 = NexButton(0, 8, "b3");
NexDSButton bt0 = NexDSButton(0, 2, "bt0");

NexTouch *nex_listen_list[] = {&bt0, &b0,&b1,&b2,&b3, NULL};
int sicaklik = 0;
int nem = 0;
int inekSayisi = 0;
int toplamSut = 0;
float sni = 0;
unsigned long lastTime = 0;
float calculateSni(int sicaklik, int nem){
  sni = (1.8*sicaklik + 32)-(0.55 - 0.0055 * nem)*(1.8*sicaklik-26);
  char sniStr[8];
  dtostrf(sni, 6, 3, sniStr);
  t11.setText(sniStr);
}
float tempC = 0;
float humidity = 0;
void bt0PopCallback(void *ptr)
{
  uint32_t dual_state;
  NexDSButton *btn = (NexDSButton *)ptr;
  dbSerialPrintln("b0PopCallback");
  dbSerialPrint("ptr=");
  dbSerialPrintln((uint32_t)ptr);
  bt0.getValue(&dual_state);
  
  if (dual_state)               
  {
    digitalWrite(13, HIGH);           // if my dual state buton is pressed
  }
  else
  {
    digitalWrite(13, LOW);            // if not
  }
}
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

void setup(void)
{
  pinMode(13, OUTPUT);
  nexInit();
  n0.setValue(inekSayisi);
  n1.setValue(toplamSut);
  
  bt0.attachPop(bt0PopCallback, &bt0);
  b0.attachPop(b0PopCallback, &b0);
  b1.attachPop(b1PopCallback, &b1);
  b2.attachPop(b2PopCallback, &b2);
  b3.attachPop(b3PopCallback, &b3);
  //dbSerialPrintln("setup done");
}
void setSicaklik(float sicaklik){
  n2.setValue(sicaklik);
}
void setNem(float nem){
  n3.setValue(nem);
}
void SendTextMessage()
{
  Serial.println("Sending Text...");
  gprsSerial.print("AT+CMGF=1\r"); // Set the shield to SMS mode
  delay(100);
  gprsSerial.println("AT+CMGS = \"+0905433413415\"");
  delay(100);
  gprsSerial.println("How are you today?"); //the content of the message
  delay(100);
  gprsSerial.print((char)26);//the ASCII code of the ctrl+z is 26 (required according to the datasheet)
  delay(100);
  gprsSerial.println();
  Serial.println("Text Sent.");
}
void DialVoiceCall()
{
  gprsSerial.println("ATD+xxxxxxx;");//dial the number, must include country code
  delay(100);
  gprsSerial.println();
}

void loop(void)
{
  if (Serial.available()) // if there is incoming serial data
   switch(Serial.read()) // read the character
   {
     case 't': // if the character is 't'
       SendTextMessage(); // send the text message
       break;
     case 'd': // if the character is 'd'
       DialVoiceCall(); // dial a number 
       break;
   } 
 
  if (gprsSerial.available()){ // if the shield has something to say
    Serial.write(gprsSerial.read()); // display the output of the shield
  }
  
  nexLoop(nex_listen_list);
  if (millis() - lastTime > 1000)//1sn
  {
    tempC = sht1x.readTemperatureC();
    humidity = sht1x.readHumidity();
    Serial.println("Sicaklik");
    Serial.println(tempC);
    Serial.println("Nem");
    Serial.println(humidity);
    setSicaklik(tempC);
    setNem(humidity);
    calculateSni(tempC, humidity);
    lastTime = millis();
  }
}
