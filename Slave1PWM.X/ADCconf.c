#include <stdint.h>
#include <pic16f887.h>
#include "ADCconfi.h"

//--------------------PARAMETROS ------------------------------
//-------------------divD      -> Eleje que division del focs se desea
//-------------------anaP      -> Indica que puerto analogico inicial
//-------------------just      -> Indica la justificacion desesada
void funcAdc(uint8_t divD,uint8_t anaP,uint8_t just){
    switch(divD){
        case 0:                             // Focs/2
            ADCON0bits.ADCS0 = 0;
            ADCON0bits.ADCS1 = 0; 
            break;
        case 1:                             // Focs/8
            ADCON0bits.ADCS0 = 1;
            ADCON0bits.ADCS1 = 0; 
            break;
            
        case 2:                             // Focs/32
            ADCON0bits.ADCS0 = 0;
            ADCON0bits.ADCS1 = 1; 
            break;
            
        default:                            // FRC
            ADCON0bits.ADCS0 = 1;
            ADCON0bits.ADCS1 = 1;
            break; 
    }
    
    switch(anaP){
        case 0:                             // AN0
            ADCON0bits.CHS0 = 0;
            ADCON0bits.CHS1 = 0;
            ADCON0bits.CHS2 = 0;
            ADCON0bits.CHS3 = 0;  // Solo Habilita el PUerto ANS0
            break;
            
        case 1:                             // AN1
            ADCON0bits.CHS0 = 1;
            ADCON0bits.CHS1 = 0;
            ADCON0bits.CHS2 = 0;
            ADCON0bits.CHS3 = 0;  // Solo Habilita el PUerto ANS8
            break;
            
        case 2:                             // AN2
            ADCON0bits.CHS0 = 0;
            ADCON0bits.CHS1 = 1;
            ADCON0bits.CHS2 = 0;
            ADCON0bits.CHS3 = 0;  // Solo Habilita el PUerto ANS8
            break;
            
        default:                            // AN8
            ADCON0bits.CHS0 = 0;
            ADCON0bits.CHS1 = 0;
            ADCON0bits.CHS2 = 0;
            ADCON0bits.CHS3 = 1;  // Solo Habilita el PUerto ANS8
            break;
    }
    
    switch(just){
        case 0:                             // AN0
            ADCON1bits.ADFM = 0; //Justificado a la izquierda
            break;
        default:
            ADCON1bits.ADFM = 1; //Justificado a la derecha
            break;
    }
            
            
    ADCON0bits.ADON = 1; //Enciende el ADC
    //------------------Configuracion de las interrupciones del ADC------------- 
    PIR1bits.ADIF = 0; //Limpia la bandera del ADC
    PIE1bits.ADIE = 1; //Habilita interrupciones del ADC
    INTCONbits.PEIE = 1; //Habilita interrupciones perifericas
    INTCONbits.GIE = 1; //Habilita interrupciones globales
    
    ADCON0bits.GO_nDONE = 1;    
    
    return;
}