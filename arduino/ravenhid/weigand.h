#include <Arduino.h>

class Weigand {
  public: 
    unsigned int facility;
    unsigned long card;
    
    Weigand(int pin0, int pin1);
    ~Weigand();
    
    void resetdata();
    void add0();
    void add1();
    
    uint64_t parsecard();

    boolean loop();
   
    unsigned int getcount();
    uint64_t getdata();
    unsigned long gettime();
  private: 
    uint64_t _data;
    unsigned int _count;
    unsigned long _time;
    
    unsigned int _d0time;
    unsigned int _d1time;
    
    void prepPin(int pin);

    
};


