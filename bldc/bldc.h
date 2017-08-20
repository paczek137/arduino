/*

Sterownik silnika BLDC.
Test bezpiecznika na diodach LED

Czêstotliwoœæ F_CPU: dowolna (ustaw w opcjach projektu)

Szczegó³y: http://mikrokontrolery.blogspot.com/2011/03/silnik-bldc-sterownik-bezpiecznik-test.html

2012 Dondu

*/


//--- D E F I N I C J E   D O T.  B E Z P I E C Z N I K A  ------

//LED bezpiecznika
#define BEZP_LED_DDR		DDRD
#define BEZP_LED_PORT		PORTD
#define BEZP_LED_PIN		PD3

//--- D E F I N I C J E   D O T.  S I L N I K A  ------

extern void bldc_bezpiecznik_stop(void);

