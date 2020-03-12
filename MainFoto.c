/*
 * File:   MainFoto.c
 * Author: hansb
 *
 * Created on 1 de marzo de 2020, 12:47 PM
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

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.


#include <xc.h>
#include <pic16f887.h>
#include <stdint.h>
#include"ADC.h"
#include"I2C.h"
#include"Stepper.h"



#define _XTAL_FREQ 8000000
#define steps 255 // how much step it will take

//***********************************************************************************************
//              PROTITPO DE FUNCIONES Y DEF. DE VARIABLES
//***********************************************************************************************
void INICIAR(void);

uint8_t ValorFoto = 0;
uint8_t te = 0;
uint8_t BASURA = 0;
uint8_t BANDERA1 = 0;
uint8_t BANDERA2 = 0;


//***********************************************************************************************
//              INTERRUPCIONES
//**********************************************************************************************
void __interrupt()isr(void){
    
    if(ADIF == 1){                          //INTERRUPCIÓN ADC
        if(ADCON0bits.GO == 0){
            ValorFoto = ADRESH;
        }
        ADIF = 0;
    }
    
     if(PIR1bits.SSPIF == 1){                //INTERRUPCIÓN PARA COMUNICACIÓN I2C

        SSPCONbits.CKP = 0;
       
        if ((SSPCONbits.SSPOV) || (SSPCONbits.WCOL)){
            te = SSPBUF;                 // Read the previous value to clear the buffer
            SSPCONbits.SSPOV = 0;       // Clear the overflow flag
            SSPCONbits.WCOL = 0;        // Clear the collision bit
            SSPCONbits.CKP = 1;         // Enables SCL (Clock)
        }

        if(!SSPSTATbits.D_nA && !SSPSTATbits.R_nW) {                //LEER DESDE EL MASTER
            //__delay_us(7);
            te = SSPBUF;                 // Lectura del SSBUF para limpiar el buffer y la bandera BF
            //__delay_us(2);
            PIR1bits.SSPIF = 0;         // Limpia bandera de interrupción recepción/transmisión SSP
            SSPCONbits.CKP = 1;         // Habilita entrada de pulsos de reloj SCL
            while(!SSPSTATbits.BF);     // Esperar a que la recepción se complete
            BASURA = SSPBUF;             // Guardar en el PORTD el valor del buffer de recepción
            __delay_us(250);
            
        }else if(!SSPSTATbits.D_nA && SSPSTATbits.R_nW){            //ESCRIBIR HACIA EL MASTER
            te = SSPBUF;
            BF = 0;
            SSPBUF = ValorFoto;                                    //COLOCAR AQUI LA VARIABLE QUE EN ENVIARÁ
            SSPCONbits.CKP = 1;
            __delay_us(250);
            while(SSPSTATbits.BF);
        }
       
        PIR1bits.SSPIF = 0;    
    }
    
}


//**********************************************************************************************
//              MAIN PRINCIPAL
//*********************************************************************************************

void main(void) {
    INICIAR();
    I2C_Slave_Init(0x40);               //DIRECCIÓN DEL SLAVE
    ADC(41);
    
    PORTA = 0;
    PORTB = 0;
    PORTC = 0;
    PORTE = 0;
    PORTD = 0;
     
    while(1){
        ADCON0bits.GO = 1;              //QUE SIEMPRE ESTE REALIZANDO LA CONVERSIÓN
        
       
        if(ValorFoto > 179 & BANDERA1 ==0){
            BANDERA1 = 1;
            BANDERA2 = 0;
            for(int i=0;i<steps;i++){
                WAVE_DRIVE(1);
                
            }   
        }
       
        __delay_ms(1000);
        
        if(ValorFoto < 179 & BANDERA2 == 0){
            BANDERA1 = 0;
            BANDERA2 = 1;
            for(int i=0;i<steps;i++){
                WAVE_DRIVE(0);
            }
        }   
        
    }
    
    
    
    return;
}



//********************************************************************************************
//              FUNCION DE INICIALIZACIÓN
//********************************************************************************************

void INICIAR(void){
 
    OSCCON = 0b01110001;        //FRECUENCIA DE 8MHZ Y UTILIZACIÓN DE RELOJ INTERNO
    ANSELH = 0;                 
    ANSEL = 0;
    
    TRISBbits.TRISB0 = 0;       //DEFINIR PORTB COMO SALIDA PARA MOTOR STEPPER
    TRISBbits.TRISB1 = 0;
    TRISBbits.TRISB2 = 0;
    TRISBbits.TRISB3 = 0;
    
    TRISBbits.TRISB5 = 1;       //DEFINIR COMO ENTRADA PARA LA FOTORRESISTENCIA
    ANSELHbits.ANS13 = 1;       //ENTRADA ANALÓGICA ANS13
    INTCON = 0b11000000;        //HABILITAR INTERUPCCIONES GIE Y PEIE
    
    
    
}