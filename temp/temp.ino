#define lm35_pin A0

int lm35;
float temp;
unsigned long czas;

void setup() {
  Serial.begin(9600);
}

void loop() {
  czas = millis();
  
  lm35 = analogRead(lm35_pin);
  temp = (5.0 * lm35 * 100.0)/1024.0;
  
  Serial.print(czas/1000);
  Serial.print(" ");
  Serial.println(temp);
  
  delay(1000);
}
