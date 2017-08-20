
const int ledPin = 13;
int data;
int buttonState = 0;
int counter = 0;

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  if(Serial.available() > 0) {
    data = Serial.read();
    if(data == '1') {
      digitalWrite(ledPin, HIGH);
    }
    else if(data == '0') {
      digitalWrite(ledPin, LOW);
    }
  
    Serial.println(data);
  }
  counter++;
  Serial.print("Arduino counter: ");
  Serial.println(counter);
  delay(500);
  
}
