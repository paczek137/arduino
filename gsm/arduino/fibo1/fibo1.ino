#include <SoftwareSerial.h>

SoftwareSerial ss(12, 8);

void setup() {
  // put your setup code here, to run once:
  ss.begin(9600);
  Serial.begin(9600);
  Serial.println("------");
}

void loop() {
  // put your main code here, to run repeatedly:

  //ss.println("AT");
  while(ss.available())
  {
    Serial.print((char)ss.read());
  }
  while(Serial.available())
  {
    ss.print((char)Serial.read());
  }
  //delay(1000);

}
