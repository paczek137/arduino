/*
//----cac ham co trong thu vien--------------------------------------------- 
void GLCD_Delay(void)				
void GLCD_OUT_Set(void)				
void GLCD_IN_Set(void)				
void GLCD_SetSide(char Side)		
void wait_GLCD(void)				
void GLCD_SetDISPLAY(uint8_t ON)	
void GLCD_SetYADDRESS(uint8_t Col)	//Set dia chi cot Y
void GLCD_SetXADDRESS(uint8_t Line)	//set dia chi page X
void GLCD_StartLine(uint8_t Offset)	//set start line khi cuon GLCD
void GLCD_WriteDATA(uint8_t DATA)	//ghi du lieu vao GLCD
uint8_t GLCD_ReadDATA(void)			//doc du lieu tu GLCD
void GLCD_Init(void)				//khoi dong GLCD
void GLCD_GotoXY(uint8_t Line, uint8_t Col)	//Di chuyen den vi tri co to do page, cot tren man hinh
void GLCD_Clr(void)							//Xoa toan bo GLCD
void GLCD_PutChar78(uint8_t Line, uint8_t Col, uint8_t chr)		//In ky tu kich thuoc 7x8
void GLCD_Print78(uint8_t Line, uint8_t Col, char* str)			//In chuoi ky tu 7x8
void GLCD_PutBMP(char *bmp)										//in hinh anh len GLCD
void GLCD_PutPixel(uint8_t x, uint8_t y){//(x, y) toa do diem can ve. [0:127, 0:63]
void GLCD_ClrPixel(uint8_t x, uint8_t y){//(x, y) toa do diem can ve. [0:127, 0:63]
void GLCD_PutLine(uint8_t x1, uint8_t y1, uint8_t x2,uint8_t y2){  // Ve 1 duong thang len GLCD
void GLCD_ClrLine(uint8_t x1, uint8_t y1,uint8_t x2,uint8_t y2){  // Xoa 1 duong thang tren GLCD
int GLCD_PutNum(unsigned char x,unsigned char y, unsigned int num) //line,colum,character
*/

#include <math.h>
#include "font.h"
#include <avr/pgmspace.h>

#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |=  _BV(bit))

//define the data port (D7:0)
#define GLCD_DATA_O  	PORTD  // Ouput Data bus
#define GLCD_DATA_I	  	PIND   // Input Data bus
#define GLCD_DATA_DDR  	DDRD   // Direction

//define the control port (RS, RW, E, CS1, CS2)
#define GLCD_CTRL_O		PORTB 
#define GLCD_CTRL_I 	PINB 
#define GLCD_CTRL_DDR 	DDRB
//GLCD control pins
#define GLCD_E	 		1// Enable line
#define GLCD_RW 		2 // read/write selecting line
#define GLCD_RS 		3 // Register select: Data/Instruction (DI)
#define GLCD_CS1        5 // left/right selecting line#1
#define GLCD_CS2        4 // left/right selecting line#2

//define the macro to enable/disable GLCD
#define GLCD_ENABLE		sbi(GLCD_CTRL_O,GLCD_E)
#define GLCD_DISABLE	cbi(GLCD_CTRL_O,GLCD_E)

//Instruction code
#define GLCD_DISPLAY	0x3E //0011111x: display, the last bit is ON or OFF
#define GLCD_YADDRESS	0x40 //01xxxxxx: Set Y (column) address
#define GLCD_XADDRESS	0xB8 //10111xxx: set X (page, line) address
#define GLCD_STARTLINE	0xC0 //11xxxxxx: Display start line or page scroll, xxxxxx is the offset of scroll
#define GLCD_BUSY		7 //bit Busy in Status 

//*************************supporting functions*****************************
void GLCD_Delay(void){
	for(uint8_t i=0; i<16; i++) asm volatile ("nop"::);								
}						 					 					 		 
void GLCD_OUT_Set(void){						 					 	
	GLCD_CTRL_DDR |=						 					 		
	(1<<GLCD_E)|(1<<GLCD_RW)|(1<<GLCD_RS)|(1<<GLCD_CS1)|(1<<GLCD_CS2);	
	cbi(GLCD_CTRL_O, GLCD_E);	
	GLCD_DATA_DDR=0xFF;
	GLCD_DATA_O=0x00;
}
void GLCD_IN_Set(void){
	GLCD_CTRL_DDR |=
	(1<<GLCD_E)|(1<<GLCD_RW)|(1<<GLCD_RS)|(1<<GLCD_CS1)|(1<<GLCD_CS2);	
	cbi(GLCD_CTRL_O, GLCD_E);	
	GLCD_DATA_DDR=0x00;
	GLCD_DATA_O=0xFF; ////pull up resistors
}
void GLCD_SetSide(char Side){	//Left or right controller
	GLCD_OUT_Set();
	if(Side==1){ //1:Right
		cbi(GLCD_CTRL_O, GLCD_CS1);
		sbi(GLCD_CTRL_O, GLCD_CS2);
	}
	else{        //0: Left
		sbi(GLCD_CTRL_O, GLCD_CS1);
		cbi(GLCD_CTRL_O, GLCD_CS2);
	}
	GLCD_Delay();
}
//*************************END supporting functions**************************

