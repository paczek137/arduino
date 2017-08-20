/**
 @file		main.c
 @brief 		pppoe source ( for W5100 @ Atmega128 )
 			optimized compiler : WinAVR-20071221
 */
#include <avr/io.h>
#include <avr/interrupt.h>
//#include <avr/signal.h>
#include <avr/eeprom.h>
#include <stdio.h> 
#include <string.h>
#include "types.h"
#include "w5100.h"
#include "mcu.h"
#include "delay.h"
#include "serial.h"
#include "socket.h"

/*
********************************************************************************
Define Part
********************************************************************************
*/
#define	MAX_BUF_SIZE	2048		/* maximum size of Rx buffer. */

/*
********************************************************************************
Local Variable Declaration Section
********************************************************************************
*/
uint8 * rx_buf = (uint8 *)(0x7000);	/* Rx buffer for Application */

/*
*******************************************************************************
Function Prototype Declaration Part
*******************************************************************************
*/

extern char uart0_getchar(FILE *f);

#define LOOP_BACK_PORT		5000

u_char bchannel_start[MAX_SOCK_NUM] = { 0, 0, 0, 0};	/** 0:close, 1:ready, 2:connect */

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


/**
	@mainpage	W5100 Firmware Source for PPPoE
	@section intro	Introduction
	- Introduction : supported PPPoE
	@section CREATEINFO	Author
	- author : www.wiznet.co.kr
	- date : 2007.1.2
*/
int16 main(void)
{
	SOCKET i;
	//int16 len;		/* size of rx data */
	uint8 ip[6];
	//uint16 loop_idx;
	//uint16 dummy;

	uint8 pppmac[6], pppsessionid[2], pppip[4];
	uint8 pppid[128];
	uint8 ppppasswd[128];
	uint8 pppid_len;
	uint8 ppppasswd_len;
	uint8 ret;
	uint8 ppp_state;
	uint8 scanf_ret;
	
	mcu_init();
	uart_init(0, 7);		// Serial Port Initialize

	printf("\r\n*** ADSL Test using W5100 ");
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

	sei();			/* enable interrupts */
	iinchip_init();

	ip[0] = 0x00; ip[1] = 0x08; ip[2] = 0xdc; ip[3] = 0x00; ip[4] = 0x11; ip[5] = 0x19;
	setSHAR(ip);
	ip[0] = 192; ip[1] = 168; ip[2] = 10; ip[3] = 1;
	setGAR(ip);
	ip[0] = 255; ip[1] = 255; ip[2] = 255; ip[3] = 0;
	setSUBR(ip);
	//ip[0] = 192; ip[1] = 168; ip[2] = 10; ip[3] = 31;
	//setSIPR(ip);
	setRCR(0x05); // timeout count is 5
	sysinit(0x55,0x55); // set 2K memory each socket
	
/* -------------------------------------------------
 * adsl connection code begin
 * -------------------------------------------------*/

	printf("* W5100 PPPoE(ADSL) test program *\r\n");
	printf("- For PPPoE(adsl) test, Input 'a' key -\r\n");
	
	if (uart0_getchar(NULL) == 'a')
	{
AUTH_FAIL:
		printf("<<Setup PPPoE connection>>\r\n");

		/* 1. Get the user id for pppoe(adsl) connection */
		printf("Enter user id : ");
		scanf_ret = scanf("%s", pppid);
		printf("%s\r\n",pppid);
		pppid_len = strlen((char*)pppid);

		/* 2. Get the password for pppoe(adsl) connection */
		printf("Enter user password : ");
		scanf_ret = scanf("%s", ppppasswd);
		printf("%s\r\n",ppppasswd);
		printf("\r\n");
		ppppasswd_len = strlen((char*)ppppasswd);

		i = 0;
		
		/* 3. Disconnect the previous pppoe(adsl) connection */
		pppterm(pppmac, pppsessionid);

		/* 4. Start to connect. If ret is 1, then adsl connection is established. */
		while ((ret = pppinit( pppid, pppid_len, ppppasswd, ppppasswd_len)) != 1)
		{
			switch (ret)
			{
				/* if ret is 2, authentication fail */
				case 2 : 
					getSn_DHAR(0, pppmac);
					getSn_DPORT(0, pppsessionid);
					goto AUTH_FAIL; 
					break;
				default : break;
			}				
			i++;
   		/*  */
			if (i == 5) break;
		}
		
		/* 5. save information about adsl connection */
		if (i != 5)
		{
			ppp_state = getIR(); // remove already setted value of INT_REG
			printf("<<PPP Connection established>>\r\n");
			getSn_DHAR(0, pppmac);
			getSn_DPORT(0, pppsessionid);
			getSIPR(pppip);
			printf("Get IP ADDRESS : ");
			for (i = 0; i < 3; i++) printf("%d.", pppip[i]);
			printf("%d", pppip[i]);
			printf("\r\n");
			setSIPR(pppip);
		}
		else
		{
			setMR(0x00);
		}
	}
/* -------------------------------------------------
 * adsl connection code end
 * -------------------------------------------------*/
 
	// display network information
	printf("\r\n====================================\r\n");
	printf("        Network Information\r\n");
	printf("====================================\r\n");

	printf("MAC ADDRESS      : ");
	getSHAR(ip);
	for (i = 0; i < 5; i++)	printf("%.2X.", ip[i]);
	printf("%.2X", ip[i]);
	printf("\r\n");
	
	printf("SUBNET MASK      : ");
	getSUBR(ip);
	for (i = 0; i < 3; i++)	printf("%d.", ip[i]);
	printf("%d", ip[i]);
	printf("\r\n");

	printf("G/W IP ADDRESS   : ");
	getGAR(ip);
	for (i = 0; i < 3; i++)	printf("%d.", ip[i]);
	printf("%d", ip[i]);
	printf("\r\n");

	printf("LOCAL IP ADDRESS : ");
	getSIPR(ip);
	for (i = 0; i < 3; i++)	printf("%d.", ip[i]);
	printf("%d", ip[i]);
	printf("\r\n");
	printf("====================================\r\n");
	printf("\r\n*** THANK YOU ***\r\n");
	return 1;

	/* loopback test... */
	while(1)
	{
		for (i = 1 ; i < MAX_SOCK_NUM ; i++)
		{
			loopback_tcps(i);
		}
	}
	return 1;
}

