#include <avr/io.h>
#include <util/delay.h>

#define LED_1_DDR DDRB
#define LED_1_PORT PORTB
#define LED_1_PIN PORTB1
#define LED_1_ON TCCR1A |= (1<<COM1A1);
#define LED_1_OFF TCCR1A &= ~(1<<COM1A1);
#define PWM_MIN 10


int main(void) {
  LED_1_DDR |= (1<<LED_1_PIN);
  
  TCCR1A |= (1<<WGM10) | (1<<COM1A1);
  TCCR1B |= (1<<WGM12) | (1<<CS10);
  OCR1A = PWM_MIN;
  
  while(1) {
     LED_1_ON
     _delay_ms(1000);
     LED_1_OFF
     //_delay_ms(100);
     OCR1A = 255;
     LED_1_ON
     _delay_ms(1000);
     OCR1A = PWM_MIN;
  }
}
