/*
 * File:   mainS1proyect.c
 * Author: Francis_Sanabria
 *
 * Created on 26 de febrero de 2020, 01:37 PM
 */
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

// -------------------LIBRERIAS DEL SISTEMA-----------------------------------
#include <xc.h>
#include <stdint.h>
#include <pic16f887.h>
#include <stdio.h>

//--------------------LIBRERIAS CREADAS-------------------------------------
#include "OsciladorConf.h"
#include "I2C.h"
#include "LCD.h"
#include "KEYPAD.h"
#include "ADCconfi.h"
//-------define reloj
#define _XTAL_FREQ 500000
// --------------------------Prototipo de funciones-----------------------------
void init(void);
void PWMconf(void);



//---------------------------VARIABLES-----------------------------------------
uint8_t conteo = 0;
uint8_t res = 0;
uint8_t z;
uint8_t numerito;
int a;

//-------------------------------INTERRUPCION-----------------------------------
//-------------------------------INTERRUPCION-----------------------------------
void __interrupt() interrupciones(void){
   // Interrupcion del ADC
   
    if (PIR1bits.ADIF){
        PIR1bits.ADIF = 0;
        res = ADRESH;
        ADCON0bits.GO_nDONE = 1; 
    }
   
}
//-----------------------------LOOP PRINCIPAL----------------------------------
void main(void) {
    init();
    initOsc(3); //Reloj de 500kHz  
    funcAdc(2,0,0); //Inicia ADC
    PWMconf();
    Lcd_Init_8bits();  //Se inicializa la LCD en modo de 8bits
    T1CON = 0x10;               //Initialize Timer Module
    //InitKeypad();
    // Dato 156 es el que se da
    //Con un reloj de de 500Khz es valora cargar es de 156
    
    //---------------------Mensaje de Bienvenida----------------------------
    
    Lcd_Clear_8bits();              //Se limpia la LCD
    Lcd_Set_Cursor_8bits(1,2);      //Se pone el cursor en el punto (1,2)  
    Lcd_Write_String("Bienvenido, por",8);  //Se escribe una cadena  
    Lcd_Set_Cursor_8bits(2,1);      //Se pone el cursor en el punto (2,3) 
    Lcd_Write_String("Francis Sanabria",8);     //Se escribe una cadena
    __delay_ms(3000);               //Se espera 5 segundos  
    Lcd_Clear_8bits();              //Se limpia la pantalla de la lCD
    
    char Key = 'n';
    

    while(1){
        //Lcd_Set_Cursor_8bits(1,2);      //Se pone el cursor en el punto (1,2)  
        //Lcd_Write_String("Contraseña:",8);  //Se escribe una cadena
        //Key = switch_press_scan();
        //Lcd_Set_Cursor_8bits(2,1);
        //Lcd_Write_String(Key,8);
        
        //__delay_ms(1000);  
        //Lcd_Write_String("Funciona:",8);
        if (res < 155){
            PORTEbits.RE2 = 1;
        }else{
            PORTEbits.RE2 = 0;
        }
        PORTD = res;
        
        // ------------------- USO DEL SENSOR DISTANCIA ------------------------
        TMR1H = 0;                //Sets the Initial Value of Timer
        TMR1L = 0;                //Sets the Initial Value of Timer

        RA4 = 1;                  //TRIGGER HIGH
        __delay_us(10);           //10uS Delay 
        RA4 = 0;                  //TRIGGER LOW

        while(!RA1);              //Waiting for Echo
        TMR1ON = 1;               //Timer Starts
        while(RA1);               //Waiting for Echo goes LOW
        TMR1ON = 0;               //Timer Stops

        a = (TMR1L | (TMR1H<<8)); //Reads Timer Value
        a = a/58.82;              //Converts Time to Distance
        a = a + 1;                //Distance Calibration
        if(a>=2 && a<=400)        //Check whether the result is valid or not
        { 
          Lcd_Clear_8bits();
          Lcd_Set_Cursor_8bits(1,1);
          Lcd_Write_String("Distance = ",8);
          
          numerito = a%10 + 48;
          Lcd_Set_Cursor_8bits(1,14);
          Lcd_Write_Char(numerito,8);

          a = a/10;
          numerito = a%10 + 48;
          Lcd_Set_Cursor_8bits(1,13);
          Lcd_Write_Char(numerito,8);

          a = a/10;
          numerito = a%10 + 48;
          Lcd_Set_Cursor_8bits(1,12);
          Lcd_Write_Char(numerito,8);

          Lcd_Set_Cursor_8bits(1,15);
          Lcd_Write_String("cm",8);
        }  
        else
        {
          Lcd_Clear_8bits();
          Lcd_Set_Cursor_8bits(1,1);
          Lcd_Write_String("Out of Range",8);
        }
        __delay_ms(400);
        
    }
    
    return;
}


//------------------------FUNCION PARA INICIALIZAR PUERTOS----------------------
void init(void){
    // Definiendo entradas y salidas
    TRISB = 0;          // Puerto B como salida
    TRISA = 0x0F;              //Medio puerto A como entradas
    //TRISD = 0b01111111;              //Puerto D como entrada para KEYPAD
    TRISD = 0;
    TRISC = 0;              //Puerto C como salida 
    TRISE = 0;              //Puerto E como salida
    ANSEL = 0;
    ANSELH = 0;         // Puertos Digitales
    ANSELbits.ANS0 = 1; //Puerto RA0 como entrada analogica    
    // inicializando puertos
    PORTA = 0;
    PORTB = 0;    
    PORTD = 0;
    PORTC = 0;
    PORTE = 0;
    
    return;
}

void PWMconf(void){
    //---------------------CCP1 ---------------------------------------------
    TRISCbits.TRISC2 = 1;
    PR2 = 155;
    CCP1CONbits.P1M1 = 0;
    CCP1CONbits.P1M0 = 0; //MODO PWM SOLO PARA SALIDA EN RC2
    
    CCP1CONbits.CCP1M0 = 0;
    CCP1CONbits.CCP1M1 = 0;
    CCP1CONbits.CCP1M2 = 1;
    CCP1CONbits.CCP1M3 = 1; //MODO PWM DEL CCP1
    
    CCPR1L = 6; 
    CCP1CONbits.DC1B0 = 1;
    CCP1CONbits.DC1B1 = 1; //LSB duty cicle
    
    PIR1bits.TMR2IF = 0;
    
    
    T2CONbits.T2CKPS0 = 1;
    T2CONbits.T2CKPS1 = 1;
    
    T2CONbits.TMR2ON = 1;
    
    PIR1bits.TMR2IF = 0;
    
    TRISCbits.TRISC2 = 0;
}

