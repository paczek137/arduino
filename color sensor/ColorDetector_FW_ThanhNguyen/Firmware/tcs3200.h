/*******************************************************************************
Default Connection

      | ATmega32

OUT -> PA0
S2  -> PA1
S3  -> PA2
LED -> PA3
S0  -> PA4 (VCC)
S1  -> PA5 (VCC)
!OE -> PA6 (GND)



*******************************************************************************/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define DATA_DDR DDRA
//S2 Connection
#define TCS_S2_PORT PORTA
#define TCS_S2_POS PA1

//S3 Connection
#define TCS_S3_PORT PORTA
#define TCS_S3_POS PA2

//Out Connection
#define TCS_OUT_PORT PINA
#define TCS_OUT_POS PA0

//LED Connection
#define TCS_LED_PORT PORTA
#define TCS_LED_POS PA3



//S0 Connection
#define TCS_S0_PORT PORTA
#define TCS_S0_POS PA4

//S1 Connection
#define TCS_S1_PORT PORTA
#define TCS_S1_POS PA5

//OE Connection
#define TCS_OE_PORT PORTA
#define TCS_OE_POS PA6


#define	TCS_S0High()	TCS_S0_PORT|=(1<<TCS_S0_POS)
#define	TCS_S0Low()		TCS_S0_PORT&=~(1<<TCS_S0_POS)
#define	TCS_S1High()	TCS_S1_PORT|=(1<<TCS_S1_POS)
#define	TCS_S1Low()		TCS_S1_PORT&=~(1<<TCS_S1_POS)
#define	TCS_OEHigh()	TCS_OE_PORT|=(1<<TCS_OE_POS)
#define	TCS_OELow()		TCS_OE_PORT&=~(1<<TCS_OE_POS)

#define	TCS_S2High()	TCS_S2_PORT|=(1<<TCS_S2_POS)
#define	TCS_S2Low()		TCS_S2_PORT&=~(1<<TCS_S2_POS)
#define	TCS_S3High()	TCS_S3_PORT|=(1<<TCS_S3_POS)
#define	TCS_S3Low()		TCS_S3_PORT&=~(1<<TCS_S3_POS)
#define TCS_LEDOn()		TCS_LED_PORT|=(1<<TCS_LED_POS)
#define TCS_LEDOff() 	TCS_LED_PORT&=~(1<<TCS_LED_POS)
void TCS3200_init()
{
	//Initialize S0,S1,S2,S3,!OE,LED are output, OUT (PA0) is input
	DATA_DDR = 0x7E; 
	/*
		S0	S1	Output Scaling (f0)
		0	0	Power down
		0	1	2%
		1	0	20%
		1	1	100%
	*/
	TCS_S0Low();
	TCS_S1High();
	TCS_OELow();
}


void TCS_SelectRed()
{
	TCS_S2Low();
	TCS_S3Low();
}

void TCS_SelectGreen()
{
	TCS_S2High();
	TCS_S3High();
}

void TCS_SelectBlue()
{
	TCS_S2Low();
	TCS_S3High();
}

void TCS_SelectClear()
{
	TCS_S2High();
	TCS_S3Low();
}

unsigned int TCS_Measure()
{
	//If the function is entered when the level on OUT line was low
	//Then wait for it to become high.
	if(!(TCS_OUT_PORT & (1<<TCS_OUT_POS)))
	{
		while(!(TCS_OUT_PORT & (1<<TCS_OUT_POS)));	//Wait for rising edge	
	}
	
	
	while(TCS_OUT_PORT & (1<<TCS_OUT_POS));	//Wait for falling edge
	
	TCNT1=0x0000;		//Reset Counter
	
	TCCR1B=(1<<CS10);	//Prescaller = F_CPU/1 (Start Counting)
	
	while(!(TCS_OUT_PORT & (1<<TCS_OUT_POS)));	//Wait for rising edge
	
	//Stop Timer
	TCCR1B=0x00;
	
	return ((float)F_CPU/(2*TCNT1));
	
}
//read Red pulse
unsigned int Measure_R()		
{
	TCS_SelectRed();
	unsigned int r;
	
	_delay_ms(1);
	r=TCS_Measure();	
	
	_delay_ms(1);
	r+=TCS_Measure();	
	
	_delay_ms(1);
	r+=TCS_Measure();
	
	return r/3;
	
}

//read green pulse
unsigned int Measure_G()
{
	TCS_SelectGreen();
	unsigned int r;
	
	_delay_ms(1);
	r=TCS_Measure();	
	
	_delay_ms(1);
	r+=TCS_Measure();	
	
	_delay_ms(1);
	r+=TCS_Measure();
	
	return r/2.5;
	
}

//read blue pulse
unsigned int Measure_B()
{
	TCS_SelectBlue();
	unsigned int r;
	
	_delay_ms(1);
	r=TCS_Measure();	
	
	_delay_ms(1);
	r+=TCS_Measure();	
	
	_delay_ms(1);
	r+=TCS_Measure();
	
	return r/3;
	
}
unsigned int Measure_C()
{
	TCS_SelectClear();
	unsigned int r;
	
	_delay_ms(1);
	r=TCS_Measure();	
	
	_delay_ms(1);
	r+=TCS_Measure();	
	
	_delay_ms(1);
	r+=TCS_Measure();
	
	return r/3;	
}


