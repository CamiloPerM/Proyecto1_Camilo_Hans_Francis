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
uint8_t Valor3 = 15;
uint8_t Valor4 = 1;
uint8_t Valor5 = 1;

uint8_t controla = 255;



//-------------------------------INTERRUPCION-----------------------------------
void __interrupt() interrupciones(void){

}
    
//-----------------------------LOOP PRINCIPAL----------------------------------
void main(void) {
    init();
    initOsc(7); //Reloj de 8MHz
    UART_Init(9600); 
    
    while(1){
        
        //__delay_ms(100);
        Enviar(255);
        Enviar(Valor1);
        __delay_ms(1);
        
        Enviar(254);
        Enviar(Valor2);
        __delay_ms(1);
        
        Enviar(253);
        Enviar(Valor3);
        __delay_ms(1);
        
        Enviar(252);
        Enviar(Valor4);
        __delay_ms(1);
        
        Enviar(251);
        Enviar(Valor5);
        
        __delay_ms(500);
        
        
        // Las siguientes lineas de codigo que se encuentran comentadas era
        //solo para verificar que se mostrabam los cambios en el puerto luego de
        //un cambio en las variables
        /*
        Valor1 ++;
        Valor2 --;
        //Valor3 = Valor3 * 2;
        
        if (Valor3 == 15){
            Valor3 = 240;
        } else if (Valor3 == 240){
            Valor3 = 15;
        }
        
        if (Valor4 == 1){
            Valor4 = 0;
        } else if (Valor4 == 0){
            Valor4 = 1;
        }
        
        if (Valor5 == 1){
            Valor5 = 0;
        } else if (Valor5 == 0){
            Valor5 = 1;
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
    
    //--------------------SPI-------------------------
    /*
    spiInit(SPI_SLAVE_SS_DIS, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
    GIE = 1;         //interrupciones globales activas
    PEIE = 1;        //interrupciones perifericas activas
    SSPIF = 0;       //bandera clear
    SSPIE = 1;   //HABILITAR INTERRUPCION SPI
     * */
    
    /*OPTION_REG	 = 0x87;
    TMR0		 = 12;   
    
    INTCONbits.GIE = 1;
    INTCONbits.T0IE = 1;  //Habilitamos interrupcion del T0
    INTCONbits.T0IF = 0; //Limpiamos la bandera de interrupcion del T0*/
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

