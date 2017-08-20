
int czujnik = A1;
float VOLT;
float TEMP;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Test czujnika temperatury");
}

void loop() {
  // put your main code here, to run repeatedly:
  int odczyt = analogRead(czujnik);
  VOLT = (odczyt * 5.0) / 1024.0;
  TEMP = VOLT * 100;
  Serial.print("Temperatura (C): ");
  Serial.println(TEMP);
  
  delay(5000);
}
