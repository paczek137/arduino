/*
*
@file		timer.c
*
*/

#include <stdio.h> 
#include <string.h>
#include <avr/io.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>

#include "types.h"
#include "serial.h"
#include "socket.h"
#include "myprintf.h"

#include "timer.h"

#define TCNT0_VALUE	0xD7		/**< 5ms at 8MHz */
#define TCNT2_VALUE	0x87		/**< 15ms at 8MHz */


#if (MAX_TIMER0_CNT > 0)
static int sys_tick_5ms;
#endif

#if (MAX_TIMER2_CNT > 0)
static int sys_tick_1s;		/**< Tick count */
#endif


/**
 @brief	Timer handler table 
*/
struct _TIMER_IRQS
{
	TIMER_IRQ timer_irq[MAX_TIMER_CNT];
}TIMER_IRQS;

/*
#define AVR_SIGNAL(signame,handle)			\
extern  void handle(void);				\
void signame (void) __attribute__ ((signal));		\
void signame (void)					\				
{							\
	handle();					\
}\


//AVR_SIGNAL(SIG_OVERFLOW0, timer0_irq)
AVR_SIGNAL(SIG_OVERFLOW2, timer2_irq)
*/
extern void timer2_irq(void);
void SIG_OVERFLOW2(void) __attribute__ ((signal));

void SIG_OVERFLOW2(void)
{
	timer2_irq();
}

#if 0

////---- ADD_2005_10_31
#define TCNT0_VALUE	0xD7		// 5ms at 8MHz
#define TCNT2_VALUE	0x87		// 15ms at 8MHz
////---- END_ADD

int sys_tick_5ms, sys_tick_1s;		// Tick count


/* Timer handler table */
struct _TIMER_IRQS
{
	TIMER_IRQ timer_irq[MAX_TIMER_CNT];
}TIMER_IRQS;


#define AVR_SIGNAL(signame,handle)			\
extern  void handle(void);				\
void signame (void) __attribute__ ((signal));		\
void signame (void)					\				
{							\
	handle();					\
}\


AVR_SIGNAL(SIG_OVERFLOW0, timer0_irq)
AVR_SIGNAL(SIG_OVERFLOW2, timer2_irq)

/*
********************************************************************************
*              TIMER ISR
*
* Description : This function counts timer tick value.
*   
* Arguments   : None
* Returns     : None
* Note        : Internal Function
********************************************************************************
*/
void timer0_irq(void)
{
	u_char i;
	sys_tick_5ms++;
	for(i=0; i < MAX_TIMER0_CNT; i++)
	{
		if(TIMER_IRQS.timer_irq[i].user_timer_handler)
			(*TIMER_IRQS.timer_irq[i].user_timer_handler)();
	}

////--- MODIFY_2005_10_31	
	// TCNT0 = 0xE1;			/* reset TCNT0(2 ms) */
	TCNT0 = TCNT0_VALUE;			/* reset TCNT0(5 ms) */
////--- END_MODIFY	
}


void timer2_irq(void)		/* signal handler for tcnt0 overflow interrupt */
{
	u_char i;
	
	if (++sys_tick_1s > 64) /* 1 sec. */
	{	
		sys_tick_1s = 0;
		for(i=MAX_TIMER0_CNT; i < MAX_TIMER_CNT; i++)
		{
			if(TIMER_IRQS.timer_irq[i].user_timer_handler)
				(*TIMER_IRQS.timer_irq[i].user_timer_handler)();
		}
	}
////---- MODIFY_2005_10_31
	//TCNT2 = 0;			/* reset TCNT0(15 ms) */
	TCNT2 = TCNT2_VALUE;		/* reset TCNT0(15 ms) */
////---- END_MODIFY	
}

/*
********************************************************************************
*              INIT. TIMER
*
* Description : This function initialize AVR timer.
*   
* Arguments   : 
* Returns     : None
* Note        :
********************************************************************************
*/
void init_timer(void)
{
	u_char i;
	cli();
	for(i=0;i<MAX_TIMER_CNT; i++)
		TIMER_IRQS.timer_irq[i].user_timer_handler = 0;
	TIMSK &= ~(1 << TOIE2);	
////---- MODIFY_2005_10_31
	//TCNT2 = 0;			/* reset TCNT0(15 ms) */
	TCNT2 = TCNT2_VALUE;		/* reset TCNT0(15 ms) */
////---- END_MODIFY	
	TCCR2 = 5;		/* count with cpu clock/1024 */
	sys_tick_1s = 0;

	TIMSK &= ~(1 << TOIE0);		/* disable TCNT0 overflow */
////--- MODIFY_2005_10_31	
	// TCNT0 = 0xE1;			/* reset TCNT0(2 ms) */
	TCNT0 = TCNT0_VALUE;			/* reset TCNT0(5 ms) */
////--- END_MODIFY	
	TCCR0 = 7;			/* count with cpu clock/1024 */		
	sys_tick_5ms = 0;
	sei();
}

