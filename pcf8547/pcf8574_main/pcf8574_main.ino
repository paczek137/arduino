#include <Wire.h>
#include "PCF8574.h"

PCF8574 expander;

void onInterrupt() {
  Serial.println("Interrupt");
  expander.checkForInterrupt();
}

void onPin0() {
  Serial.println("Pin 0");
}

void onPin1() {
  Serial.println("Pin 1");
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  expander.begin(0x20);
  expander.pinMode(2, OUTPUT);
  //expander.pinMode(0, INPUT);
  //expander.pinMode(1, INPUT);
  //expander.pullDown(1);
  
  expander.digitalWrite(2, LOW);
  delay(100);
  expander.digitalWrite(2, HIGH);
  delay(100);
  expander.digitalWrite(2, LOW);
  delay(100);
  expander.digitalWrite(2, HIGH);
  delay(100);
  
  expander.enableInterrupt(2, onInterrupt);
  expander.attachInterrupt(0, onPin0, FALLING);
  expander.attachInterrupt(1, onPin1, FALLING);
}

void loop() {
  // put your main code here, to run repeatedly:
  //byte value = expander.digitalRead(1);
  //Serial.println(value, DEC);
  //delay(2000);
  
  
}
