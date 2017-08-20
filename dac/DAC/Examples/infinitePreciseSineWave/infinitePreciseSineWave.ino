//**************************************************************//
//  Name    : Biblioteka ShiftRegister R2R DAC                  //
//  Author  : Bartłomiej Witek                                  //
//  Date    : 13 Sep, 2014                                      //
//  Version : 1.0                                               //
//  Notes   : Code for using a 74HC595 Shift Register with R2R  //
//          : Digital to Analog Converter                       //
//          : This code creates infinite sine wave              //
//**************************************************************//
//                  Infinite Precise SineWave                   //
//**************************************************************//
#include <DAC.h>
// DAC name_of_object(latchPin,clockPin,dataPin);
// latchPin connected to ST_CP of 74HC595
// clockPin connected to SH_CP of 74HC595
// dataPin connected to DS of 74HC595

DAC dac(8,12,11);

void setup()
{
 //nothing to do here
}

void loop()
{
  //name.preciseSineWave(frequencyInHz, LowestVoltage, HighestVoltage);
  //note that method "preciseSineWave" uses delayMicroseconds and for loops and stops the program
  //from doing anything else
 dac.infinitePreciseSineWave(0.5, 30,250);
 //and long delay after all
 delay(10000)
}
