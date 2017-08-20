#define led 9

void setup() {
  pinMode(led, OUTPUT);
  
  noInterrupts();
  TCCR1A = 0;
  TCCR1B = 0;
  
  TCNT1 = 34286; // preload timer 65536-16Mhz/256/2Hz
  TCCR1B |= (1<<CS12);
  TIMSK1 |= (1<<TOIE1); //overflow interrupt
  interrupts();
  
}

ISR(TIMER1_OVF_vect) {
  TCNT1 = 34286;
  digitalWrite(led, digitalRead(led) ^ 1);
}

void loop() {
  
}
