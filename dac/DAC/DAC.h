#ifndef DAC_h
#define DAC_h

#include "Arduino.h"


class DAC
{
  public:
        DAC(uint8_t latchPin,uint8_t clockPin,uint8_t dataPin);
        void setVoltage(uint8_t);
        void preciseSineWave(float,uint8_t,uint8_t,int);
        void infinitePreciseSineWave(float, uint8_t, uint8_t);
        void sineWave(float);

  private:
        uint8_t latch_pin, clock_pin, data_pin,sineValue;
        boolean upordown;
        unsigned long previousMicros;
        int delayinterval;
};

#endif