/*
********************************************************************************
*              Regiser TIMER Handler
*
* Description : Register the timer handler
*   
* Arguments   : timer - Timer Handler Number
*		handler - user specific function to be called by timer interrupt
* Returns     : None
* Note        :
********************************************************************************
*/
void set_timer(u_int timer, void (*handler)(void)) 
{

	cli();
	if(timer < MAX_TIMER0_CNT)
		TIMSK |= (1 << TOIE0);
	else
		TIMSK |= (1 << TOIE2);
	if(handler) TIMER_IRQS.timer_irq[timer].user_timer_handler = handler;
	sei();
}


/*
********************************************************************************
*              Unregister Timer Handler
*
* Description : Unregister the timer handler
*   
* Arguments   : timer - user specific function to be called by timer interrupt
* Returns     : None
* Note        :
********************************************************************************
*/
void kill_timer(u_int timer) 
{
	u_char i;
	u_char bkill;
	cli();
	TIMER_IRQS.timer_irq[timer].user_timer_handler = 0;

	if(timer < MAX_TIMER0_CNT)
	{
		bkill = 1;
		for(i = 0; i < MAX_TIMER0_CNT; i++)
		{
			if(TIMER_IRQS.timer_irq[i].user_timer_handler)
			{
				bkill = 0;
				break;
			}
		}
		if(bkill) 
		{
			sys_tick_5ms = 0;
			TIMSK &= ~(1<<TOIE0);		
		}
	}
	else
	{
		bkill = 1;
		for(i = MAX_TIMER0_CNT; i < MAX_TIMER_CNT; i++)
		{
			if(TIMER_IRQS.timer_irq[i].user_timer_handler)
			{
				bkill = 0;
				break;
			}
		}
		if(bkill)
		{
			TIMSK &= ~(1<<TOIE2);
			sys_tick_1s = 0;
		}
	}
	sei();
}
#endif



#if (MAX_TIMER0_CNT > 0)
#if (__COMPILER_VERSION__ == __WINAVR_20050214__)

#if 0
void timer0_irq( void );
void SIG_OVERFLOW0( void ) __attribute__ ((signal));

void SIG_OVERFLOW0( void )
{
	timer0_irq();
}
#endif



/**
@brief	This function counts timer tick value.
 
Internal Function \n
Interrupt Service Routine for AVR GCC ver. 3.4.6
*/
void timer0_irq( void )
{ 
	u_char i;
	sys_tick_5ms++;
	for(i=0; i < MAX_TIMER0_CNT; i++)
	{
		if(TIMER_IRQS.timer_irq[i].user_timer_handler)
			(*TIMER_IRQS.timer_irq[i].user_timer_handler)();
	}

	TCNT0 = TCNT0_VALUE;			/* reset TCNT0(5 ms) */
}
#else
/**
@brief	This function counts timer tick value.
 
Internal Function \n
Interrupt Service Routine for AVR GCC ver. 3.4.6
*/
ISR(TIMER0_OVF_vect)
{ 
	u_char i;
	sys_tick_5ms++;
	for(i=0; i < MAX_TIMER0_CNT; i++)
	{
		if(TIMER_IRQS.timer_irq[i].user_timer_handler)
			(*TIMER_IRQS.timer_irq[i].user_timer_handler)();
	}

	TCNT0 = TCNT0_VALUE;			/* reset TCNT0(5 ms) */
}
#endif
#endif


#if (MAX_TIMER2_CNT > 0)
#if (__COMPILER_VERSION__ == __WINAVR_20050214__)

#if 0
static void timer2_irq( void );
void SIG_OVERFLOW2( void ) __attribute__ ((signal));

void SIG_OVERFLOW0( void )
{
	timer2_irq();
}
#endif

