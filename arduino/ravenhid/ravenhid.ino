#include <LiquidCrystal.h>
#include <SoftPWM.h>
#include "status.h"

#define LCD_BLUE A4
#define LCD_GREEN A3
#define LCD_RED A2
#define LCD_RS A0
#define LCD_ENABLE A1
#define LCD_D4 8
#define LCD_D5 7
#define LCD_D6 6
#define LCD_D7 5

LiquidCrystal lcd(LCD_RS, LCD_ENABLE, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

void setLCDColor(lcdstatus s) {
  switch(s) {
  case error: 
    lcdWriteColor(0xff, 0x00, 0x00);
    break;   
  case warning: 
    lcdWriteColor(0xff, 0x22, 0x00);
    break;    
  case info: 
    lcdWriteColor(0x00, 0xff, 0xff);
    break;    
  default: 
    lcdWriteColor(0xff, 0xff, 0xff);
  }
}

void lcdWriteColor(int r, int g, int b) {
  SoftPWMSet(LCD_RED, map(r, 0, 255, 255, 0)); 
  SoftPWMSet(LCD_GREEN, map(g, 0, 255, 255, 0));
  SoftPWMSet(LCD_BLUE, map(b, 0, 255, 255, 0));
}

void lcdWriteLine1(char text[]) {
  lcd.setCursor(0, 0);
  lcd.print(text); 
}

void lcdWriteLine2(char text[]) {
  lcd.setCursor(0, 1);
  lcd.print(text); 
}

void lcdClear() {
  lcd.clear(); 
}

void setup() {
  lcd.begin(16, 2);
  lcd.print("Starting up...");

  SoftPWMBegin();
  
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(A4, OUTPUT);
  
  lcdClear();
}

void loop() {
  lcdWriteLine1("Warning");
  setLCDColor(warning);
  delay(1000);
  lcdWriteLine1("Error");
  setLCDColor(error);
  delay(1000);
  lcdWriteLine1("Info");
  setLCDColor(info);
  delay(1000);
  lcdWriteLine1("Normal");
  setLCDColor(normal);  
  delay(1000);
}


