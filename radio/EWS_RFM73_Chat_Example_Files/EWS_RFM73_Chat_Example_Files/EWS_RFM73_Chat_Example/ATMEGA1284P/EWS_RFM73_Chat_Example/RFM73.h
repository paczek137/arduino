//=========RFM73.h=====================================================
/**********************************************************************
	SPECIFICATIONS:
	- Radio: RFM73 - 2.4GHz
	- Hardware: ATMEL ATMEGA1284P, 3.3V
	- IDE: AVRStudio 4.19 (build 730) -or- WINAVR
	- Programmer: AVRISP MKII -or- optiboot (arduino) bootloader
	- Data Rate: 4800bps
	- Hardware connections:
	
	    RFM70	|  ATMEGA1284P	|	Other Hardware
	----------------------------------------------
		GND		|	   GND		|
		VDD		|	   VCC		|
		
		IRQ		|	   PB0		|****Note: IRQ not used in this example****
		CE		|	   PB2		|
		CSN		|	   PB4		|
		SCK		|	   PB7		|
		MOSI	|	   PB5		|
		MISO	|	   PB6		|
				
				| 	   PC6		|	Green LED circuit
				|	   PC4		|	Red LED circuit
		
By: Embedded Wireless Solutions LLC, 2012 (http://www.goEWS.com)
***********************************************************************/

//============INCLUDES==================
#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <string.h>
#include <util/delay.h>
#include <avr/interrupt.h>
//======================================

//=============MACROS===================
#define sbi(var, mask)   ((var) |= (uint8_t)(1 << mask))	// MACRO: set register bit
#define cbi(var, mask)   ((var) &= (uint8_t)~(1 << mask))	// MACRO: clear register bit
#define toggle(var, mask) 	((var) ^= (uint8_t)(1<< mask))	// MACRO: toggle register bit
//======================================

//===============DEFINES================

#define CE   2		// PB2 	(Chip enable) - OUTPUT
#define CSN  4		// PB4 	(Chip Select) - OUTPUT
#define SCK  7		// PB7 	(SPI Clock) - OUTPUT
#define MISO 6		// PB6 	(MOSI - Master out/Slave in) - OUTPUT
#define MOSI 5		// PB5 	(MISO - Master in/slave out) - INPUT
#define IRQ  0		// PB0 	(Interrupt pin) - INPUT (Note: Not used in this example)

#define RED_LED 4	// PC4 (RED LED)
#define GREEN_LED 6	// PC6 (GREEN LED)

#define USART_BAUDRATE1 4800	// UART1 Baudrate
#define BAUD_PRESCALE1 (((F_CPU / (USART_BAUDRATE1 * 16UL))) - 1) // UART1 Baud Prescale value for UBRR register

#define MAX_PACKET_LEN  32// max value is 32

#define OFF 0
#define ON 1
//======================================

//************************FSK COMMAND and REGISTER****************************************//
// SPI(RFM73) commands
#define READ_REG        		0x00  // Define read command to register
#define WRITE_REG       		0x20  // Define write command to register
#define RD_RX_PLOAD     		0x61  // Define RX payload register address
#define WR_TX_PLOAD     		0xA0  // Define TX payload register address
#define FLUSH_TX        		0xE1  // Define flush TX register command
#define FLUSH_RX        		0xE2  // Define flush RX register command
#define REUSE_TX_PL     		0xE3  // Define reuse TX payload register command
#define W_TX_PAYLOAD_NOACK_CMD	0xb0
#define W_ACK_PAYLOAD_CMD		0xa8
#define ACTIVATE_CMD			0x50
#define R_RX_PL_WID_CMD			0x60
#define NOP_NOP            		0xFF  // Define No Operation, might be used to read status register

// SPI(RFM73) registers(addresses)
#define CONFIG          0x00  // 'Config' register address
#define EN_AA           0x01  // 'Enable Auto Acknowledgment' register address
#define EN_RXADDR       0x02  // 'Enabled RX addresses' register address
#define SETUP_AW        0x03  // 'Setup address width' register address
#define SETUP_RETR      0x04  // 'Setup Auto. Retrans' register address
#define RF_CH           0x05  // 'RF channel' register address
#define RF_SETUP        0x06  // 'RF setup' register address
#define STATUS          0x07  // 'Status' register address
#define OBSERVE_TX      0x08  // 'Observe TX' register address
#define CD              0x09  // 'Carrier Detect' register address
#define RX_ADDR_P0      0x0A  // 'RX address pipe0' register address
#define RX_ADDR_P1      0x0B  // 'RX address pipe1' register address
#define RX_ADDR_P2      0x0C  // 'RX address pipe2' register address
#define RX_ADDR_P3      0x0D  // 'RX address pipe3' register address
#define RX_ADDR_P4      0x0E  // 'RX address pipe4' register address
#define RX_ADDR_P5      0x0F  // 'RX address pipe5' register address
#define TX_ADDR         0x10  // 'TX address' register address
#define RX_PW_P0        0x11  // 'RX payload width, pipe0' register address
#define RX_PW_P1        0x12  // 'RX payload width, pipe1' register address
#define RX_PW_P2        0x13  // 'RX payload width, pipe2' register address
#define RX_PW_P3        0x14  // 'RX payload width, pipe3' register address
#define RX_PW_P4        0x15  // 'RX payload width, pipe4' register address
#define RX_PW_P5        0x16  // 'RX payload width, pipe5' register address
#define FIFO_STATUS     0x17  // 'FIFO Status Register' register address
#define PAYLOAD_WIDTH   0x1f  // 'payload length of 256 bytes modes register address

//interrupt status
#define STATUS_RX_DR 	0x40
#define STATUS_TX_DS 	0x20
#define STATUS_MAX_RT 	0x10

#define STATUS_TX_FULL 	0x01

//FIFO_STATUS
#define FIFO_STATUS_TX_REUSE 	0x40
#define FIFO_STATUS_TX_FULL 	0x20
#define FIFO_STATUS_TX_EMPTY 	0x10

#define FIFO_STATUS_RX_FULL 	0x02
#define FIFO_STATUS_RX_EMPTY 	0x01

uint8_t SPI_Read_Reg(uint8_t reg);
void SPI_Read_Buf(uint8_t reg, uint8_t *pBuf, uint8_t bytes);

void SPI_Write_Reg(uint8_t reg, uint8_t value);
//void SPI_Write_Reg_Bank0(const uint8_t reg, const uint8_t value);
//void SPI_Write_Buf(uint8_t reg, uint8_t *pBuf, uint8_t bytes);
void SPI_Write_Buf(uint8_t reg, uint8_t *pBuf, uint8_t length);
void SPI_Write_Buf_bank(uint8_t reg, uint8_t *pBuf, uint8_t length);

void SwitchToTxMode(void);
void SwitchToRxMode(void);

void SPI_Bank1_Read_Reg(uint8_t reg, uint8_t *pBuf);
void SPI_Bank1_Write_Reg(uint8_t reg, uint8_t *pBuf);
void SwitchCFG(char _cfg);

void RFM73_Initialize(void);
