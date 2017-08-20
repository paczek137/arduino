/**
 @file		mcu.c
 @brief 		functions to initialize MCU
 */
 
#include <avr/io.h>
#include <avr/interrupt.h>

#include "types.h"
#include "mcu.h"


#define	ATMEGA128_0WAIT		0
#define	ATMEGA128_1WAIT		1
#define	ATMEGA128_2WAIT		2
#define	ATMEGA128_3WAIT		3
#define 	ATMEGA128_NUM_WAIT	ATMEGA128_0WAIT


/**
 * @brief	Initialize MCU
 */ 
void mcu_init(void) 
{
	cli();
#ifndef __DEF_IINCHIP_INT__	
	EICRA=0x00;
	EICRB=0x00;
	EIMSK=0x00;
	EIFR=0x00;
#else
	EICRA = 0x00;			// External Interrupt Control Register A clear
	EICRB = 0x02;			// External Interrupt Control Register B clear // edge 
	EIMSK = (1 << INT4);		// External Interrupt Mask Register : 0x10
	EIFR = 0xFF;			// External Interrupt Flag Register all clear
	DDRE &= ~(1 << INT4);		// Set PE Direction 
	PORTE |= (1 << INT4);		// Set PE Default value
#endif

#if (ATMEGA128_NUM_WAIT == ATMEGA128_0WAIT)
	MCUCR = 0x80;		
	XMCRA=0x40;
#elif (ATMEGA128_NUM_WAIT == ATMEGA128_1WAIT)
	MCUCR = 0xc0;		// MCU control regiseter : enable external ram
	XMCRA=0x40;		// External Memory Control Register A : 
						// Low sector   : 0x1100 ~ 0x7FFF
						// Upper sector : 0x8000 ~ 0xFFFF
#elif (ATMEGA128_NUM_WAIT == ATMEGA128_2WAIT )
	MCUCR = 0x80;
	XMCRA=0x42;
#elif ((ATMEGA128_NUM_WAIT == ATMEGA128_3WAIT)
	MCUCR = 0xc0;
	XMCRA=0x42;
#else
#error "unknown atmega128 number wait type"
#endif	
	sei();				// enable interrupts
}


void mcu_soft_reset(void)
{
	asm volatile("jmp 0x0000");	
}