//*************************primary functions of GLCD*************************
//-------------------------For INSTRUCTION---------------------
void wait_GLCD(void){
	GLCD_IN_Set();
	cbi(GLCD_CTRL_O, GLCD_RS); //pull both RS down
	sbi(GLCD_CTRL_O, GLCD_RW); //pull both RW up, (GLCD->AVR)

	GLCD_ENABLE;		   //Pull the EN line up
	GLCD_Delay();
	GLCD_DISABLE;
	
	while (bit_is_set(GLCD_DATA_I,GLCD_BUSY)){
		GLCD_ENABLE;		   //Pull the EN line up		
		GLCD_Delay();
		GLCD_DISABLE;		   //Pull the EN line down						
	}
}
void GLCD_SetDISPLAY(uint8_t ON){
	wait_GLCD();
	GLCD_OUT_Set();
	cbi(GLCD_CTRL_O, GLCD_RS); //pull both RS, RW down, (AVR->GLCD)
	cbi(GLCD_CTRL_O, GLCD_RW);
	
	GLCD_DATA_O=GLCD_DISPLAY+ON;
	GLCD_ENABLE;		   //Pull the EN line up
	GLCD_Delay();
	GLCD_DISABLE;		   //Pull the EN line down
}
void GLCD_SetYADDRESS(uint8_t Col){ //set Y address (or column) of GLCD
	wait_GLCD();
	GLCD_OUT_Set();
	cbi(GLCD_CTRL_O, GLCD_RS); //pull both RS, RW down, (AVR->GLCD)
	cbi(GLCD_CTRL_O, GLCD_RW);
	
	GLCD_DATA_O=GLCD_YADDRESS+Col;
	GLCD_ENABLE;		   //Pull the EN line up	
	GLCD_Delay();
	GLCD_DISABLE;		   //Pull the EN line down
}
void GLCD_SetXADDRESS(uint8_t Line){ //set X address (or line) of GLCD
	wait_GLCD();
	GLCD_OUT_Set();
	cbi(GLCD_CTRL_O, GLCD_RS); //pull both RS, RW down,(AVR->GLCD)
	cbi(GLCD_CTRL_O, GLCD_RW);
	
	GLCD_DATA_O=GLCD_XADDRESS+Line;
	GLCD_ENABLE;		   //Pull the EN line up
	GLCD_Delay();
	GLCD_DISABLE;		   //Pull the EN line down		
}
void GLCD_StartLine(uint8_t Offset){ //set Y address (or column) of GLCD
	wait_GLCD();
	GLCD_OUT_Set();
	cbi(GLCD_CTRL_O, GLCD_RS); //pull both RS, RW down, (AVR->GLCD)
	cbi(GLCD_CTRL_O, GLCD_RW);
	
	GLCD_SetSide(0);
	GLCD_DATA_O=GLCD_STARTLINE+Offset;
	GLCD_ENABLE;		   //Pull the EN line up	
	GLCD_Delay();
	GLCD_DISABLE;		   //Pull the EN line down
	
	GLCD_SetSide(1);
	GLCD_ENABLE;		   //Pull the EN line up		
	GLCD_DATA_O=GLCD_STARTLINE+Offset;
	GLCD_Delay();
	GLCD_DISABLE;		   //Pull the EN line down
}
//-------------------------END For INSTRUCTION---------------------

