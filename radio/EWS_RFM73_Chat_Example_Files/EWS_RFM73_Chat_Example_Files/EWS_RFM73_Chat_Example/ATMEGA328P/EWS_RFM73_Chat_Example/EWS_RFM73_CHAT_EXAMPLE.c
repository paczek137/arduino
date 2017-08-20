/************************************************************************  
Copyright (C) 2012 Embedded Wireless Solutions LLC

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*************************************************************************/

/************************************************************************
*********************** EWS_RFM73_CHAT_EXAMPLE.c ************************
*************************************************************************

	DESCRIPTION:  This program acts as both transmitter and receiver
		for the RFM73, allowing users to enter a character message 
		via a UART terminal program (such as Hyperterminal), which is sent 
		wirelessly to another RFM73-based station.  This program is a 
		simple, basic demonstration of use of the RFM73 wireless 
		transceiver module.  Maximum message length: 32 characters.

		This program is adapted from example RFM73 applications on the 
		manufacturer's website (www.hoperf.com).
		
	SPECIFICATIONS:
	- Radio: RFM73 - 2.4GHz
	- Hardware: ATMEL ATMEGA328P, 3.3V
	- IDE: AVRStudio 4.19 (build 730) -or- WINAVR
	- Programmer: AVRISP MKII -or- optiboot (arduino) bootloader
	- Data Rate: 4800bps
	- Hardware connections:
	
	    RFM73	|  ATMEGA328P	|	Other Hardware
	----------------------------------------------
		GND		|	   GND		|
		VDD		|	   VCC		|
		
		CE		|	   PC2		|
		CSN		|	   PB2		|
		SCK		|	   PB5		|
		MOSI	|	   PB3		|
		MISO	|	   PB4		|
				
				|	   PB1		| 	RED LED circuit		
				|	   PB0		| 	GREEN LED circuit

By: Embedded Wireless Solutions LLC, 2012 (http://www.goEWS.com)
***********************************************************************/

#include "RFM73_init.c"

//=======FUNCTION DECLARATIONS==========
void ioinit(void);

void uart_putstr(const char *str);

void power_on_delay(void);

void Send_Packet(uint8_t type,uint8_t* pbuf,uint8_t len);
void Receive_Packet(void);

extern void RFM73_Initialize(void);
extern void SwitchToTxMode(void);
extern void SwitchToRxMode(void);

//uint8_t tx_buf[17]={0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3a,0x3b,0x3c,0x3d,0x3e,0x3f,0x79};
uint8_t tx_buf[MAX_PACKET_LEN]; 
uint8_t rx_buf[MAX_PACKET_LEN];

extern const uint8_t RX0_Address[];
extern const unsigned long Bank1_Reg0_13[];

uint8_t txFlag=OFF;
//======================================

int main(void)
{
	//uint8_t i,j;	//used in repetitive TX VERSION

	ioinit();	// initialize the MCU	
	tx_buf[0] = '\0';	// initialize tx buffer
	sei();
	power_on_delay();  

	RFM73_Initialize();
	
	uart_putstr("**********Welcome to EWS_RFM73_CHAT_EXAMPLE!!!!!**********\n\r\n\r");
	while(1)
	{
/*
//=====Use the following code snippet for repetitive TX VERSION ====
//===========(~1 second interval between transmits)=================

		_delay_ms(1000);		// wait one second between transmits
		
		j=0;
		for(i=0;i<16;i++)
		{
			j += tx_buf[i];
		}
		tx_buf[16] = j;

		Send_Packet(W_TX_PAYLOAD_NOACK_CMD,tx_buf,17);	// transmit packet - TX VERSION
		tx_buf[0] += 1;

		SwitchToRxMode();  		// switch to Rx mode - TX VERSION
 //=========================================================
*/

//===Use the following code snippet for simple chat Example (both Tx & Rx)====
//=========Type message in hyperterminal, followed by '\r'====================

		if(txFlag == ON)
		{
			uart_putstr("\n\r");
			cli();				// disable interrupts
			Send_Packet(W_TX_PAYLOAD_NOACK_CMD,tx_buf,(uint8_t)strlen(tx_buf));	// transmit packet
			tx_buf[0] = '\0';	// clear tx_buf
			txFlag = OFF;		// clear txFlag
			sei();				// enable interrupts
			SwitchToRxMode();
		}
//====================================================================

		Receive_Packet();		// receive packet (if there are any)
	}
}


/*********************************************************
Function:      power_on_delay()                                    
                                                            
Description:                                                
 
*********************************************************/
void power_on_delay(void)
{
	_delay_ms(1000);
}

/**************************************************
Function: Send_Packet
Description:
	fill FIFO to send a packet
Parameter:
	type: WR_TX_PLOAD or  W_TX_PAYLOAD_NOACK_CMD
	pbuf: a buffer pointer
	len: packet length
Return:
	None
**************************************************/
void Send_Packet(uint8_t type,uint8_t* pbuf,uint8_t len)
{
	uint8_t fifo_sta;
	
	SwitchToTxMode();  //switch to tx mode

	fifo_sta=SPI_Read_Reg(FIFO_STATUS);	// read register FIFO_STATUS's value
	if((fifo_sta&FIFO_STATUS_TX_FULL)==0)//if not full, send data (write buff)
	{ 
	  	sbi(PORTB,RED_LED);
		
		SPI_Write_Buf(type, pbuf, len); // Writes data to buffer
		
		pbuf[len-1] = '\0';				// remove checksum before sending to terminal
		uart_putstr("Sending: ");		// print sent message to terminal
		uart_putstr(pbuf);	
		uart_putstr("\n\r\n\r");

		_delay_ms(50);
		cbi(PORTB,RED_LED);
		_delay_ms(50);
	}	  	 	
}

