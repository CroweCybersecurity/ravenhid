#include "weigand.h"

/*********************************************************
 * SETUP
 *********************************************************/
//Constructor
Weigand::Weigand(int pin0, int pin1) {
  resetdata();

  prepPin(pin0);
  prepPin(pin1);
  
  //Delay a bit to let things settle out.
  delay(10);

}

//Destructor
Weigand::~Weigand() {
  
}

void Weigand::prepPin(int pin) {  
  pinMode(pin, OUTPUT);
  digitalWrite(pin, HIGH);
  digitalWrite(pin, LOW);
  pinMode(pin, INPUT);
  pinMode(pin, HIGH);
}

void Weigand::resetdata() {
  _count = 0;
  _data = 0;
  _time = 0;   
}

/*********************************************************
 * READING FROM RFID
 *********************************************************/
void Weigand::addbit(boolean type) {
  _count++;
  _data = _data << 1;
  _time = millis();
  
  if(!type) {
    _data |= 1;
  }
}


/*********************************************************
 * LOOP STUFF
 *********************************************************/
boolean Weigand::loop() {
  return ((millis() - _time) > 100);
}

unsigned int Weigand::getcount() {
  return _count; 
}

unsigned long Weigand::getdata() {
  return _data; 
}

/*********************************************************
 * PARSING CODES
 *********************************************************/
/*void Weigand::parseCode() {
  //0x1FFFFFE = 1111111111111111111111110
  //Remove parity bits
  card = (_data &  0x1FFFFFE) >> 1;
}*/
