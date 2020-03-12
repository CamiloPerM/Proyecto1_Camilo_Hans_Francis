/*
 * File:   Proyecto1_Slave_Temp_Mot.c
 * Author: Camilo Perafán Montoya
 *
 * Created on 1 de marzo de 2020, 06:24 AM
 */


// PIC16F887 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
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

#define _XTAL_FREQ 8000000 //Se define la frecuencia del oscilador para el delay


#include <xc.h>
#include <stdint.h>
#include "Config_ADC.h"
#include "Oscilador.h"
#include "I2C.h"
#include "LCD.h"

//******************************************************************************
//Prototipos de Funciones y Declaración de Variables
//******************************************************************************
void init (void);

float ADC_TEMP_V = 0;
float ADC_TEMP = 0;
float TEMP_diez = 0;
float TEMP_cien = 0;

int DECI_1_TEMP = 0;

uint8_t ADC_TEMP_BIN = 0;
uint8_t TEMP_EN = 0;
uint8_t TEMP_EN1 = 0;
uint8_t TEMP_EN2 = 0;
uint8_t TEMP_D1 = 0;
uint8_t temp = 0;
uint8_t DUM = 0;

//******************************************************************************
//Interrupciones
//******************************************************************************
void __interrupt() isr(void){
    //Interrupción del ADC
    
    if(ADIF){ //Si ocurrio la interrupción del ADC
        ADIF = 0; // Apague la bandera de la interrupción 
        if (ADCON0bits.GO_nDONE == 0){ //Revise si terminó la conversión
            ADC_TEMP_BIN = ADRESH; //Si la terminó, guarde el ADRESH en la variable
        }    
    }
  
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
           SSPBUF = ADC_TEMP_BIN;              //Guarde en el buffer el valor de la variable
           SSPCONbits.CKP = 1;          //Habilite la entrada de pulsos de reloj
           __delay_us(250);             //Espere 250 us
           while(SSPSTATbits.BF);       //Espere a que el envio se complete
    
       }
       
       PIR1bits.SSPIF = 0;             //Apague la bandera de la interrupción 
    
    }    
}


//******************************************************************************
//Función Principal
//******************************************************************************
void main(void) {
    initOsc(7); //Se inicializa el reloj interno a 8 MHz
    init(); //Se inicializan los puertos
    iniADC(2); //Se inicializa el ADC 
    chADC(11); //Se elige AN11 
    //LCD_INIT(); //Se inicializa la LCD    
    I2C_Slave_Init(0x50); //Inicialice I2C Slave con la dirección 0x50
    
    PORTA = 0;
    PORTD = 0; //Se empiezan todos los puertos en 0
    PORTDbits.RD2 = 1; //El valor de RD2 empieza en 1
    
    //LCD_POINT(2,2);
    //LCD_ENV_CHAR('.'); //Se envía el caracter .
    //LCD_POINT(2,4);
    //LCD_ENV_CHAR('C'); //Se envía el caracter C 
    
    while(1){     
        ADCON0bits.GO_nDONE = 1; //Empieze la conversión
        
//******************************************************************************
//Conversión de Voltaje a temperatura
//******************************************************************************        
/*
        ADC_TEMP_V = ADC_TEMP_BIN/51.00; //Obtener el valor de voltaje del ADC
        ADC_TEMP = ADC_TEMP_V/0.01; //Obtener el valor de temperatura del sensor
        
        TEMP_diez = ADC_TEMP/10.0; 
        TEMP_EN1 = (uint8_t)(TEMP_diez); //Se obtiene el valor del entero 1 de temp.
        
        TEMP_EN = (uint8_t)(ADC_TEMP);
        TEMP_EN2 = TEMP_EN%10; //Se obtiene el valor del entero 2 de temp.
        
        TEMP_cien = ADC_TEMP*10.0;
        DECI_1_TEMP = (int)(TEMP_cien);
        TEMP_D1 = DECI_1_TEMP%10; //Se obtiene el valor del decimal de temp.
        
        LCD_POINT(2,0);
        LCD_ENV_CHAR(TEMP_EN1 + 48);
        
        LCD_POINT(2,1);
        LCD_ENV_CHAR(TEMP_EN2 + 48);        
        
        LCD_POINT(2,3);
        LCD_ENV_CHAR(TEMP_D1 + 48);  
*/        
//******************************************************************************
//Control de Ventilador        
//******************************************************************************  
        if(ADC_TEMP_BIN >= 17){ //Si elvalor de temp. se pasa de 33.3°C
            PORTDbits.RD2 = 0;//Mande el trigger que active el relay
        }else{
            PORTDbits.RD2 = 1; //Mantenga el relay desactivado
        }
        
    }
    return;
}

//******************************************************************************
//Inicialización de Puertos
//******************************************************************************
void init(void){
    TRISA = 0; //PORTA configurado como salida
    TRISBbits.TRISB4 = 1; //RB4 como entrada
    TRISD = 0; //PORTD configurado como salida
    ANSEL = 0; //Puertos configurados como digitales
    ANSELH = 0b00001000; //Puertos configurados como digitales excepto AN11
    INTCON = 0b11000000; //GIE y PIE Activadas    
}
