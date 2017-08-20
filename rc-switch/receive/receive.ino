#include <RCSwitch.h>

/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO 
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN takes care 
  of use the correct LED pin whatever is the board used.
  If you want to know what pin the on-board LED is connected to on your Arduino model, check
  the Technical Specs of your board  at https://www.arduino.cc/en/Main/Products
  
  This example code is in the public domain.

  modified 8 May 2014
  by Scott Fitzgerald
  
  modified 2 Sep 2016
  by Arturo Guadalupi
*/

RCSwitch rc = RCSwitch();

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(13, OUTPUT);
  Serial.begin(9600);
  rc.enableReceive(0);
}

// the loop function runs over and over again forever
void loop() {

  if(rc.available())
  {
    digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
    int value = rc.getReceivedValue();

    if(value == 0)
    {
      Serial.print("Unknown encoding");
    } else
    {
      Serial.print("Received: ");
      Serial.print(rc.getReceivedValue());
      Serial.print(" / ");
      Serial.print(rc.getReceivedBitlength());
      Serial.print("bit ");
      Serial.print("Protocool: ");
      Serial.println(rc.getReceivedProtocol());
    }
    rc.resetAvailable();
  }
  delay(1000);                       // wait for a second
  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
}