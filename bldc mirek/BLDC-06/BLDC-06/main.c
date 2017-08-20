/*

Sterownik silnika BLDC - Test drajwera

Czêstotliwoœæ F_CPU: dowolna (ustaw w opcjach projektu)

Szczegó³y: http://mikrokontrolery.blogspot.com/2011/03/Silnik-BLDC-driver-drajwer-schemat-plytka-pcb.html

luty 2014 Dondu

*/


#include <avr/io.h>
#include <util/delay.h>
#include "bldc.h" //pliki bldc.h oraz bldc.c znajdziesz w za³¹czniku do artyku³u


int main (void) 
{

 //ustaw stan pocz¹tkowy wyjœæ steruj¹cych tranzystorami
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


 //pêtla g³ówna
 while(1){
 
  //faza U tranzystor górny
  U_TR_G_ON;
  _delay_ms(150);
  U_TR_G_OFF;

  //faza U tranzystor dolny
  U_TR_D_ON;
  _delay_ms(150);
  U_TR_D_OFF;

  //faza V tranzystor górny
  V_TR_G_ON;
  _delay_ms(150);
  V_TR_G_OFF;

  //faza V tranzystor dolny
  V_TR_D_ON;
  _delay_ms(150);
  V_TR_D_OFF;

  //faza W tranzystor górny
  W_TR_G_ON;
  _delay_ms(150);
  W_TR_G_OFF;

  //faza W tranzystor dolny
  W_TR_D_ON;
  _delay_ms(150);
  W_TR_D_OFF; 
 
 }; 

}
