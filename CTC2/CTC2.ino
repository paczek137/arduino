#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <avr/interrupt.h>

//#define F_CPU 16000000UL // 16MHz 
#define BAUD 9600UL
#define BAUDRATE ((F_CPU)/(BAUD * 16UL)-1)

#define LED_1_DDR DDRB
#define LED_2_DDR DDRB
#define LED_3_DDR DDRB
#define LED_1_PORT PORTB
#define LED_2_PORT PORTB
#define LED_3_PORT PORTB
#define LED_1_PIN PORTB1
#define LED_2_PIN PORTB2
#define LED_3_PIN PORTB3



void uart_init(void);
void uart_transmit(unsigned char data);
void uart_transmit_str(char *data);
void timer0_init(void);
void leds_init(void);

unsigned char uart_receive(void);
bool start;
volatile uint8_t tot_overflow;
volatile uint8_t cnt;

int main(void) {
  
  uart_init();
  leds_init();
  timer0_init();
  sei();
  for(;;) {
    if(start) {
      uart_transmit_str("Hello world\r\n");
      start = 0;
    }
  }
}

void uart_init(void) {
  UBRR0H = (BAUDRATE >> 8);
  UBRR0L = BAUDRATE;
  UCSR0B |= (1<<RXEN0) | (1<<TXEN0) | (1<<RXCIE0);
  UCSR0C |= (1<<UCSZ01) | (1<<UCSZ00);
  start = 1;
}

void uart_transmit(unsigned char data) {
  while(!(UCSR0A & (1<<UDRE0))) { }
  UDR0 = (uint8_t)data;
}

unsigned char uart_receive(void) {
  while(!(UCSR0A & (1<<RXC0))){}
  return UDR0;
}

void uart_transmit_str(char *data) {
  while(*data) {
    uart_transmit(*data++);
  }
}

ISR(USART_RX_vect) {
  byte s = UDR0;
  
  if(s == 'h') {
    uart_transmit_str("Wyslano znak 'h'\r\n");
  } else {
    uart_transmit(s);
  } 
}

/*ISR(TIMER1_COMPA_vect) {
  if(cnt >= 10) {
    LED_1_PORT ^= (1<<LED_1_PIN);
    cnt = 0;
  } else cnt++;
}*/

void timer0_init(void) {
  TCCR1A |= (1<<COM1A0);
  TCCR1B |= (1<<WGM12) | (1<<CS11) | (1<<CS10);  // prescaler 64, mode CTS
  
  TCNT1 = 0;  // initialize counter
  
  OCR1A = 24999;  // initialize compare value
  
  //TIMSK1 |= (1<<OCIE1A);
  
  // 100ms
  
}

void leds_init(void) {
  LED_1_DDR |= (1<<LED_1_PIN);  // DDRB |= (1<<PORTB1)
  LED_2_DDR |= (1<<LED_2_PIN);  // makes 9,10,11 pins as Output
  LED_3_DDR |= (1<<LED_3_PIN);
}