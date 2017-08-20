#define strobo 3
#define onTime 100

unsigned StroboFlash(unsigned long ontime = 100, unsigned long offTime = 10000) {
  float czasBlysku = (ontime + offTime) / 1000000.0;
  float czest = 1 / czasBlysku;
  bool bigOffDelay = false;
  if(offTime >= 100000) {
    offTime = offTime / 1000;
    bigOffDelay = true;
  }
  bool bigOnDelay = false;
  if(ontime >= 100000) {
    ontime = ontime / 1000;
    bigOnDelay = true;
  }
  digitalWrite(strobo, HIGH);
  if(bigOnDelay) {
    delay(onTime);
  } else {
    delayMicroseconds(onTime);
  }
  digitalWrite(strobo, LOW);
  if(bigOffDelay) {
    delay(offTime);
  } else {
    delayMicroseconds(offTime);
  }
  return czest;
}

void StroboFlash(float Czest = 10.0f) {
 if(Czest > 100) {
  digitalWrite(strobo, HIGH);
  delayMicroseconds(onTime);
  digitalWrite(strobo, LOW);
  delayMicroseconds((1000000.0f - onTime) / Czest);
 } else {
  digitalWrite(strobo, HIGH);
  delayMicroseconds(onTime);
  digitalWrite(strobo, LOW);
  delay((1000 / Czest));
 }
}

void setup() {
  Serial.begin(9600);
  pinMode(strobo, OUTPUT);
}

void loop() {
  static int oldAnalog;
  int analog = analogRead(A0);
  float Czest = StroboFlash(onTime, analog * 15);
  
  if(abs(oldAnalog - analog) > 0) {
    oldAnalog = analog;
    Serial.print("Częstotliwość błysków [Hz]: ");
    Serial.println(Czest);
  }
}
