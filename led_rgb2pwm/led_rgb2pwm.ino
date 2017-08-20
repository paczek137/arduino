int RedPin = 6;
int GreenPin = 10;
int BluePin = 9;

int redVal, blueVal, greenVal;

void setup() {
  
 pinMode(RedPin, OUTPUT);
 pinMode(BluePin, OUTPUT);
 pinMode(GreenPin, OUTPUT);
 digitalWrite(RedPin, HIGH);
 digitalWrite(BluePin, HIGH);
 digitalWrite(GreenPin, HIGH);
  
}

void loop() {

  redVal = 255;
  blueVal = 0;
  greenVal = 0;
  
  for(int i = 0; i < 255; i++) {
    greenVal++;
    redVal--;
    analogWrite(GreenPin, 255 - greenVal);
    analogWrite(RedPin, 255 - redVal);
    delay(20);
  }
  
  redVal = 0;
  blueVal = 0;
  greenVal = 255;
  
  for(int i = 0; i < 255; i++) {
    blueVal++;
    greenVal--;
    analogWrite(BluePin, 255 - blueVal);
    analogWrite(GreenPin, 255 - greenVal);
    delay(20);
  }
  
  redVal = 0;
  blueVal = 255;
  greenVal = 0;
  
  for(int i = 0; i < 255; i++) {
    redVal++;
    blueVal--;
    analogWrite(RedPin, 255 - redVal);
    analogWrite(BluePin, 255 - blueVal);
    delay(20);
  }
  
  
  
  
}