//-------------------------For DATA---------------------
//write Data to GLCD: used in "Write Display Data" only
void GLCD_WriteDATA(uint8_t DATA){	
	wait_GLCD();
	GLCD_OUT_Set();
	sbi(GLCD_CTRL_O, GLCD_RS); //pull RS up
	cbi(GLCD_CTRL_O, GLCD_RW); //RW down,  (AVR->GLCD)
	GLCD_DATA_O=DATA;	    // Put Data out
	
	GLCD_ENABLE;		   //Pull the EN line up	
	GLCD_Delay();
	GLCD_DISABLE;		   //Pull the EN line down
	GLCD_Delay();
	
}
//Read DATA from GLCD: used for "Read Display Data" only
uint8_t GLCD_ReadDATA(void){
	uint8_t DATA;
	wait_GLCD();
	GLCD_IN_Set();
	sbi(GLCD_CTRL_O, GLCD_RS); //pull both RS up	
	sbi(GLCD_CTRL_O, GLCD_RW); //pull both RW up, (GLCD->AVR)
	
	GLCD_ENABLE;		   //Pull the EN line up
	GLCD_Delay();		
	
	GLCD_DISABLE;		    //[new] ket thuc lan doc 1 (doc gia - dummy read)	
	GLCD_Delay();			//[new]	cho du lieu tu GLCD RAM duoc day ra thanh ghi output		
	GLCD_ENABLE;		   	//[new] Pull the EN line up	(chuan bi cho lan 2)	
	
	DATA=GLCD_DATA_I;	   // get Data
	GLCD_DISABLE;		   //Pull the EN line down
	GLCD_Delay();
			
	return DATA;
}
//*************************END primary functions of GLCD**********************


//*************************Direct used Function*******************************
//----Initialize GLCD---------------------
void GLCD_Init(void){
	GLCD_SetSide(0); // left side
	GLCD_SetDISPLAY(1);
	GLCD_SetYADDRESS(0);
	GLCD_SetXADDRESS(0);
	GLCD_StartLine(0);
	
	GLCD_SetSide(1); //right side
	GLCD_SetDISPLAY(1);
	GLCD_SetYADDRESS(0);
	GLCD_SetXADDRESS(0);
	GLCD_StartLine(0);
}
//------move the pointer to the X, Y position---------
void GLCD_GotoXY(uint8_t Line, uint8_t Col){ //Line:0-7, Col: 0-127
	uint8_t Side;
	Side=Col/64 ; //Select controller, 0:Left, 1: Right
	GLCD_SetSide(Side);
	Col -= 64*Side; //Update real Col : 0-63
	GLCD_SetYADDRESS(Col);
	GLCD_SetXADDRESS(Line);	
}
void GLCD_Clr(void){
	uint8_t Line, Col;		
	for (Line=0; Line<8; Line++){
		GLCD_GotoXY(Line,0);		
		for (Col=0; Col<64; Col++) GLCD_WriteDATA(0);
	}
	for (Line=0; Line<8; Line++){
		GLCD_GotoXY(Line,64);		
		for (Col=0; Col<64; Col++) GLCD_WriteDATA(0);
	}	
}
//-------Print a character with 7x8 size onto GLCD-------
void GLCD_PutChar78(uint8_t Line, uint8_t Col, uint8_t chr){
	uint8_t i;	
	if ((Col>57) && (Col<64)){ //there is a "jump" from left->right
		GLCD_GotoXY(Line, Col);    //left first
		for(i=0;i<64-Col;i++) 
			GLCD_WriteDATA(pgm_read_byte(&font7x8[((chr - 32) * 7) + i]));
		GLCD_GotoXY(Line, 64);   // then right
		for(i=64-Col;i<7;i++) 
			GLCD_WriteDATA(pgm_read_byte(&font7x8[((chr - 32) * 7) + i]));
	}
	else{
		GLCD_GotoXY(Line, Col);
		for(i=0;i<7;i++) 
			GLCD_WriteDATA(pgm_read_byte(&font7x8[((chr - 32) * 7) + i]));
	}
}
//print an array of character onto GLCD----
void GLCD_Print78(uint8_t Line, uint8_t Col, char* str){
	uint8_t i, x;
	x=Col;
	for (i=0; str[i]!=0; i++){
		if (x>=128){
			Col=0;
			x=Col;
			Line++;
		}
		GLCD_PutChar78(Line, x , str[i]);	 
		x+=8;
    }
}
//-----fill GLCD with a bipmap size 128x64-----------
void GLCD_PutBMP(char *bmp){
	uint8_t Line, Col;
	uint16_t i;
	for (Line=0; Line<8; Line++){
		GLCD_GotoXY(Line,0);		
		for (Col=0; Col<64; Col++){ //Left LCD
			i=128*Line+Col;
			GLCD_WriteDATA(pgm_read_byte(&bmp[i]));
		}
		GLCD_GotoXY(Line,64);
		for (Col=64; Col<128; Col++){ //Right LCD
			i=128*Line+Col;
			GLCD_WriteDATA(pgm_read_byte(&bmp[i]));
		}			
	}
}

//-----------V2-------------------------------------------------------------------
#define abs(n)          __builtin_abs ((n))