/**
@brief	This function counts timer tick value.
*/
void timer2_irq( void )
{
	u_char i;
	
	if (++sys_tick_1s > 64) /* 1 sec. */
	{	
		sys_tick_1s = 0;
		for(i=MAX_TIMER0_CNT; i < MAX_TIMER_CNT; i++)
		{
			if(TIMER_IRQS.timer_irq[i].user_timer_handler)
				(*TIMER_IRQS.timer_irq[i].user_timer_handler)();
		}
	}
	TCNT2 = TCNT2_VALUE;		/* reset TCNT0(15 ms) */
}
#else
/**
@brief	This function counts timer tick value.
  
Internal Function \n
Interrupt Service Routine for AVR GCC ver. 3.4.6
*/
ISR(TIMER2_OVF_vect)
{
	u_char i;

	if (++sys_tick_1s > 64) /* 1 sec. */
	{	
		sys_tick_1s = 0;
		for(i=MAX_TIMER0_CNT; i < MAX_TIMER_CNT; i++)
		{
			if(TIMER_IRQS.timer_irq[i].user_timer_handler)
				(*TIMER_IRQS.timer_irq[i].user_timer_handler)();
		}
	}
	TCNT2 = TCNT2_VALUE;		/* reset TCNT0(15 ms) */
}
#endif	/* #ifdef __STDIO_FDEVOPEN_COMPAT_12 */
#endif	/* #if (MAX_TIMER2_CNT > 0) */


/**
@brief	This function initialize AVR timer.
*/
void init_timer(void)
{
#if ( MAX_TIMER_CNT > 0 )
	u_char i;
	
	cli();
	for(i=0;i<MAX_TIMER_CNT; i++)
		TIMER_IRQS.timer_irq[i].user_timer_handler = 0;
#if (MAX_TIMER2_CNT > 0)	
	TIMSK &= ~(1 << TOIE2);	
	TCNT2 = TCNT2_VALUE;		/* reset TCNT2(15 ms) */
	TCCR2 = 5;		/* count with cpu clock/1024 */
	sys_tick_1s = 0;
#endif	

#if (MAX_TIMER0_CNT > 0)
	TIMSK &= ~(1 << TOIE0);		/* disable TCNT0 overflow */
	TCNT0 = TCNT0_VALUE;			/* reset TCNT0(5 ms) */
	TCCR0 = 7;			/* count with cpu clock/1024 */		
	sys_tick_5ms = 0;
#endif	
	sei();
#endif	/* #if ( MAX_TIMER_CNT > 0 ) */
}


/**
@brief	Register the timer handler
*/
void set_timer(
	u_int timer, 			/**< timer Handler Number */
	void (*handler)(void)	/**< user specific function to be called by timer interrupt */
	) 
{
#if ( MAX_TIMER_CNT > 0 )
	cli();
#if (MAX_TIMER0_CNT > 0)	
	if(timer < MAX_TIMER0_CNT)
	{
		TIMSK |= (1 << TOIE0);
	}	
	else
#endif		
	{
#if (MAX_TIMER2_CNT > 0)		
		TIMSK |= (1 << TOIE2);
#endif
	}
	
	if(handler) TIMER_IRQS.timer_irq[timer].user_timer_handler = handler;
	sei();
#endif /* #if ( MAX_TIMER_CNT > 0 ) */
}


/**
@brief	Unregister Timer Handler
*/
void kill_timer(
	u_int timer	/**< user specific function to be called by timer interrupt */
	) 
{
#if ( MAX_TIMER_CNT > 0 )
	u_char i;
	u_char bkill;
	cli();
	TIMER_IRQS.timer_irq[timer].user_timer_handler = 0;

#if (MAX_TIMER0_CNT > 0)
	if(timer < MAX_TIMER0_CNT)
	{
		bkill = 1;
		for(i = 0; i < MAX_TIMER0_CNT; i++)
		{
			if(TIMER_IRQS.timer_irq[i].user_timer_handler)
			{
				bkill = 0;
				break;
			}
		}
		if(bkill) 
		{
			sys_tick_5ms = 0;
			TIMSK &= ~(1<<TOIE0);		
		}
	}
	else
#endif		
	{
#if (MAX_TIMER2_CNT > 0)	
		bkill = 1;
		for(i = MAX_TIMER0_CNT; i < MAX_TIMER_CNT; i++)
		{
			if(TIMER_IRQS.timer_irq[i].user_timer_handler)
			{
				bkill = 0;
				break;
			}
		}
		if(bkill)
		{
			TIMSK &= ~(1<<TOIE2);
			sys_tick_1s = 0;
		}
#endif		
	}
	sei();
#endif /* #if ( MAX_TIMER_CNT > 0 ) */
}

