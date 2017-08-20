#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdio.h>

#define F_CPU 16000000UL // 16MHz 
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

// PWM Ton=2ms Toff=8ms f=100Hz

void uart_init(void);
void uart_transmit(unsigned char data);
void uart_transmit_str(char *data);
void timer1_init(void);
void leds_init(void);
void cmd_help(char *arg);
void cmd_wrong_command(void);
void cmd_set_rgb(char *arg);
void cmd(char *s);
void rgb(uint8_t r, uint8_t g, uint8_t b);
uint8_t hex2dec(char c);
int uart_transmit_printf(char var, FILE *stream);

unsigned char uart_receive(void);
bool start;
volatile uint8_t tot_overflow;
volatile uint8_t cnt;
volatile uint16_t brightness;
volatile uint8_t rgb_brighteness[3];
byte buf[100] = {0};
int idx = 0;

static FILE mystdout = FDEV_SETUP_STREAM(uart_transmit_printf, NULL, _FDEV_SETUP_WRITE);

struct t_command {
  char *cmd;
  void (*function)(char *);  // void (*function)(void);
};

const struct t_command commands[] = {
  {"help", cmd_help},
  {"set_rgb", cmd_set_rgb},
  {NULL, NULL}  // important!
};



int main(void) {
  
  uart_init();
  stdout = &mystdout;
  leds_init();
  timer1_init();
  sei();
  for(;;) {
    if(start) {
      uart_transmit_str("Hello world\r\n");
      start = 0;
    }
    //OCR1A = 400;
    for(brightness = 0; brightness < 2499; ++brightness) {
      OCR1A = brightness;
      _delay_us(70);
    }
    for(brightness = 2499; brightness > 0; --brightness) {
      OCR1A = brightness;
      _delay_us(70);
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

int uart_transmit_printf(char var, FILE *stream) {
  if(var == '\n') {
    uart_transmit('\r');
  }
  uart_transmit(var);
  return 0;
}

ISR(USART_RX_vect) {
  byte s = UDR0;
  
  /*if(s == 'h') {
    uart_transmit_str("Wyslano znak 'h'\r\n");
  } else {
    uart_transmit(s);
  } */
  
  if(s >= ' ') {
    buf[idx] = s;
    idx++;
    buf[idx] = '\0';
  }
  if(idx < 99) {
    if(s == 13) {
      if(buf[0] != '\0') {
        cmd((char*)buf);
      }
      idx = 0;
      buf[0] = '\0';
    }
  } else {
    cmd_wrong_command();
    idx = 0;
    buf[0] = '\0';
  }
  
      
}

/*ISR(TIMER1_COMPA_vect) {
  cnt++;
  if(cnt %2 == 0) {
    OCR1A = 499;
    LED_1_PORT |= (1<<LED_1_PIN);
  } else {
    OCR1A = 1999;
    LED_1_PORT &= ~(1<<LED_1_PIN);
  }
}*/

void timer1_init(void) {
  TCCR1A |= (1<<COM1A0) | (1<<COM1A1) |(1<<WGM11);
  TCCR1B |= (1<<WGM12) | (1<<WGM13) | (1<<CS11) | (1<<CS10);  // prescaler 64, mode CTS
  
  TCNT1 = 0;  // initialize counter
  
  ICR1 = 2499;  // initialize compare value
  
  //OCR1A = 2400;
  
  //TIMSK1 |= (1<<OCIE1A);  // enable interrupt
  
    
}

void leds_init(void) {
  LED_1_DDR |= (1<<LED_1_PIN);  // DDRB |= (1<<PORTB1)
  LED_2_DDR |= (1<<LED_2_PIN);  // makes 9,10,11 pins as Output
  LED_3_DDR |= (1<<LED_3_PIN);
}

void cmd_help(char *arg) {
  uart_transmit_str(
  "help - pomoc\r\n"
  "leds_on - wlacza diody\r\n"
  "leds_off - wylacza diody\r\n");
}
void cmd_wrong_command() {
  uart_transmit_str(
  "Niepoprawna dlugosc komendy\r\n");
}

void cmd_set_rgb(char *arg){
  
  uint8_t re, gr, bl;
  rgb(re, gr, bl);
  
  uart_transmit_str(
  "ARGUMENT TO ");
  //uart_transmit_str(rgb_brighteness[0]);
}

void rgb(uint8_t r, uint8_t g, uint8_t b) {
  if(r>100) r = 100;
  if(g>100) g = 100;
  if(b>100) b = 100;
  rgb_brighteness[0] = (byte)(r*255/100);
  rgb_brighteness[1] = (byte)(g*255/100);
  rgb_brighteness[2] = (byte)(b*255/100);
}

uint8_t hex2dec(char c) {
  if(c > 64 && c < 71) return c - 55;
  else if(c >= 48 && c <= 57) return c - 48;
}

void cmd(char *s) {
  int i = 0;
  int len = 0;
  while(commands[i].cmd != NULL) {
    len = strlen(commands[i].cmd);
    if(strncmp(s, commands[i].cmd, len) == 0) {  // check only len characters to compare
      if(s[len] == ' ') {
        s += (len +1);  // move the indicator past to the empty space
        commands[i].function(s);  // call function and pass the indicator
        break;
      } else {
        commands[i].function(NULL);
        break;
      }
    }
    i++;
  }
}
