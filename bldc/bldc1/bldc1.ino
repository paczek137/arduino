#include <avr/io.h>
#include <util/delay.h>
#include "bldc.h"
#include "bldc.c"

int main(void) {
  
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
        
        WYLACZ_TRANZYSTORY

        //test
        //faza U
        
        U_TR_G_ON
        bldc_bezpiecznik();
        _delay_ms(2000);
        U_TR_G_OFF
        bldc_bezpiecznik();
        
        _delay_ms(2000);
        
        U_TR_D_ON
        bldc_bezpiecznik();
        _delay_ms(2000);
        U_TR_D_OFF
        bldc_bezpiecznik();
        _delay_ms(2000);
        
        //faza V
        
        V_TR_G_ON
        bldc_bezpiecznik();
        _delay_ms(2000);
        V_TR_G_OFF
        bldc_bezpiecznik();
        
        _delay_ms(2000);
        
        V_TR_D_ON
        bldc_bezpiecznik();
        _delay_ms(2000);
        V_TR_D_OFF
        bldc_bezpiecznik();
        _delay_ms(2000);
        
        //faza W
        
        W_TR_G_ON
        bldc_bezpiecznik();
        _delay_ms(2000);
        W_TR_G_OFF
        bldc_bezpiecznik();
        
        _delay_ms(2000);
        
        W_TR_D_ON
        bldc_bezpiecznik();
        _delay_ms(2000);
        W_TR_D_OFF
        bldc_bezpiecznik();
        _delay_ms(2000);
        
        bldc_bezpiecznik();
        while(1);

}
