/*

Sterownik silnika BLDC.
Test bezpiecznika na diodach LED

Czêstotliwoœæ F_CPU: dowolna (ustaw w opcjach projektu)

Szczegó³y: http://mikrokontrolery.blogspot.com/2011/03/silnik-bldc-sterownik-bezpiecznik-test.html

2012 Dondu

*/


#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "bldc.h"


//--------------------------------------------------------------------

void bldc_bezpiecznik_stop(void){

	//Funkcja wy³¹cza wszelkie tranzystory oraz przechodzi w stan sygnalizacji
	//b³êdu komutacji. Funkcja ta razem z funkcj¹ bezpiecznik() pe³ni rolê
	//zabezpieczenia przeciwzwarciowego dla b³êdnie dzia³aj¹cego algorytmu
	//komutacji w czasie pisania i testów programu.

	//wy³¹cz przerwania
	cli();

	//natychmiast wy³¹cz tranzystory
	WYLACZ_TRANZYSTORY

	//i ustaw stany niskie na pinach steruj¹cych 
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

	//ustaw pin LED jako wyjœcie
	BEZP_LED_DDR	|=	(1<<BEZP_LED_PIN);

	//zatrzymaj program w pêtli nieskoñczonej sygnalizuj¹c b³¹d 
	while(1){ 

		//zmieñ stan LED na przeciwny
		BEZP_LED_PORT  ^= (1<<BEZP_LED_PIN);		

		//co 100ms
		_delay_ms(100);

	}
}


//------------------------------------------------------------------

void bldc_bezpiecznik(void){

	//Sprawdzamy, czy nie wystêpuje konflikt sterowania, powoduj¹cy
	//jednoczene otwarcie tranzystora górnego i dolnego w tej samej fazie,
	//co oznacza wyst¹pienie zwarcia !!!

	if(U_TR_G_SPRAW_STAN && U_TR_D_SPRAW_STAN){

		//Faza U - oba tranzystory s¹ w³¹czone - sytuacja niedopuszczalna!!!
		bldc_bezpiecznik_stop();

	}else if(V_TR_G_SPRAW_STAN && V_TR_D_SPRAW_STAN){

		//Faza V - oba tranzystory s¹ w³¹czone - sytuacja niedopuszczalna!!!
		bldc_bezpiecznik_stop();

	}else if(W_TR_G_SPRAW_STAN && W_TR_D_SPRAW_STAN){

		//Faza W - oba tranzystory s¹ w³¹czone - sytuacja niedopuszczalna!!!
		bldc_bezpiecznik_stop();

	}

}

