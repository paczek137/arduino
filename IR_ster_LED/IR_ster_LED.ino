#include <IRremote.h>
#define ir_pin 11
#define dioda_pin 13

int diodaStatus = LOW;
int jasnosc = 0;
IRrecv irrecv(ir_pin);
decode_results results;

void setup() {
  Serial.begin(9600);
  irrecv.enableIRIn();
  
  pinMode(dioda_pin, OUTPUT);
}

void loop() {
  if (irrecv.decode(&results)) {
    
    switch (results.value) {
      case 0x20DF22DD:
        Serial.println("OK");
        diodaStatus = ~diodaStatus;
        digitalWrite(dioda_pin, diodaStatus);
        delay(250);
        break;
    }
    
    irrecv.resume();
  }
}
