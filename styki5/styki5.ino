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

uint8_t key1_lock, key2_lock;

void change_led1(void) {
  LED_1_PORT ^= (1<<LED_1_PIN);
}
void change_led2(void) {
  LED_2_PORT ^= (1<<LED_2_PIN);
}

void key_press(uint8_t *klock, volatile uint8_t *KPIN, uint8_t key_mask, void (*kfun)(void));
void key_push_up(uint8_t *klock, volatile uint8_t *KPIN, uint8_t key_mask, void (*kfun)(void));

int main(void) {
  
  LED_1_DDR |= (1<<LED_1_PIN);  // DDRB |= (1<<PORTB1)
  LED_2_DDR |= (1<<LED_2_PIN);  // makes 9,10,11 pins as Output
  LED_3_DDR |= (1<<LED_3_PIN);
  
  BUTTON_1_DDR &= ~(1<<BUTTON_1_PIN);  // makes 2,3 pins as Input
  BUTTON_2_DDR &= ~(1<<BUTTON_2_PIN);
  
  //BUTTON_1_PORT |= (1<<BUTTON_1_PIN);  // pull-up (not necessary)
  //BUTTON_2_PORT |= (1<<BUTTON_2_PIN);
  
  _delay_ms(10);
  
  while(1) {
    
    key_press(&key1_lock, &BUTTON_1_PINx, (1<<BUTTON_1_PIN), change_led1);
    key_push_up(&key2_lock, &BUTTON_2_PINx, (1<<BUTTON_2_PIN), change_led2);
  }
}

void key_press(uint8_t *klock, volatile uint8_t *KPIN, uint8_t key_mask, void (*kfun)(void)) {
  register uint8_t key_press = (*KPIN & key_mask);
  if(!*klock && key_press) {
    *klock = 1;
    
    if(kfun) {
      kfun();
    }
  } else if(*klock && !key_press) {
    (*klock)++;
  }
}

void key_push_up(uint8_t *klock, volatile uint8_t *KPIN, uint8_t key_mask, void (*kfun)(void)) {
  register uint8_t key_press = (*KPIN & key_mask);
  if(!*klock && key_press) {
    *klock = 1;
    
  } else if(*klock && !key_press) {
    if(!++*klock) {
      if(kfun) {
        kfun();
      }
    }
  }
}