/*
 * File:   mainLab2.c
 * Author: Francis_Sanabria
 *
 * Created on 28 de enero de 2020, 03:48 PM
 */

//Este es el nuevo proyecto

// PIC16F887 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = ON       // Brown Out Reset Selection bits (BOR enabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)
// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

// -------------------LIBRERIAS DEL SISTEMA-----------------------------------
#include <xc.h>
#include <stdint.h>

//--------------------LIBRERIAS CREADAS-------------------------------------
#include "OsciladorConf.h"
#include "SPI.h"
#include "USARTinit.h"

//-------define reloj
#define _XTAL_FREQ 8000000
// --------------------------Prototipo de funciones-----------------------------
void init(void);
void Enviar(uint8_t Venviar);
uint8_t Recibir(void);

//---------------------------VARIABLES-----------------------------------------
uint8_t Valor1 = 0;
uint8_t Valor2 = 0;
uint8_t Valor3 = 0;
uint8_t Valor4 = 0;
uint8_t Valor5 = 0;

uint8_t dato = 0;
uint8_t controla = 0;

uint8_t z = 0;


//-------------------------------INTERRUPCION-----------------------------------
void __interrupt() interrupciones(void){
    if(RCIF==1){
        
    }
    //----------SPI-----------------------------
    /*if(SSPIF == 1){
        instruc = spiRead();
        if (instruc == 0){
            spiWrite(valor1);
        } else if (instruc == 1){
            spiWrite(valor2);
        }
        SSPIF = 0;
    }*/
}
    
//-----------------------------LOOP PRINCIPAL----------------------------------
void main(void) {
    init();
    initOsc(7); //Reloj de 8MHz
    UART_Init(9600); 
    
    while(1){
        // ------------------------ DATO 1 -------------------------------------
        if (PIR1bits.RCIF){
            controla = RCREG;
        }
        if (controla == 255){
            //__delay_ms(10);
            if (PIR1bits.RCIF){
                Valor1 = RCREG;
            }}
        // ------------------------ DATO 2 -------------------------------------
        if (PIR1bits.RCIF){
            controla = RCREG;
        }
        if (controla == 254){
            //__delay_ms(10);
            if (PIR1bits.RCIF){
                Valor2 = RCREG;
            }}
        
        // ------------------------ DATO 3 -------------------------------------
        
        if (PIR1bits.RCIF){
            controla = RCREG;
        }
        if (controla == 253){
            //__delay_ms(10);
            if (PIR1bits.RCIF){
                Valor3 = RCREG;
            }}
        
        // ------------------------ DATO 4 -------------------------------------
        
        if (PIR1bits.RCIF){
            controla = RCREG;
        }
        if (controla == 252){
            //__delay_ms(10);
            if (PIR1bits.RCIF){
                Valor4 = RCREG;
            }}
        
        // ------------------------ DATO 5 -------------------------------------
        
        if (PIR1bits.RCIF){
            controla = RCREG;
        }
        if (controla == 251){
            //__delay_ms(10);
            if (PIR1bits.RCIF){
                Valor5 = RCREG;
            }}
        //Las siguientes lineas de codigo que se encuentran comentadas se usaban
        //solo para verificar el funcionamiento del UART mostrando datos en el puerto
        /*
        PORTA = Valor1; 
        PORTB = Valor2;
        PORTD = Valor3;
        

        
        if (Valor4 == 1){
            PORTEbits.RE0 =1;
        }else {
            PORTEbits.RE0 =0;
        }
        
        
        if (Valor5 == 1){
            PORTEbits.RE1 =1;
        }else {
            PORTEbits.RE1 =0;
        }*/

    }
    
    return;
}


//------------------------FUNCION PARA INICIALIZAR PUERTOS----------------------
void init(void){
    // Definiendo entradas y salidas
    TRISA = 0;              //Puerto A como salidas
    TRISB = 0;          ////Puerto B como salida para 
    TRISD = 0;              //Puerto D como salida para
    TRISC = 0;              //Puerto C como SALIDA
    TRISE = 0;              //Puerto E como salida
    
    //-----------------UART PORT CONF------------------------
    TRISCbits.TRISC7 = 1;   //RX como entrada
    TRISCbits.TRISC6 = 1;   //QUEDA PENDIENTE
    
    
    //-----------------SPI PORT CONF------------------------
    TRISCbits.TRISC5 = 0; //Serial data out de SPI
    TRISCbits.TRISC4 = 1; //Serial data in de SPI
    TRISCbits.TRISC3 = 1; //entrada clock del MASTER
    
    
    ANSEL = 0;
    ANSELH = 0;         // Puertos Digitales

    // inicializando puertos
    PORTA = 0;
    PORTB = 0;    
    PORTD = 0;
    PORTC = 0;
    PORTE = 0;
    
    
    //PIE1bits.RCIE = 1;
    //INTCONbits.GIE = 1;
    //INTCONbits.PEIE = 1;
    //--------------------SPI-------------------------
    /*
    spiInit(SPI_SLAVE_SS_DIS, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
    GIE = 1;         //interrupciones globales activas
    PEIE = 1;        //interrupciones perifericas activas
    SSPIF = 0;       //bandera clear
    SSPIE = 1;   //HABILITAR INTERRUPCION SPI
     * */
    return;
}


void Enviar(uint8_t Venviar){
    TXREG = Venviar;
    Esperar:
    if (PIR1bits.TXIF == 0){
        goto Esperar;
        }
}
uint8_t Recibir(void){
    if (PIR1bits.RCIF){
            uint8_t dato = 0;
            dato = RCREG;
            return dato;
        }
}

