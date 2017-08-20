/*    ClickButton

 Arduino library that works with 8-bit R2R Digital to Analog Converter
 based on 74HC595 Shift Register.

 Copyright (C) 2014 bartibv


 GNU GPLv3 license

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.



 Contact: bartibv@gmail.com


 History:

 2014.09.12 - First version. Basically just a small OOP programming exercise.
*/

#include "Arduino.h"
#include "DAC.h"

DAC::DAC(uint8_t latchPin,uint8_t clockPin,uint8_t dataPin)
{
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  latch_pin = latchPin;
  clock_pin = clockPin;
  data_pin  = dataPin;
  boolean upordown =1;
  unsigned long previousMicros =0;
  sineValue = 0;
}


void DAC::setVoltage(uint8_t setVoltageValue)
{
    digitalWrite(latch_pin, LOW);
    shiftOut(data_pin, clock_pin, MSBFIRST, setVoltageValue);
    digitalWrite(latch_pin, HIGH);
}

void DAC::preciseSineWave(float hertz, uint8_t sineMinValue, uint8_t sineMaxValue, int repeats)
{

    delayinterval = (1000000/hertz)/(sineMaxValue-sineMinValue+1);
    sineValue=sineMinValue;
    if(repeats > 0)
  {
    for(int i=1; i<=repeats; i++)
    {
       for(sineValue; sineValue<sineMaxValue; sineValue++)
       {
        digitalWrite(latch_pin, LOW);
        shiftOut(data_pin, clock_pin, MSBFIRST, sineValue);
        digitalWrite(latch_pin, HIGH);
       delayMicroseconds(delayinterval/2);
       }
       for(sineValue; sineValue>sineMinValue; sineValue--)
       {
        digitalWrite(latch_pin, LOW);
        shiftOut(data_pin, clock_pin, MSBFIRST, sineValue);
        digitalWrite(latch_pin, HIGH);
       delayMicroseconds(delayinterval/2);
       }
    }
  }
}


void DAC::infinitePreciseSineWave(float hertz, uint8_t sineMinValue, uint8_t sineMaxValue)
{

    delayinterval = (1000000/hertz)/(sineMaxValue-sineMinValue+1);
    sineValue=sineMinValue;

    while(1)
    {
       for(sineValue; sineValue<sineMaxValue; sineValue++)
       {
        digitalWrite(latch_pin, LOW);
        shiftOut(data_pin, clock_pin, MSBFIRST, sineValue);
        digitalWrite(latch_pin, HIGH);
       delayMicroseconds(delayinterval/2);
       }
       for(sineValue; sineValue>sineMinValue; sineValue--)
       {
        digitalWrite(latch_pin, LOW);
        shiftOut(data_pin, clock_pin, MSBFIRST, sineValue);
        digitalWrite(latch_pin, HIGH);
       delayMicroseconds(delayinterval/2);
       }
    }

}

void DAC::sineWave(float hertz)
{
    unsigned long currentMicros = micros();
    delayinterval = ((1000000/hertz)/256);




      if(currentMicros - previousMicros > delayinterval/2)
      {

         // save the last time you blinked the LED
         previousMicros = currentMicros;
          if(upordown==1)
          {
            digitalWrite(latch_pin, LOW);
            shiftOut(data_pin, clock_pin, MSBFIRST, sineValue);
            digitalWrite(latch_pin, HIGH);
            sineValue++;
          }
          else if(upordown==0)
          {
            digitalWrite(latch_pin, LOW);
            shiftOut(data_pin, clock_pin, MSBFIRST, sineValue);
            digitalWrite(latch_pin, HIGH);
            sineValue--;
          }

          if(sineValue==0)
         {
          upordown = 1;
         }
          else if(sineValue==255)
         {
          upordown = 0;
         }

       }

}
