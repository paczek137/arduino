/*

Sterownik silnika BLDC.
Test bezpiecznika na diodach LED

Czêstotliwoœæ F_CPU: dowolna (ustaw w opcjach projektu)

Szczegó³y: http://mikrokontrolery.blogspot.com/2011/03/silnik-bldc-sterownik-bezpiecznik-test.html

2012 Dondu

*/


#include <avr/io.h>
#include <util/delay.h>
#include "bldc.h"


//------------------------------------------------------------------

int main (void) 
{

	//ustaw stan pocz¹tkowy wyjœæ struj¹cych tranzystorami
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
	//Testuj program dowolnie steruj¹c tranzystorami usuwaj¹c znak komentarza
	//w poni¿szych instrukcjach, obserwuj¹c diody LED poszczególnych 
	//tranzystorów. Stan zabroniony powinien zostaæ z³apany przez funkcjê
	//bezpiecznik() i sygnalizowany jej diod¹ LED.


	//faza U tranzystor górny
	//U_TR_G_ON;
	//U_TR_G_OFF;

	//faza U tranzystor dolny
	//U_TR_D_ON;
	//U_TR_D_OFF;

	//faza V tranzystor górny
	//V_TR_G_ON;
	//V_TR_G_OFF;

	//faza V tranzystor dolny
	//V_TR_D_ON;
	//V_TR_D_OFF;

	//faza W tranzystor górny
	//W_TR_G_ON;
	//W_TR_G_OFF;

	//faza W tranzystor dolny
	//W_TR_D_ON;
	//W_TR_D_OFF;


	//sprawdzamy, czy nie ma stanu zabronionego na tranzystorach
	//ZAWSZE WYWO£UJ TÊ FUNKCJÊ, GDY ZMIENIASZ STAN TRANZYSTORÓW!!!
	bldc_bezpiecznik();
	

	//pêtla g³ówna
	while(1);	

}






