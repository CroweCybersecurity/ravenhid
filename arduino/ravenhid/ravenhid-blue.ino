#include <SD.h>
#include <LiquidCrystal.h>
#include <SoftPWM.h>
#include "lcd.h"
#include "weigand.h"
#include <Arduino.h>
#include <AltSoftSerial.h>

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

#define SD_CS 10
 
AltSoftSerial BLEMini; 


unsigned char buf[21] = {0};
unsigned char len = 0;

Weigand w(RFID_D0, RFID_D1);
LiquidCrystal lcd(LCD_RS, LCD_ENABLE, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

boolean startupError = false;

void readD0() {
  w.add0();
}

void readD1() {
  w.add1();
}

void setup() {
  Serial.begin(57600);
  Serial.println(F("[*] Setting up..."));
  
  BLEMini.begin(57600);
  
  lcd.begin(16, 2);
  lcdClear();
  lcdWriteLine1("Starting up...");
   
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(A4, OUTPUT);
  SoftPWMBegin();

  attachInterrupt(0, readD0, FALLING);
  attachInterrupt(1, readD1, FALLING);

  w.resetdata();

  pinMode(SD_CS, OUTPUT);
  if(!SD.begin(SD_CS)) {
    Serial.println("[!] SD card failed or not present.");
    setLCDColor(error);
    lcdClear();
    lcdWriteLine1("No SD Card Found");
    startupError = true;
  }
  
  if(startupError) {
    delay(5000); //Give our users a chance to read the error instead of coloring the rainbow.
  } else {
    lcdRainbow(); // Wait for a little bit to let the data lines settle. Since we've got time, lets show off.
  }
  
  lcdClear();
  Serial.println("[*] Setup complete.");
}

void loop() {
  if(w.loop()) {
     Serial.println("");
     setLCDColor(green);
     lcdWriteCard(w.getcount(), w.parsecard());
     sdWriteCard();
	 sendBluetooth();
	 
     delay(2000);
     w.resetdata();
     lcdClear();
   } else {
     setLCDColor(info);
     lcdWriteLine1("Hunting...");
     lcd.setCursor(0,1);
     lcd.print(freeRam());
     lcd.print(" b");
   }
}

void sdWriteCard() {
  File csvFile = SD.open("cards.csv", FILE_WRITE);
  if(csvFile) {
    uint64_t card = w.parsecard();
    csvFile.print(w.getcount());
    csvFile.print(",");
    csvFile.print(card, BIN);
    csvFile.print(",");
    csvFile.print(card, HEX);
    csvFile.println("");
    csvFile.close();
    
    Serial.print("[*] Got data: '");
    Serial.print(w.getcount());
    Serial.print(",");
    Serial.print(card, BIN);
    Serial.print(",");
    Serial.print(card, HX);
    Serial.println("'");
  } else {
    lcdClear();
    setLCDColor(error);
    lcdWriteLine1("Error writing.");
    lcdWriteLine2("to file.");
  }
}

void sendBluetooth() {
	if (BLEMini.available()) {
		char buff[21];
		// not sure which of the two lines below will work
		sprintf(buff, "%" PRIu64, card);
		snprintf (buff, sizeof(buf)-1, "%llu", (unsigned long long) card);
		
		len = sizeof(buff);
		BLEMini.write();	
		for (int i = 0; i < len; i++)
         BLEMini.write(buff[i]);
      len = 0;
	}
}

extern uint8_t* __brkval;
extern uint8_t __heap_start;
int freeRam () {
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}
