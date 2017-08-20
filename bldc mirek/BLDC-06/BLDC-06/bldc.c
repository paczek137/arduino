/*

Sterownik silnika BLDC.
Test bezpiecznika na diodach LED

Cz�stotliwo�� F_CPU: dowolna (ustaw w opcjach projektu)

Szczeg�y: http://mikrokontrolery.blogspot.com/2011/03/silnik-bldc-sterownik-bezpiecznik-test.html

2012 Dondu

*/


#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "bldc.h"


//--------------------------------------------------------------------

void bldc_bezpiecznik_stop(void){

	//Funkcja wy��cza wszelkie tranzystory oraz przechodzi w stan sygnalizacji
	//b��du komutacji. Funkcja ta razem z funkcj� bezpiecznik() pe�ni rol�
	//zabezpieczenia przeciwzwarciowego dla b��dnie dzia�aj�cego algorytmu
	//komutacji w czasie pisania i test�w programu.

	//wy��cz przerwania
	cli();

	//natychmiast wy��cz tranzystory
	WYLACZ_TRANZYSTORY

	//i ustaw stany niskie na pinach steruj�cych 
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

	//ustaw pin LED jako wyj�cie
	BEZP_LED_DDR	|=	(1<<BEZP_LED_PIN);

	//zatrzymaj program w p�tli niesko�czonej sygnalizuj�c b��d 
	while(1){ 

		//zmie� stan LED na przeciwny
		BEZP_LED_PORT  ^= (1<<BEZP_LED_PIN);		

		//co 100ms
		_delay_ms(100);

	}
}


//------------------------------------------------------------------

void bldc_bezpiecznik(void){

	//Sprawdzamy, czy nie wyst�puje konflikt sterowania, powoduj�cy
	//jednoczene otwarcie tranzystora g�rnego i dolnego w tej samej fazie,
	//co oznacza wyst�pienie zwarcia !!!

	if(U_TR_G_SPRAW_STAN && U_TR_D_SPRAW_STAN){

		//Faza U - oba tranzystory s� w��czone - sytuacja niedopuszczalna!!!
		bldc_bezpiecznik_stop();

	}else if(V_TR_G_SPRAW_STAN && V_TR_D_SPRAW_STAN){

		//Faza V - oba tranzystory s� w��czone - sytuacja niedopuszczalna!!!
		bldc_bezpiecznik_stop();

	}else if(W_TR_G_SPRAW_STAN && W_TR_D_SPRAW_STAN){

		//Faza W - oba tranzystory s� w��czone - sytuacja niedopuszczalna!!!
		bldc_bezpiecznik_stop();

	}

}

