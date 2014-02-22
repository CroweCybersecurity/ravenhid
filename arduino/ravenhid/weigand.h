#include <Arduino.h>

class Weigand {
  public: 
    unsigned int facility;
    unsigned long card;
    
    Weigand(int pin0, int pin1);
    ~Weigand();
    
    void resetdata();
    void addbit(boolean type);

    boolean loop();
   
    unsigned int getcount();
    unsigned long getdata();
  private: 
    unsigned long _data;
    unsigned int _count;
    unsigned long _time;
    
    void prepPin(int pin);
    void parseCode();
};


