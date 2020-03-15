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
#include"USARTinit.h"

#define _XTAL_FREQ 8000000


//***********************************************************************************************
//              PROTITPO DE FUNCIONES Y DEF. DE VARIABLES
//***********************************************************************************************
void Init(void);

void Enviar(uint8_t Venviar);
uint8_t Recibir(void);

//VARIABLES SPI (prueba)
uint8_t VERIFICADOR = 0;
uint8_t SENSOR1 = 10;
uint8_t SENSOR2 = 20;
uint8_t SENSOR3 = 30;
uint8_t SENSOR4 = 40;

//VARIABLES UART
uint8_t Valor1 = 0;
uint8_t Valor2 = 0;
uint8_t Valor3 = 0;
uint8_t Valor4 = 0;
uint8_t Valor5 = 0;

uint8_t dato = 0;
uint8_t controla = 0;


//***********************************************************************************************
//              INTERRUPCIONES
//**********************************************************************************************
void __interrupt()isr(void){
    if(SSPIF == 1){
        VERIFICADOR = spiRead();
        if(VERIFICADOR == 1){
            spiWrite(Valor1);
        }
        else if(VERIFICADOR == 2){
            spiWrite(Valor2);
        }
        else if(VERIFICADOR == 3){
            spiWrite(Valor3);
        }
        else if(VERIFICADOR == 4){
            spiWrite(Valor4);
        }
        else if(VERIFICADOR == 5){
            spiWrite(Valor5);
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
    UART_Init(9600);
    
    PORTA = 0;                      //INICIALIZAR PUERTOS     
    PORTB = 0;
    PORTC = 0;
    PORTE = 0;
    PORTD = 0;
    
    
    while(1){
        
        controla = UART_Read();
        if (controla == 255){
            Valor1 = UART_Read();
        }
        else if (controla == 254){
            Valor2 = UART_Read();
        }
        else if (controla == 253){
            Valor3 = UART_Read();
        }
        else if (controla == 252){
            Valor4 = UART_Read();
        }
        else if (controla == 251){
            Valor5 = UART_Read();
        }
        
        /*
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
            }}*/
        
        
        PORTB = Valor5;
        
        
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
    
    //-----------------UART PORT CONF------------------------
    TRISCbits.TRISC7 = 1;   //RX como entrada
    TRISCbits.TRISC6 = 1;   //QUEDA PENDIENTE
    
    TRISB = 0;                  //PUERTO COMO SALIDA (PRUEBA UART)
    
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
