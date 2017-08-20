#include <Wire.h>

#define RTC_ADDR (0xA2 >> 1)


unsigned char time_date_raw[7];  // stores time/date from RTC
int index = 0;  //  index into above array
char seconds[] = "00";  //  stores seconds string

void setup() {
  // put your setup code here, to run once:
Wire.begin();
delay(1000);
Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:

Wire.beginTransmission(RTC_ADDR);
Wire.write(0x02);
Wire.endTransmission();

Wire.requestFrom(RTC_ADDR, 7);
while(Wire.available()) {
  time_date_raw[index] = Wire.read();
  index++;
  if(index >= 7) {
    index = 0;
    break;
  }
}

Wire.endTransmission();

delay(300);
//  convert seconds to string
seconds[0] = ((time_date_raw[0] >> 4) & 0x07) + '0';
seconds[1] = (time_date_raw[0]  & 0x0F) + '0';
Serial.print("DEC: ");
Serial.println(time_date_raw[0]);
Serial.println(seconds);

}
