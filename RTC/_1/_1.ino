#include <Wire.h>

#define RTC_ADDR (0xA2 >> 1)

void setup() {
  // put your setup code here, to run once:
Wire.begin();
delay(1000);
Wire.beginTransmission(RTC_ADDR);
Wire.write(0x0D);
Wire.write(0x83);
Wire.endTransmission();
}

void loop() {
  // put your main code here, to run repeatedly:

}
