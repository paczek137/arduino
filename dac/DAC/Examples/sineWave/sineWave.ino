//**************************************************************//
//  Name    : Biblioteka ShiftRegister R2R DAC                  //
//  Author  : Bartłomiej Witek                                  //
//  Date    : 13 Sep, 2014                                      //
//  Version : 1.0                                               //
//  Notes   : Code for using a 74HC595 Shift Register with R2R  //
//          : Digital to Analog Converter                       //
//          : This code creates a sine wave that can run        //
//          : with other code at the same time                  //
//**************************************************************//
//                   SineWave                                   //
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
//name.sineWave(frequencyInHz);
  dac.sineWave(0.5);
//this sine wave can run with other commands at the same time,
//but correcrntess of the sine wave depends on how long is Your
//code and how much time it takes to make another circle of "void loop()"
}