void GLCD_PutPixel(uint8_t x, uint8_t y){//(x, y) toa do diem can ve. [0:127, 0:63] 
	uint8_t GLCD_oldData, Line;
	y=63-y; //mang goc toa do xuong duoi
	Line=y>>3; //tuong duong y/8
	y &=0x07; //chon 3 bit thap cua y, tuong duong phep chia lay phan du cua y cho 8
	
	GLCD_GotoXY(Line,x); //di chuyen den Line (page) can ghi/doc
	GLCD_oldData=GLCD_ReadDATA(); //doc du lieu tu GLCD Ram, tai hang Line, cot x	
	sbi(GLCD_oldData, y);//set bit thu y cua page hien tai
	GLCD_GotoXY(Line,x); //di chuyen den Line (page) can ghi/doc
	GLCD_WriteDATA(GLCD_oldData);
}

void GLCD_ClrPixel(uint8_t x, uint8_t y){//x: cot thu 0:127, y hang thu 0:63
	uint8_t GLCD_oldData, Line;
	y=63-y; //mang goc toa do xuong duoi
	Line=y>>3; //tuong duong y/8
	y &=0x07; //chon 3 bit thap cua y, tuong duong phep chia lay phan du cua y cho 8
	
	GLCD_GotoXY(Line,x); //di chuyen den Line (page) can ghi/doc
	GLCD_oldData=GLCD_ReadDATA(); //doc du lieu tu GLCD Ram, tai hang Line, cot x
	cbi(GLCD_oldData, y); //xoa bit thu y cua page hien tai
	GLCD_GotoXY(Line,x); //di chuyen den Line (page) can ghi/doc
	GLCD_WriteDATA(GLCD_oldData); 
}

void GLCD_PutLine(uint8_t x1, uint8_t y1, uint8_t x2,uint8_t y2){  // Ve 1 duong thang len GLCD
  uint8_t x,y;	
  if(abs(x2-x1)>=abs(y2-y1)){ //truong hop truc x lon hon truc y
		if (x2<x1){ //hoan doi vi tri 2 diem
			x = x1;
			y = y1;
			x1 = x2;
			y1 = y2;
			x2 = x;
			y2 = y;
		}
		for(x=x1;x<=x2;++x){		
			if(y1>=y2)	y = y1 - (x-x1)*(y1-y2)/(x2-x1);
			else y = y1 + (x-x1)*(y2-y1)/(x2-x1);			
			GLCD_PutPixel(x, y);			
		}
	}	
	else{ //truong hop truc y lon hon truc x
		if (y2<y1){
			x = x1;
			y = y1;
			x1 = x2;
			y1 = y2;
			x2 = x;
			y2 = y;
		};		
		for(y=y1;y<=y2;y++){
			if(x1>=x2)	x = x1 - (y-y1)*(x1-x2)/(y2-y1);
			else x = x1 + (y-y1)*(x2-x1)/(y2-y1);
			GLCD_PutPixel(x,y);
		}
	}
}

void GLCD_ClrLine(uint8_t x1, uint8_t y1,uint8_t x2,uint8_t y2){  // Xoa 1 duong thang tren GLCD    
  uint8_t x,y;
  if(abs(x1-x2)>=abs(y1-y2)){
	  if (x2<x1){
		x = x1;
		y = y1;
		x1 = x2;
		y1 = y2;
		x2 = x;
		y2 = y; 
	  };
	  for(x=x1;x<=x2;x++){
		if(y1>=y2)	y = y1 - (x-x1)*(y1-y2)/(x2-x1);
		else y = y1 + (x-x1)*(y2-y1)/(x2-x1);
		GLCD_ClrPixel(x,y);
		}		
		
	}
	else{
		if (y2<y1){
			x = x1;
			y = y1;
			x1 = x2;
			y1 = y2;
			x2 = x;
			y2 = y; 
		};		
        for(y=y1;y<=y2;y++){
			if(x1>=x2)	x = x1 - (y-y1)*(x1-x2)/(y2-y1);
			else x = x1 + (y-y1)*(x2-x1)/(y2-y1);
			GLCD_ClrPixel(x,y);
		}	
	}
}


// GLCD put number
int GLCD_PutNum(unsigned char x,unsigned char y, unsigned int num)
{
    int a,b;
        	a=num/1000 ;
        	GLCD_PutChar78(x,y,a+0x030);
        	b=num%1000;
			a=b/100;
        	GLCD_PutChar78(x,y+8,a+0x030);
			a=b%100;
			b=a/10;
			GLCD_PutChar78(x,y+16,b+0x030);
			b=a%10;
			GLCD_PutChar78(x,y+24,b+0x30);

	return 0;
}
