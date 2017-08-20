  /**********************************************
  *	Writen by: Nguyen Xuan Thanh - EEIU09053	*
  *	school of Electrical Engineering			*
  *				-------							*
  *	Project: Colour Sensor TCS3200				*
  *												*
  ***********************************************
  HARDWARE
  --------
  MCU = ATmega32 Running at 8MHz Crystal
  
  DISPLAY = 128x64 GLCD Module
  *************************
  
  *define the data port (D7:0)
  GLCD_DATA_O  		PORTD  // Ouput Data bus
  GLCD_DATA_I	  	PIND   // Input Data bus
  GLCD_DATA_DDR  	DDRD   // Direction

  *define the control port (RS, RW, E, CS1, CS2)
  GLCD_CTRL_O		PORTB 
  GLCD_CTRL_I 		PINB 
  GLCD_CTRL_DDR 	DDRB
  *GLCD control pins
  GLCD_E	 		1// Enable line
  GLCD_RW 			2 // read/write selecting line
  GLCD_RS 			3 // Register select: Data/Instruction (DI)
  GLCD_CS1        	5 // left/right selecting line#1
  GLCD_CS2        	4 // left/right selecting line#2
  
  TCS3200 Module
  **************
  
      | ATmega32

	OUT -> PA0
	S2  -> PA1
	S3  -> PA2
	LED -> PA3
	S0  -> PA4 
	S1  -> PA5 
	!OE -> PA6 

  
  BUZZER -> C0
  
********************************************************/
#define F_CPU	8000000ul		//define CPU frequency
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "myGLCD.h"
#include "tcs3200.h"

#define Buzzer	PORTC			//Buzzer port

unsigned char count=0;  //global variable to count for genarating 1kHz pulse
char color=0,color_pre=0;
void Clear();


ISR (TIMER0_OVF_vect){
	TCNT0=-100;
	count++;
	if (count==5)
		Buzzer ^=0x02;		//create pulse 1kHz at PC1
}

int main(void)
{
	//declare variable
	unsigned int r=0,g=0,b=0,_r,_g,_b,wb=0,c=0;	
	unsigned char stt=0,pre=0;
    //Initialize the GLCD Library
    GLCD_Init();
	GLCD_Clr();	

	//Put my bitmap to GLCD
	
	GLCD_PutBMP(hiGLCD);	
	_delay_ms(3000);
	
    GLCD_Clr();				//clear GLCD
	//Initialize TCS Library
	TCS3200_init();			
	
	//Initialize Timer0 
	TIMSK= (1<<TOIE0)|(1<<TOIE1);	//enable timer0,timer1
	
	TCNT0=-100;			//recalculate if crystal is changes
	/* 1kHz => T = 1ms -> half pulse is 0.5ms
	TCNT0 = 100 => 100*8/8MHz = 0.1ms 
	--> 0.5ms = 5* 0.1ms
	*/
	//Buzzer Pin PC1 as output and Button pin PC0 as input and pullup
	PORTC = 0x03;	
	DDRC=~(1<<PC0)|(1<<1);			
	sei();					//enable global interrupts
	while(1)
	{		
		_delay_ms(500);		

		TCS_LEDOn();		//turn on LED
		_r=Measure_R();		//measure frequency with red filter
		_g=Measure_G();		//measure frequency with green filter
		_b=Measure_B();		//measure frequency with blue filter	

		c=Measure_C();		//measure frequency without filter
		TCS_LEDOff();		//turn off LED	

		//Find the smallest value in RGB -> scale the RGB value to determine color
		if (c>2100){
			
			if(_r<_b)
			{
				if(_r<_g){
					wb=_r;
				}
				else
					wb=_g;
			}
			else
			{
				if(_b<_g)
					wb=_b;
				else
					wb=_g;	
			}	
			wb=wb/10;
			r=_r/wb;	
			g=_g/wb;
			b=_b/wb;

			if ((wb>250)&(r==10)){		//determine White color
				color = 1;
				Clear();
				GLCD_Print78(2,0,"Color is White");
				pre=0;
			}
			else if ((r>14)&(g>b)&(b==10)){			//determine Yellow
				color = 2;
				Clear();
				GLCD_Print78(2,0,"Color is Yellow");
				pre=0;
			}
			else if ((b>g)&(b>=15)&(r==10)){	//determine Blue
				color = 3;
				Clear();
				GLCD_Print78(2,5,"Color is Blue");
				pre=0;
			}
			else if ((r>b)&(g==10)&(b==10)){	//determine Red
				color = 4;
				Clear();
				GLCD_Print78(2,10,"Color is Red");
				pre=0;		
			}
			else if ((g>b)&(g>r)&(_r>80)){		//determine Green
				color = 5;
				Clear();
				GLCD_Print78(2,0,"Color is Green");
				pre=0;
			}
			else if ((b>g)&(r==10)&(g==11)&(wb<150)){  //determine purple
				color = 6;
				Clear();
				GLCD_Print78(2,0,"Color is Purple ");
				pre=0;
			}
			else if ((r==10)&(g==10)&(b==10)&(_r<1000)&(_b<1000)&(wb>82)){	//determine black
				color = 7;
				Clear();
				GLCD_Print78(0,5,"Color is Black");
				_delay_ms(100);
				if ((stt==0)&(pre==0)) stt=1;
				else if (stt==2){
					GLCD_Print78(2,1," Buzzer is OFF");
				}
			}

			else {
				color = 8;
				Clear();
				GLCD_Print78(0,0,"Oooppp!!!");
				GLCD_Print78(2,10,"Unknown color");
				pre=0;
				}
			if (stt==1){		//if color is black,enable timer0 and 1KHz to buzzer
				TCCR0=0x02;		//TIMER0 INTERNAL CLOCK, PRESCALER/8
				pre=1;
				GLCD_Print78(4,1," Buzzer is ON");
			}
		}
		else
		{
			color=9;
			Clear();
			GLCD_Print78(0,10,"No Object");	
			pre=0;
		}
		if ((pre==0 )&(stt==2)) stt = 0;	//reset status of buzzer plays when color is black
		if (bit_is_clear(PINC,0)&(stt==1)) {		//stop buzzer if button is pressed
			stt =2;
			TCCR0=0;		//stop timer0
			PORTC=(1<<1)|(1<<0);
			GLCD_Clr();
			
		} 
		
		/*
		GLCD_PutNum(4,10,r);
		GLCD_PutNum(4,45,g);
		GLCD_PutNum(4,80,b);
		GLCD_PutNum(6,10,_r);
		GLCD_PutNum(6,45,_g);
		GLCD_PutNum(6,80,_b);
		GLCD_PutNum(7,0,c);
		GLCD_PutNum(7,50,wb);
		*/
	}
}

/*This function is used to check the status of color change to clear the screen
if color is not changed, the graphic LCD will not clear
if color is change, it will call GLCD_Clr() to clear all screen
pre_color and color are the variable to check
*/
void Clear()
{
	if (color!= color_pre){
		GLCD_Clr();
		color_pre=color;
	}
}


