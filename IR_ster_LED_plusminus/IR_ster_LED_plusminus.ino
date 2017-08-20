#include <IRremote.h>
#define ir_pin 11
#define dioda_pin 9

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
        
       case 0x20DF40BF:
         if (jasnosc < 255) {
           jasnosc = jasnosc+5;
         }
         Serial.println(jasnosc);
         analogWrite(dioda_pin, jasnosc);
         break;
         
        case 0x20DFC03F:
         if (jasnosc > 0) {
           jasnosc = jasnosc-5;
         }
         Serial.println(jasnosc);
         analogWrite(dioda_pin, jasnosc);
         break;
    }
    
    irrecv.resume();
  }
}
