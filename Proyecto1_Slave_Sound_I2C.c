/*
 * File:   Proyecto1_Slave_Sound.c
 * Author: Camilo Perafán Montoya
 *
 * Created on 1 de marzo de 2020, 06:11 PM
 */

// PIC16F887 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#define _XTAL_FREQ 8000000


#include <xc.h>
#include <stdint.h>
#include "Oscilador.h"
#include "I2C.h"

//********************************************************************************************************
// Prototipos de Funciones y Declaración de Variables
//********************************************************************************************************

void init (void);

uint8_t REB = 0;
uint8_t EST = 0;
uint8_t BAND_ANTI = 0;
uint8_t temp = 0;
uint8_t DUM = 0;

//********************************************************************************************************
//Interrupciones
//********************************************************************************************************
void __interrupt() isr(void){
    if(PIR1bits.SSPIF == 1){ //Revisa si ocurrio una interrupción del MSSP
         
       SSPCONbits.CKP = 0; //Apague el reloj
       
       if ((SSPCONbits.SSPOV) || (SSPCONbits.WCOL)){ //Si ocurrio Overflow o Colisión
           temp = SSPBUF;              // Lea el valor anterior para limpiar el buffer
           SSPCONbits.SSPOV = 0;       // Limpie la bandera de Overflow
           SSPCONbits.WCOL = 0;        // Limpie el bit de colisión
           SSPCONbits.CKP = 1;         // Active el reloj
       }
        if(!SSPSTATbits.D_nA && !SSPSTATbits.R_nW) {
            temp = SSPBUF;              // Lectura del SSBUF para limpiar el buffer y la bandera BF
            PIR1bits.SSPIF = 0;         // Limpia bandera de interrupción recepción/transmisión SSP
            SSPCONbits.CKP = 1;         // Habilita entrada de pulsos de reloj SCL
            while(!SSPSTATbits.BF);     // Esperar a que la recepción se complete
            DUM = SSPBUF;               // Guardar en el PORTD el valor del buffer de recepción
            __delay_us(250);
            
        }else if(!SSPSTATbits.D_nA && SSPSTATbits.R_nW){
           temp = SSPBUF;               //Lectura del SSBUF para limpiar el buffer y la bandera BF
           BF = 0;                      //Limpie la bandera BF
           SSPBUF = EST;              //Guarde en el buffer el valor de la variable
           SSPCONbits.CKP = 1;          //Habilite la entrada de pulsos de reloj
           __delay_us(250);             //Espere 250 us
           while(SSPSTATbits.BF);       //Espere a que el envio se complete
    
       }
       
       PIR1bits.SSPIF = 0;             //Apague la bandera de la interrupción 
    
    }    
}

//********************************************************************************************************
// Función Principal
//********************************************************************************************************

void main(void) {
    initOsc(7); //Se inicializa el reloj interno a 8 MHz
    init(); // Se inicializa los puertos
    I2C_Slave_Init(0x60); //Inicialice I2C Slave con la dirección 0x50    
    
    PORTA = 0;
    PORTC = 0; //Se inicializan todos los puertos en cero
    
    while(1){
        if (PORTAbits.RA5 == 1){ // Si el sensor de sonido es activado
            BAND_ANTI = 1; //Prenda la bandera de que se activó el sensor
            __delay_ms(5); //Espere 5 ms (Para darle tiempo al programa de ajustarse)
            if (PORTAbits.RA5 == 0 && BAND_ANTI == 1) { //Pregunta si se desactivo el sensor y si la bandera está encendida, cumpliendo una función antirrebote     
                if(EST == 0){ //Si la bandera de estado está apagada
                    EST = 1; //Encienda la bandera de estado
                    PORTAbits.RA0 = 1; //Encienda RA0
                    PORTAbits.RA1 = 1; //Encienda RA1 
                    PORTAbits.RA2 = 1; //Encienda RA2
                    PORTAbits.RA3 = 1; //Encienda RA3
                    PORTAbits.RA4 = 1; //Encienda RA4

                }else{ //Si la bandera de estado está encendida
                    EST = 0; //Apague la bandera de estado
                    PORTAbits.RA0 = 0; //Encienda RA0
                    PORTAbits.RA1 = 0; //Encienda RA1
                    PORTAbits.RA2 = 0; //Encienda RA2
                    PORTAbits.RA3 = 0; //Encienda RA3
                    PORTAbits.RA4 = 0; //Encienda RA4                        
                }
            }    
        }
    }
    return;
}


//********************************************************************************************************
//Función de Inicialización de Puertos
//********************************************************************************************************

void init (void){
    TRISAbits.TRISA0 = 0; // PIN A0 de PORTA configurado como salida
    TRISAbits.TRISA1 = 0; // PIN A1 de PORTA configurado como salida
    TRISAbits.TRISA2 = 0; // PIN A2 de PORTA configurado como salida
    TRISAbits.TRISA3 = 0; // PIN A3 de PORTA configurado como salida
    TRISAbits.TRISA4 = 0; // PIN A4 de PORTA configurado como salida
    TRISAbits.TRISA5 = 1; // PIN A5 de PORTA configurado como entrada
    ANSEL = 0; // Pines connfigurados como entradas digitales
    ANSELH = 0; // Pines connfigurados como entradas digitales
    
}
