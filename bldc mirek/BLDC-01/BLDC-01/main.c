/*

Sterownik silnika BLDC.
Test bezpiecznika na diodach LED

Cz�stotliwo�� F_CPU: dowolna (ustaw w opcjach projektu)

Szczeg�y: http://mikrokontrolery.blogspot.com/2011/03/silnik-bldc-sterownik-bezpiecznik-test.html

2012 Dondu

*/


#include <avr/io.h>
#include <util/delay.h>
#include "bldc.h"


//------------------------------------------------------------------

int main (void) 
{

	//ustaw stan pocz�tkowy wyj�� struj�cych tranzystorami
	U_TR_G_USTAW_DDR
	V_TR_G_USTAW_DDR
	W_TR_G_USTAW_DDR
	U_TR_D_USTAW_DDR
	V_TR_D_USTAW_DDR
	W_TR_D_USTAW_DDR
	U_TR_G_PIN_L
	V_TR_G_PIN_L
	W_TR_G_PIN_L
	U_TR_D_PIN_L
	V_TR_D_PIN_L
	W_TR_D_PIN_L


	//na wszelki wypadek
	WYLACZ_TRANZYSTORY


	//UWAGA!!!
	//Testuj program dowolnie steruj�c tranzystorami usuwaj�c znak komentarza
	//w poni�szych instrukcjach, obserwuj�c diody LED poszczeg�lnych 
	//tranzystor�w. Stan zabroniony powinien zosta� z�apany przez funkcj�
	//bezpiecznik() i sygnalizowany jej diod� LED.


	//faza U tranzystor g�rny
	//U_TR_G_ON;
	//U_TR_G_OFF;

	//faza U tranzystor dolny
	//U_TR_D_ON;
	//U_TR_D_OFF;

	//faza V tranzystor g�rny
	//V_TR_G_ON;
	//V_TR_G_OFF;

	//faza V tranzystor dolny
	//V_TR_D_ON;
	//V_TR_D_OFF;

	//faza W tranzystor g�rny
	//W_TR_G_ON;
	//W_TR_G_OFF;

	//faza W tranzystor dolny
	//W_TR_D_ON;
	//W_TR_D_OFF;


	//sprawdzamy, czy nie ma stanu zabronionego na tranzystorach
	//ZAWSZE WYWO�UJ T� FUNKCJ�, GDY ZMIENIASZ STAN TRANZYSTOR�W!!!
	bldc_bezpiecznik();
	

	//p�tla g��wna
	while(1);	

}






