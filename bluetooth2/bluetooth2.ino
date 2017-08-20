#include <SoftwareSerial.h>

SoftwareSerial bt(4,2); // RX, TX
const int ledPin = 13;
int data;
int buttonState = 0;
int counter = 0;

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  bt.begin(9600);
}

void loop() {
  if(bt.available() > 0) {
    data = bt.read();
    if(data == '1') {
      digitalWrite(ledPin, HIGH);
    }
    else if(data == '0') {
      digitalWrite(ledPin, LOW);
    }
  
    Serial.println(data);
    bt.println(data);
  }
  counter++;
  Serial.print("Arduino counter: ");
  Serial.println(counter);
  delay(500);
  
}
