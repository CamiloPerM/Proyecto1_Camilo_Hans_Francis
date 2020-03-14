/*
 * File:   Main2SPI.c
 * Author: hansb
 *
 * Created on 14 de marzo de 2020, 10:22 AM
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
#include <pic16f887.h>
#include <stdint.h>
#include"SPI.h"

#define _XTAL_FREQ 8000000


//***********************************************************************************************
//              PROTITPO DE FUNCIONES Y DEF. DE VARIABLES
//***********************************************************************************************
void Init(void);

//VARIABLES SPI
uint8_t VERIFICADOR = 0;
uint8_t SENSOR1 = 10;
uint8_t SENSOR2 = 20;
uint8_t SENSOR3 = 30;
uint8_t SENSOR4 = 40;




//***********************************************************************************************
//              INTERRUPCIONES
//**********************************************************************************************
void __interrupt()isr(void){
    if(SSPIF == 1){
        VERIFICADOR = spiRead();
        if(VERIFICADOR == 1){
            spiWrite(SENSOR1);
        }
        else if(VERIFICADOR == 2){
            spiWrite(SENSOR2);
        }
        else if(VERIFICADOR == 3){
            spiWrite(SENSOR3);
        }
        else if(VERIFICADOR == 4){
            spiWrite(SENSOR4);
        }
        
        SSPIF = 0;
    }
    
}


//**********************************************************************************************
//              MAIN PRINCIPAL
//*********************************************************************************************

void main(void) {
    Init();
    spiInit(SPI_SLAVE_SS_DIS, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_HIGH, SPI_ACTIVE_2_IDLE);        //INICIALIZAR SPI 
    
    PORTA = 0;                      //INICIALIZAR PUERTOS     
    PORTB = 0;
    PORTC = 0;
    PORTE = 0;
    PORTD = 0;
    
    
    while(1){
        
    }
    
    
    return;
}


//********************************************************************************************
//              FUNCION DE INICIALIZACIÓN
//********************************************************************************************
void Init(void){
    OSCCON = 0b01110001;        //FRECUENCIA DE 8MHZ Y UTILIZACIÓN DE RELOJ INTERNO
    ANSEL = 0;                  //PUERTOS COMO SALIDAS DIGITALES
    ANSELH = 0;
    
    INTCON = 0b11000000;        //HABILITAR GIE, PEIE
    
    PIR1bits.SSPIF = 0;         //Borrar bandera MSP
    PIE1bits.SSPIE = 1;         //Habilitar interrupción MSP
    
}
