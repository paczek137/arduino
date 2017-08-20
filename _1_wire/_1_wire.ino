#include <DallasTemperature.h>
#include <OneWire.h>

#define ONE_WIRE_BUS 7
// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
int led = 13;

OneWire oneWire(ONE_WIRE_BUS);

DallasTemperature sensors(&oneWire);

// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);     
  Serial.begin(9600);
  sensors.begin();
}

// the loop routine runs over and over again forever:
void loop() {
  sensors.requestTemperatures();
  Serial.println("");
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);               // wait for a second
  Serial.print("Sensor 1: ");
  Serial.println(sensors.getTempCByIndex(0));
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);               // wait for a second
}
