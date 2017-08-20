#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define LED_1_DDR DDRB
#define LED_1_PORT PORTB
#define LED_1_PIN PORTB1
#define LED_1_ON TCCR1A |= (1<<COM1A1);
#define LED_1_OFF TCCR1A &= ~(1<<COM1A1);


#define prog_0 230
#define prog_1 205
#define prog_2 180
#define prog_3 155
#define prog_4 130
#define prog_5 105
#define prog_6 80
#define prog_7 55
#define prog_8 30
#define prog_9 15

char LED_przelicz(char wartosc) {
  if(wartosc > prog_0) {
    return 255;
  } else if(wartosc > prog_1) {
    return 128;
  } else if(wartosc > prog_2) {
    return 80;
  } else if(wartosc > prog_3) {
    return 60;
  } else if(wartosc > prog_4) {
    return 50;
  } else if(wartosc > prog_5) {
    return 35;
  } else if(wartosc > prog_6) {
    return 20;
  } else if(wartosc > prog_7) {
    return 10;
  } else if(wartosc > prog_8) {
    return 6;
  } else if(wartosc > prog_9) {
    return 3;
  } else {
    return 0;
  }
}

int main(void) {
  
  char test = 0; 
  char up = 1;
  
  LED_1_DDR |= (1<<LED_1_PIN);
  
  TCCR1A |= (1<<COM1A1) | (1<<WGM10); 
  TCCR1B |= (1<<CS12) ; //preskaler 256
  
  
  while(1) {
     
    _delay_ms(100);
     
    if(up) {
      if(test < 240) {
        test += 15;
      } else {
        test = 255;
        up = 0;
      }
    } else {
      if(test > 15) {
        test -= 15;
      } else {
        test = 0;
        up = 1;
      }
    }
    
    OCR1A = LED_przelicz(test);
    //OCR1A = test;
  }
}

