#include "Nextion.h"
SoftwareSerial HMISerial(10, 11);

NexText t0 = NexText(0, 2, "t0");
NexNumber n0 = NexNumber(0, 12, "n0");
NexNumber n1 = NexNumber(0, 13, "n1");
NexNumber n2 = NexNumber(0, 17, "n2");
NexNumber n3 = NexNumber(0, 18, "n3");
NexText t13 = NexText(0, 26, "t13");
NexText t12 = NexText(0, 25, "t12");
NexText t11 = NexText(0, 24, "t11");
NexText t10 = NexText(0, 23, "t10");
NexButton b0 = NexButton(0, 8, "b0");
NexButton b1 = NexButton(0, 9, "b1");
NexButton b2 = NexButton(0, 10, "b2");
NexButton b3 = NexButton(0, 11, "b3");
NexSlider h0 = NexSlider(0, 1, "h0");
NexGauge z0  = NexGauge(0, 3, "z0");
NexDSButton bt0 = NexDSButton(0, 5, "bt0");

NexTouch *nex_listen_list[] = {&h0, &bt0, &b0,&b1,&b2,&b3, NULL};
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
void h0PopCallback(void *ptr)
{
  int numberx = 0;
  int gosterge = 0;
  uint32_t number = 0;
  char temp[10] = {0};
  dbSerialPrintln("h0PopCallback");
  h0.getValue(&number);
  utoa(number, temp, 10);
  t0.setText(temp);
  numberx = number;
  gosterge = map(numberx, 0, 100, 0, 180);
  z0.setValue(gosterge);
}

void setup(void)
{
  pinMode(13, OUTPUT);
  nexInit();
  n0.setValue(inekSayisi);
  n1.setValue(toplamSut);
  
  h0.attachPop(h0PopCallback);
  bt0.attachPop(bt0PopCallback, &bt0);
  b0.attachPop(b0PopCallback, &b0);
  b1.attachPop(b1PopCallback, &b1);
  b2.attachPop(b2PopCallback, &b2);
  b3.attachPop(b3PopCallback, &b3);
  dbSerialPrintln("setup done");
}
void setSicaklik(){
  sicaklik = random(20,25);
  n2.setValue(sicaklik);
}
void setNem(){
  nem = random(20,25);
  n3.setValue(nem);
}
void loop(void)
{
  nexLoop(nex_listen_list);
  if (millis() - lastTime > 1000)//1sn
  {
    setSicaklik();
    setNem();
    calculateSni(sicaklik, nem);
    lastTime = millis();
  }
}
