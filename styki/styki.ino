#include <avr/io.h>
#include <util/delay.h>

#define LED_1_DDR DDRB
#define LED_2_DDR DDRB
#define LED_3_DDR DDRB
#define BUTTON_1_DDR DDRD
#define BUTTON_2_DDR DDRD

#define LED_1_PORT PORTB
#define LED_2_PORT PORTB
#define LED_3_PORT PORTB
#define BUTTON_1_PORT PORTD
#define BUTTON_2_PORT PORTD

#define LED_1_PIN PORTB1
#define LED_2_PIN PORTB2
#define LED_3_PIN PORTB3
#define BUTTON_1_PIN PORTD2
#define BUTTON_2_PIN PORTD3

#define BUTTON_1_PINx PIND
#define BUTTON_2_PINx PIND

int main(void) {
  
  LED_1_DDR |= (1<<LED_1_PIN);  // DDRB != (1<<PORTB1)
  LED_2_DDR |= (1<<LED_2_PIN);  // makes 9,10,11 pins as Output
  LED_3_DDR |= (1<<LED_3_PIN);
  
  BUTTON_1_DDR &= ~(1<<BUTTON_1_PIN);  // makes 2,3 pins as Input
  BUTTON_2_DDR &= ~(1<<BUTTON_2_PIN);
  
  //BUTTON_1_PORT |= (1<<BUTTON_1_PIN);
  
  while(1) {
    /*if(BUTTON_1_PINx & (1<<BUTTON_1_PIN)) {
      LED_1_PORT ^= (1<<LED_1_PIN);
      _delay_ms(300);
      LED_1_PORT ^= (1<<LED_1_PIN);
      _delay_ms(300);
    }*/
    
    if(PIND & (1<<PORTD2)) //If switch is pressed
    {
      LED_1_PORT |= (1<<LED_1_PIN);
    }
    else {
      LED_1_PORT &= ~(1<<LED_1_PIN);
    }
    
    
  }
  
}
