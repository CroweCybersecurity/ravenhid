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

void lcdRainbow() {
  unsigned int rgbColor[3];
   
  // Start off with red.
  rgbColor[0] = 255;
  rgbColor[1] = 0;
  rgbColor[2] = 0;
   
  // Choose the colours to increment and decrement.
  for (int decColor = 0; decColor < 3; decColor += 1) {
    int incColor = decColor == 2 ? 0 : decColor + 1;
     
    // cross-fade the two colours.
    for(int i = 0; i < 255; i += 1) {
      rgbColor[decColor] -= 1;
      rgbColor[incColor] += 1;
      SoftPWMSet(LCD_RED, rgbColor[0]); 
      SoftPWMSet(LCD_GREEN, rgbColor[1]);
      SoftPWMSet(LCD_BLUE, rgbColor[2]);
      delay(5);
    }
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

void lcdWriteCard(unsigned int bits, uint64_t card) {
  lcdClear();
  
  lcd.setCursor(0, 0); 
  lcd.print(F("Size: "));
  lcd.print(bits);
  
  lcd.setCursor(0, 1);
  lcd.print(card, HEX);
}

void lcdClear() {
  lcd.clear(); 
}
