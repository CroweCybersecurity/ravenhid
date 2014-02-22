#include <LiquidCrystal.h>
#include <SoftPWM.h>
#include "lcd.h"
#include "weigand.h"

#define LCD_BLUE A4
#define LCD_GREEN A3
#define LCD_RED A2
#define LCD_RS A0
#define LCD_ENABLE A1
#define LCD_D4 8
#define LCD_D5 7
#define LCD_D6 6
#define LCD_D7 5

#define RFID_D0 2
#define RFID_D1 3

#define SUPPORT_LONGLONG

#define READER_DELAY 500 // How many ms do we wait before we call it read

LiquidCrystal lcd(LCD_RS, LCD_ENABLE, LCD_D4, LCD_D5, LCD_D6, LCD_D7);
Weigand w(RFID_D0, RFID_D1);

void readD0() {
  w.add0();
}  

void readD1() {
  w.add1();
}

void setup() {
  Serial.begin(57600);
  Serial.print(F("Starting up... "));
  
  Serial.print(F("lcd "));
  lcd.begin(16, 2);
  lcdClear();
  lcd.print("Starting up...");
 
  SoftPWMBegin();
  
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(A4, OUTPUT);

  Serial.print(F("interrupts "));     
  attachInterrupt(0, readD0, FALLING);
  attachInterrupt(1, readD1, FALLING);

  //Delay a little bit to let things settle out.
  delay(10);

  Serial.print(F("weigand "));
  w.resetdata();

  Serial.println(F("done."));
  lcdClear();
}

void loop() {
  if(w.loop()) {
     Serial.println("");
     setLCDColor(green);
     lcdWriteLine1("Got a card!");
     Serial.print(F("Card bytes: "));
     Serial.println(w.getcount());
     Serial.print(F("Card data: "));
     Serial.println(w.getdata(), BIN);
     Serial.print(F("Card data w/ preamble: "));
     Serial.println(w.parsecard(), BIN);
     Serial.println(w.parsecard(), HEX);

     w.resetdata();
     lcdClear();
   } else {
     setLCDColor(error);
     lcdWriteLine1("Waiting");
   } 
}

extern uint8_t* __brkval;
extern uint8_t __heap_start;
int freeRam () {
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}
