#include "weigand.h"

/*********************************************************
 * SETUP
 *********************************************************/
//Constructor
Weigand::Weigand(int pin0, int pin1) {
  pinMode(pin0, OUTPUT);
  digitalWrite(pin0, HIGH);
  digitalWrite(pin0, LOW);
  pinMode(pin0, INPUT_PULLUP);
  
  pinMode(pin1, OUTPUT);
  digitalWrite(pin1, HIGH);
  digitalWrite(pin1, LOW);
  pinMode(pin1, INPUT_PULLUP);
}

//Destructor
Weigand::~Weigand() {
  
}

void Weigand::resetdata() {
  _count = 0;
  _data = 0;
  _time = 0;   
}

/*********************************************************
 * READING FROM RFID
 *********************************************************/
 
void Weigand::add0() {
  if(millis() - _d0time > 1) {
    Serial.print("0");
    _count++;
    
    _data = _data << 1;
    
    _time = millis();
    _d0time = _time;
  }
}

void Weigand::add1() {
  if(millis() - _d1time > 1) {
    Serial.print("1");
    _count++;
    
    _data = _data << 1;
    _data |= 1;      
    
    _time = millis();
    _d1time = _time;
  }
}

/*********************************************************
 * LOOP STUFF
 *********************************************************/
boolean Weigand::loop() {
  if(_time == 0) {
    return false;   
  } else {
    return ((millis() - _time) > 100);
  }
}

unsigned long Weigand::gettime() {
  return _time; 
}

unsigned int Weigand::getcount() {
  return _count; 
}

uint64_t Weigand::getdata() {
  return _data; 
}

/*********************************************************
 * PARSING CODES
 *********************************************************/
uint64_t Weigand::parsecard() {
  uint64_t tmp = _data;

  switch(_count) {
    case 26:    
      return _data + 137506062336LL;
      break;
  }
}