/**************************************************
Function: Receive_Packet
Description:
	read FIFO to read a packet
Parameter:
	None
Return:
	None
**************************************************/
void Receive_Packet(void)
{
	uint8_t len,rx_count,j,sta,fifo_sta,chksum;
	uint8_t rx_buf[MAX_PACKET_LEN];

	rx_count = 0;	
	sta=SPI_Read_Reg(STATUS);	// read register STATUS's value

	if((STATUS_RX_DR&sta) == 0x40)				// if receive data ready (RX_DR) interrupt
	{
		do
		{
			len=SPI_Read_Reg(R_RX_PL_WID_CMD);	// read len

			if(len<=MAX_PACKET_LEN)
			{
				SPI_Read_Buf(RD_RX_PLOAD,rx_buf,len);// read receive payload from RX_FIFO buffer
				rx_count += len;	// keep tally of # of bytes received
			}
			else
			{
				SPI_Write_Reg(FLUSH_RX,0);//flush Rx
			}

			fifo_sta=SPI_Read_Reg(FIFO_STATUS);	// read register FIFO_STATUS's value
							
		}while((fifo_sta&FIFO_STATUS_RX_EMPTY)==0); //while not empty
		
		rx_buf[rx_count] = '\0';	// mark end of buffer with null character
		chksum = 0;
		for(j=0;j<(rx_count-1);j++)		// calculate checksum 
		{
			chksum += rx_buf[j]; 
		}

		if(chksum==rx_buf[rx_count-1])
		{
			sbi(PORTB,GREEN_LED);
			_delay_ms(100);
			cbi(PORTB,GREEN_LED);

			//Send_Packet(W_TX_PAYLOAD_NOACK_CMD,rx_buf,17);	// not used...
			
 	//=========USE THE FOLLOWING FOR RX VERSION=======			
			SwitchToRxMode();//switch to RX mode				// RX VERSION
			
			rx_buf[len-1] = '\0';		// terminate string with NULL character (and remove the checksum)
			
			uart_putstr("Receiving: ");
			uart_putstr(rx_buf);
			uart_putstr("\n\r\n\r");
	//===============================================

		}	
	}
	SPI_Write_Reg(WRITE_REG|STATUS,sta);	// clear RX_DR or TX_DS or MAX_RT interrupt flag	
}

/************************************************************************
	FUNCTION: ioinit() 
	DESCRIPTION:  This function prepares the ATMEGA for use: 
		1. Initialize on I/O ports
		2. Initialize SPI port
		3. Initialize UART1 port
*************************************************************************/
void ioinit(void)
{
	
	// Initialize I/O ports
    //1 = output, 0 = input
    //DDRB = 0b10110100; //MISO (PB6) & IRQ (PB0) are inputs, PB1 & PB3 are unused, the rest are outputs
    DDRB = 0b11101110; // MISO (PB4) & IRQ (PB0) are inputs, PB1 & PB3 are unused, the rest are outputs
	DDRC = 0b11111111;	// PC2 is used as an output for chip enable of RFM73 module
	
	// Initialize SPI port
	SPCR |= 0b01010011;		// Fosc/128
	
	// Initialize UART0
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0); // Turn on the transmission and reception circuitry
	UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01); // Use 8-bit character sizes
	UBRR0L = (unsigned char)BAUD_PRESCALE; // Load lower 8-bits of the baud rate value into the low byte of the UBRR register
	UBRR0H = (unsigned char)(BAUD_PRESCALE >> 8); // Load upper 8-bits of the baud rate value into the high byte of the UBRR register

	UCSR0B |= (1 << RXCIE0); // Enable the USART Receive Complete interrupt (USART_RXC)
}

/************************************************************************
	FUNCTION: uart_putstr() 
	DESCRIPTION:  This function: 
		1. sends a character string on USART1
		2. uses no ISR's (checks for flags)
*************************************************************************/
void uart_putstr(const char *str)	// send a character string on USART1
{
	unsigned char i;
	i = 0;
	
	while (str[i] != '\0') 
	{
		while ( !( UCSR0A & (1<<UDRE0)) );	// wait for the Tx data buffer to be empty
		UDR0 = str[i];	// send next character in buffer t
		i++;			
	}
}

/************************************************************************
	FUNCTION: ISR(USART1_RX_vect) - UART1 Receive Interrupt
	DESCRIPTION:  This is the ISR function for handling a byte received on 
		UART1 (character entered by user to transmit, NOT received from RFM22B)
*************************************************************************/

ISR(USART_RX_vect)
{
	uint8_t ReceivedByte,i,j,chksum;
	
	ReceivedByte = UDR0; // Fetch the recieved byte value into the variable "ByteReceived"

	
	if (((i=(uint8_t)strlen(tx_buf)) > (MAX_PACKET_LEN-3)) || (ReceivedByte == '\r')) // send if packet full or msg done
	{
		chksum = 0;
		for(j=0;j<i;j++)		// calculate checksum 
		{
			chksum +=tx_buf[j]; 
		}
		tx_buf[i] = chksum;		// add checksum to end of buffer
		txFlag = ON;			// set flag to indicate transmit
	}
	else
	{
		tx_buf[i] = ReceivedByte;	// add received byte to transmit buffer
	}
	tx_buf[i+1]	= '\0';			// add end to the buffer
	UDR0 = ReceivedByte; // Send received byte to same UART channel (Echo mode)
}

/************************************************************************/
