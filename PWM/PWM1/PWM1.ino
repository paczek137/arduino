#include <avr/io.h>
#include <util/delay.h>

#define LED_1_DDR DDRB
#define LED_1_PORT PORTB
#define LED_1_PIN PORTB1


int main(void) {
  LED_1_DDR |= (1<<LED_1_PIN);
  
  while(1) {
     LED_1_PORT ^= (1<<LED_1_PIN);
     _delay_ms(2500);
  }
}

