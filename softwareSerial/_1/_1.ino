#include <SoftwareSerial.h>

SoftwareSerial ss(0,1);

void setup() {
  // put your setup code here, to run once:
  ss.begin(9600);
  ss.println("START:");
}

void loop() {
  // put your main code here, to run repeatedly:
  
  while(ss.available())
  {
    ss.print((char)ss.read());
  }
}
