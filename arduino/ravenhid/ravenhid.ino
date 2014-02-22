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

#define RFID_D0 3
#define RFID_D1 4

#define READER_DELAY 500 // How many ms do we wait before we call it read

LiquidCrystal lcd(LCD_RS, LCD_ENABLE, LCD_D4, LCD_D5, LCD_D6, LCD_D7);
Weigand w(RFID_D0, RFID_D1);

void readD0() {
  w.addbit(0);
}  

void readD1() {
  w.addbit(1);
}

void setup() {
  lcd.begin(16, 2);
  lcd.print("Starting up...");

  SoftPWMBegin();
  
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(A4, OUTPUT);
  
  lcdClear();
  
  attachInterrupt(0, readD0, FALLING);
  attachInterrupt(1, readD1, FALLING);
}

void loop() {
  if(w.loop()) {
     setLCDColor(green);
     lcdWriteLine1("Got a card!");
     Serial.print(F("Card bytes: "));
     Serial.println(w.getcount());
     Serial.print(F("Card data (HEX): "));
     Serial.println(w.getdata(), HEX);
     Serial.print(F("Card data (DEC): "));
     Serial.println(w.getdata(), DEC);
   }
}


