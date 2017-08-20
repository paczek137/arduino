#include <IRremote.h>
#define ir_pin 11

IRrecv irrecv(ir_pin);
decode_results results;

void setup() {
  Serial.begin(9600);
  irrecv.enableIRIn();
}

void loop() {
  if (irrecv.decode(&results)) {
    Serial.print("0x");
    Serial.println(results.value, HEX);
    delay(250);
    irrecv.resume();
  }
}
