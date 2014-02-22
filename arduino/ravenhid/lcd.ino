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
  case green:
    lcdWriteColor(0x00, 0xff, 0x00);
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
