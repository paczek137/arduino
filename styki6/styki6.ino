#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>

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

typedef struct {
  volatile uint8_t *KPIN;
  uint8_t key_mask;
  uint8_t wait_time_s;
  void (*kfun1)(void);
  void (*kfun2)(void);
  uint8_t klock;
  uint8_t flag;
} TBUTTON;

volatile uint16_t Timer1, Timer2;

void change_led1(void) {
  LED_1_PORT ^= (1<<LED_1_PIN);
}
void change_led2(void) {
  LED_2_PORT ^= (1<<LED_2_PIN);
}

void key_press(TBUTTON *btn);

TBUTTON button;

int main(void) {
  
  LED_1_DDR |= (1<<LED_1_PIN);  // DDRB |= (1<<PORTB1)
  LED_2_DDR |= (1<<LED_2_PIN);  // makes 9,10,11 pins as Output
  LED_3_DDR |= (1<<LED_3_PIN);
  
  BUTTON_1_DDR &= ~(1<<BUTTON_1_PIN);  // makes 2,3 pins as Input
  BUTTON_2_DDR &= ~(1<<BUTTON_2_PIN);
  
  //BUTTON_1_PORT |= (1<<BUTTON_1_PIN);  // pull-up (dla odwrotnego połączenie)
  //BUTTON_2_PORT |= (1<<BUTTON_2_PIN);
  
  _delay_ms(10);
  
  TCCR2A |= (1<<WGM21); // CTC
  TCCR2B |= (1<<CS22)|(1<<CS21)|(1<<CS20);  // preskaler 1024
  OCR2A = 108;  // interrupt every 10ms (100Hz)
  TIMSK2 = (1<<OCIE2B);  // unlock CompareMatch interrupt
  
  button.KPIN = &BUTTON_1_PINx;
  button.key_mask = (1<<BUTTON_1_PIN);
  button.wait_time_s = 3;
  button.kfun1 = change_led1;
  button.kfun2 = change_led2;
  
  sei();
  
  while(1) {
    
    key_press(&button);
    if(!Timer2) {
      Timer2 = 50;
      LED_3_PORT ^= (1<<LED_3_PIN);
    }
  }
}

void key_press(TBUTTON *btn) {
  
  register uint8_t key_press = (*btn->KPIN & btn->key_mask);
  
  if(!btn->klock && key_press) {
    btn->klock = 1;
    
    // reaction for fast press button
    if(btn->kfun1) {
      btn->kfun1();
    }
    btn->flag = 1;
    Timer1 = (btn->wait_time_s * 1000)/10;
  } else if(btn->klock && !key_press) {
    (btn->klock)++;
    if(!btn->klock) {
      Timer1 = 0;
      btn->flag = 0;
    }
  } else if(btn->flag && !Timer1) {
    // reaction for long press button
    if(btn->kfun2) {
      btn->kfun2();
    }
    btn->flag = 0;
  }
}

ISR(TIMER2_COMP_vect) {
  uint16_t n;
  n = Timer1;  // 100Hz Timer1
  if(n) {
    Timer1 = --n;
  }
  n = Timer2;  // 100Hz Timer2
  if(n) {
    Timer2 = --n;
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
