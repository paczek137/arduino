/*

Sterownik silnika BLDC.
Test bezpiecznika na diodach LED

Częstotliwość F_CPU: dowolna (ustaw w opcjach projektu)

Szczegóły: http://mikrokontrolery.blogspot.com/2011/03/silnik-bldc-sterownik-bezpiecznik-test.html

2012 Dondu

*/


//--- D E F I N I C J E   D O T.  B E Z P I E C Z N I K A  ------

//LED bezpiecznika
#define BEZP_LED_DDR		DDRD
#define BEZP_LED_PORT		PORTD
#define BEZP_LED_PIN		PORTD3

//--- D E F I N I C J E   D O T.  S I L N I K A  ------

//Faza U
//tranzystor górny
#define U_TR_G_PORTx		PORTB
#define U_TR_G_DDRx			DDRB
#define U_TR_G_PINx			PINB
#define U_TR_G_PIN			PORTB1
#define U_TR_G_USTAW_DDR	U_TR_G_DDRx  |=  (1<<U_TR_G_PIN); //ustaw port
#define U_TR_G_PIN_L		U_TR_G_PORTx &= ~(1<<U_TR_G_PIN); //ustaw niski
#define U_TR_G_ON 			U_TR_G_PORTx |=  (1<<U_TR_G_PIN); //włącz tranz.
#define U_TR_G_OFF			U_TR_G_PORTx &= ~(1<<U_TR_G_PIN); //wyłącz tranz.
#define U_TR_G_SPRAW_STAN	(U_TR_G_PINx  &  (1<<U_TR_G_PIN)) //warunek stanu
//tranzystor dolny
#define U_TR_D_PORTx		PORTD
#define U_TR_D_DDRx			DDRD
#define U_TR_D_PINx			PIND
#define U_TR_D_PIN			PORTD4
#define U_TR_D_USTAW_DDR	U_TR_D_DDRx  |=  (1<<U_TR_D_PIN); //ustaw port
#define U_TR_D_PIN_L		U_TR_D_PORTx &= ~(1<<U_TR_D_PIN); //ustaw niski
#define U_TR_D_ON 			U_TR_D_PORTx |=  (1<<U_TR_D_PIN); //włącz tranz.
#define U_TR_D_OFF			U_TR_D_PORTx &= ~(1<<U_TR_D_PIN); //wyłącz tranz.
#define U_TR_D_SPRAW_STAN	(U_TR_D_PINx  &  (1<<U_TR_D_PIN)) //warunek stanu

//Faza V
//tranzystor górny
#define V_TR_G_PORTx		PORTB
#define V_TR_G_DDRx			DDRB
#define V_TR_G_PINx			PINB
#define V_TR_G_PIN			PORTB3
#define V_TR_G_USTAW_DDR	V_TR_G_DDRx  |=  (1<<V_TR_G_PIN); //ustaw port
#define V_TR_G_PIN_L		V_TR_G_PORTx &= ~(1<<V_TR_G_PIN); //ustaw niski
#define V_TR_G_ON 			V_TR_G_PORTx |=  (1<<V_TR_G_PIN); //włącz tranz.
#define V_TR_G_OFF			V_TR_G_PORTx &= ~(1<<V_TR_G_PIN); //wyłącz tranz.
#define V_TR_G_SPRAW_STAN	(V_TR_G_PINx  &  (1<<V_TR_G_PIN)) //warunek stanu
//tranzystor dolny
#define V_TR_D_PORTx		PORTD
#define V_TR_D_DDRx			DDRD
#define V_TR_D_PINx			PIND
#define V_TR_D_PIN			PORTD5
#define V_TR_D_USTAW_DDR	V_TR_D_DDRx  |=  (1<<V_TR_D_PIN); //ustaw port
#define V_TR_D_PIN_L		V_TR_D_PORTx &= ~(1<<V_TR_D_PIN); //ustaw niski
#define V_TR_D_ON 			V_TR_D_PORTx |=  (1<<V_TR_D_PIN); //włącz tranz.
#define V_TR_D_OFF			V_TR_D_PORTx &= ~(1<<V_TR_D_PIN); //wyłącz tranz.
#define V_TR_D_SPRAW_STAN	(V_TR_D_PINx  &  (1<<V_TR_D_PIN)) //warunek stanu


//Faza W
//tranzystor górny
#define W_TR_G_PORTx		PORTB
#define W_TR_G_DDRx			DDRB
#define W_TR_G_PINx			PINB
#define W_TR_G_PIN			PORTB2
#define W_TR_G_USTAW_DDR	W_TR_G_DDRx  |=  (1<<W_TR_G_PIN); //ustaw port
#define W_TR_G_PIN_L		W_TR_G_PORTx &= ~(1<<W_TR_G_PIN); //ustaw niski
#define W_TR_G_ON 			W_TR_G_PORTx |=  (1<<W_TR_G_PIN); //włącz tranz.
#define W_TR_G_OFF			W_TR_G_PORTx &= ~(1<<W_TR_G_PIN); //wyłącz tranz.
#define W_TR_G_SPRAW_STAN	(W_TR_G_PINx  &  (1<<W_TR_G_PIN)) //warunek stanu 
//tranzystor dolny
#define W_TR_D_PORTx		PORTD
#define W_TR_D_DDRx			DDRD
#define W_TR_D_PINx			PIND
#define W_TR_D_PIN			PORTD7
#define W_TR_D_USTAW_DDR	W_TR_D_DDRx  |=  (1<<W_TR_D_PIN); //ustaw port
#define W_TR_D_PIN_L		W_TR_D_PORTx &= ~(1<<W_TR_D_PIN); //ustaw niski
#define W_TR_D_ON 			W_TR_D_PORTx |=  (1<<W_TR_D_PIN); //włącz tranz.
#define W_TR_D_OFF			W_TR_D_PORTx &= ~(1<<W_TR_D_PIN); //wyłącz tranz.
#define W_TR_D_SPRAW_STAN	(W_TR_D_PINx &   (1<<W_TR_D_PIN)) //warunek stanu



//Wspólna definicja wyłączająca wszystkie tranzystory
#define WYLACZ_TRANZYSTORY	U_TR_G_OFF; U_TR_D_OFF; V_TR_G_OFF; V_TR_D_OFF; W_TR_G_OFF; W_TR_D_OFF; 			

extern void bldc_bezpiecznik(void);
extern void bldc_bezpiecznik_stop(void);


