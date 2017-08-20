/**
 @file		main.c
 @brief 		DHCP source ( for W5100 @ Atmega128)
 			optimized compiler : WinAVR-20071221
 */
 
#include <avr/io.h>
#include <avr/interrupt.h>
// #include <avr/signal.h>
#include <avr/eeprom.h>
#include <stdio.h> 
#include <string.h>

#include "types.h"
#include "mcu.h"
#include "delay.h"

#include "w5100.h"
#include "serial.h"
#include "socket.h"
#include "timer.h"
#include "dhcp.h"


extern u_char SRC_MAC_ADDR[6];		/**< Source MAC Address */

#define LED_AVR_PORT_VAL	PORTG
#define LED_AVR_PORT_DIR	DDRG

#define LED_PIN_0		3
#define LED_PIN_1		4

//#define LOOP_BACK_PORT		5000

/*
********************************************************************************
Function Implementation Part
********************************************************************************
*/
void led_off(u_char led)
{
	LED_AVR_PORT_VAL &= ~(1 << (LED_PIN_0+led));
}

void led_on(u_char led)
{
	LED_AVR_PORT_VAL |= (1 << (LED_PIN_0+led));
}

void led_init(void)
{
	u_int i;
	
	LED_AVR_PORT_DIR |= (1<<LED_PIN_0) | (1<<LED_PIN_1);
	for(i = 0; i < 5; i++)
	{
		led_on(0);
		led_on(1);
		wait_10ms(50);
		led_off(0);
		led_off(1);
		wait_10ms(50);
	}
}


#if 0
/**
 @brief	loopback test using TCP(server mode)
 */
void loopback_tcps(
	u_char ch	/**< channel(socket) index */
	)
{
	int len;							
	u_char * data_buf = (u_char*) TX_BUF;

	switch (getSn_SR(ch))
	{
	case SOCK_ESTABLISHED:						/* if connection is established */
		if(bchannel_start[ch]==1)
		{
			//printf("%d : Connected by %s(%u)",ch,inet_ntoa(GetDestAddr(ch)),GetDestPort(ch));
			bchannel_start[ch] = 2;
		}
		if ((len = getSn_RX_RSR(ch)) > 0) 			/* check Rx data */
		{
			if (len > TX_RX_MAX_BUF_SIZE) len = TX_RX_MAX_BUF_SIZE;	/* if Rx data size is lager than TX_RX_MAX_BUF_SIZE */
									/* the data size to read is MAX_BUF_SIZE. */
			len = recv(ch, data_buf, len);			/* read the received data */
			send(ch, data_buf, len);				/* send the received data */
		}
		break;
	case SOCK_CLOSE_WAIT:                           		/* If the client request to close */
		printf("%d : CLOSE_WAIT\r\n", ch);
		disconnect(ch);
		bchannel_start[ch] = 0;
		break;
	case SOCK_CLOSED:                                               /* if a socket is closed */
		if(!bchannel_start[ch]) 
		{
			printf("%d : Loop-Back TCP Server Started.\r\n",ch);
			bchannel_start[ch] = 1;
		}
		if(socket(ch,Sn_MR_TCP,LOOP_BACK_PORT,0x00) == 0)    /* reinitialize the socket */
		{
			printf("\a%d : Fail to create socket.\r\n",ch);
			bchannel_start[ch] = 0;
		}
		else
			listen(ch);
		
		break;
	}
}
#endif


/**
	@mainpage	W5100 Firmware Source for DHCPC
	@section intro	Introduction
	- Introduction : supported dhcp client
	@section CREATEINFO	Author
	- author : www.wiznet.co.kr
	- last update : 2008.1.8
*/
int main(void)
{
//	SOCKET i;

	u_char tmpip[4];
	mcu_init();
	uart_init(0, 7);		// Serial Port Initialize
	
	printf("\r\n*** DHCP Test using W5100 ");
#if (__DEF_IINCHIP_BUS__ == __DEF_IINCHIP_DIRECT_MODE__)
	printf("[DIRECT MODE]");
#elif(__DEF_IINCHIP_BUS__ == __DEF_IINCHIP_INDIRECT_MODE__)	/* INDIRECT MODE I/F */
	printf("[INDIRECT MODE]");
#elif (__DEF_IINCHIP_BUS__ == __DEF_IINCHIP_SPI_MODE__)
	printf("[SPI MODE]");
#else
	#error "unknown bus type"
#endif	
	printf(" ***\r\n");

	printf("******* Firmware Version : %d.%d.%d.%d *******\r\n",
			(u_char)(FW_VERSION>>24),
			(u_char)(FW_VERSION>>16),
			(u_char)(FW_VERSION>>8),
			(u_char)(FW_VERSION) );
	
	wait_10ms(100);
	
	led_init();	
	init_timer();		// Initialize timer
	

	// MAC address
	SRC_MAC_ADDR[0] = 0x00;
	SRC_MAC_ADDR[1] = 0x08;
	SRC_MAC_ADDR[2] = 0xDC;
	SRC_MAC_ADDR[3] = 0x00;
	SRC_MAC_ADDR[4] = 0x00;
	SRC_MAC_ADDR[5] = 0x4F;
	
	/* get ip */
	init_dhcp_client(0, mcu_soft_reset, mcu_soft_reset);

	while ( 1 )
	{
		printf("\r\nSerching DHCP Server...\r\n");
		if ( getIP_DHCPS() )
			break;
	}
	
	printf("\r\n*** THANK YOU ***\r\n");
	tmpip[0] = 211;
	tmpip[1] = 46;
	tmpip[2] = 117;
	tmpip[3] = 79;
	
	sendto(3, (const u_char*)"for PING TEST", 13, tmpip, 5000); 
	while ( 1 ) 
	{
		check_DHCP_state( 0 );
	}
	
	return 1;
#if 0
	printf("\r\n\r\nRunning loop-back server\r\n");	
	while(1)
	{
		if ( SOCK_TCPS )
			check_DHCP_state( 0 );

		for (i = SOCK_TCPS ; i < MAX_SOCK_NUM ; i++)
		{
			loopback_tcps(i);
		}
	}
#endif	
}

