/*
 * File:   MainStepper.c
 * Author: hansb
 *
 * Created on 4 de marzo de 2020, 03:54 PM
 */

#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

#include <xc.h>
#include <xc.h>
#include <pic16f887.h>
#include <stdint.h>
#include"Stepper.h"

#define _XTAL_FREQ 8000000
#define steps 512 // how much step it will take


//***********************************************************************************************
//              PROTITPO DE FUNCIONES Y DEF. DE VARIABLES
//***********************************************************************************************
void INIT (void);



//***********************************************************************************************
//              INTERRUPCIONES
//**********************************************************************************************




//**********************************************************************************************
//              MAIN PRINCIPAL
//*********************************************************************************************

void main(void) {
    
    INIT();
    PORTA = 0;
    PORTB = 0;
    PORTC = 0;
    PORTE = 0;
    PORTD = 0;
    
    
    while(1){
        
        for(int i=0;i<steps;i++){
            WAVE_DRIVE(1);
        }
        
        __delay_ms(1000);
        
        for(int i=0;i<steps;i++){
            WAVE_DRIVE(0);
        }
        
        __delay_ms(1000);
        
        
        
    }
    
    return;
}




//********************************************************************************************
//              FUNCION DE INICIALIZACIÓN
//********************************************************************************************

void INIT (void){
    OSCCON = 0b01110001;        //FRECUENCIA DE 8MHZ Y UTILIZACIÓN DE RELOJ INTERNO
    ANSELH = 0;                 
    ANSEL = 0;
    TRISB = 0;                  //DEFINIR PORTB COMO SALIDA 
}