int RedPin = 8;
int GreenPin = 10;
int BluePin = 9;

void setup() {
  
 pinMode(RedPin, OUTPUT);
 pinMode(BluePin, OUTPUT);
 pinMode(GreenPin, OUTPUT);
 digitalWrite(RedPin, HIGH);
 digitalWrite(BluePin, HIGH);
 digitalWrite(GreenPin, HIGH);
  
}

void loop() {

  digitalWrite(GreenPin, LOW);
  delay(1000);
  digitalWrite(GreenPin,HIGH);
  digitalWrite(BluePin,LOW);
  delay(1000);
  digitalWrite(BluePin,HIGH);
  digitalWrite(RedPin,LOW);
  delay(1000);
  digitalWrite(RedPin,HIGH);
  
}
