/*
 * File:   manL3master.c
 * Author: Francis_Sanabria
 *
 * Created on 11 de febrero de 2020, 11:50 AM
 */

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF       // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
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
//-------define reloj

// -------------------LIBRERIAS DEL SISTEMA-----------------------------------
#include <xc.h>
#include <stdint.h>
//#include <pic16f887.h>
#include <stdio.h>
#include <stdlib.h>
//--------------------LIBRERIAS CREADAS-------------------------------------
#include "OsciladorConf.h"
#include "SPI.h"
#include "LCD.h"

#define _XTAL_FREQ 8000000
#include <xc.h>

// --------------------------Prototipo de funciones-----------------------------
void init(void);
void Enviar(int Venviar);

//---------------------------VARIABLES-----------------------------------------
uint8_t dato = 0;
uint8_t contador = 0;
uint8_t valor1 = 0;
uint8_t valor2 = 0;
uint8_t valor3 = 0;
uint8_t valor4 = 0;
uint8_t valor5 = 0;
uint8_t controla = 0;

unsigned int temp;

int data1;
int data2;
int data3;
int temp1 = 0;
int porcentaje = 0;

uint8_t datat1 = 0;
uint8_t datat2 = 0;
uint8_t datat3 = 0;


//float ADC_TEMP_V = 0.0;
//float ADC_TEMP = 0.0;


void float_to_string(float val1);
void num_to_string(int numero);
void ConvNum(int digito, char punt);

void main(void) {
    init();
    initOsc(7); //Reloj de 8MHz
    Lcd_Init_8bits();  //Se inicializa la LCD en modo de 8bits
    /*
    Lcd_Clear_8bits();              //Se limpia la LCD
    Lcd_Set_Cursor_8bits(1,1);      //Se pone el cursor en el punto (1,2)  
    Lcd_Write_String(" Datos  Rasp",8);  //Se escribe una cadena
    Lcd_Set_Cursor_8bits(2,1);      //Se pone el cursor en el punto (2,3) 
    Lcd_Write_String("Mostrar aca",8);     //Se escribe una cadena
    __delay_ms(3000);               //Se espera 5 segundos  
    Lcd_Clear_8bits();              //Se limpia la pantalla de la lCD
    */
     //---------------------ESTO SIEMPRE APARECERA EN PANTALLA --------------
    Lcd_Set_Cursor_8bits(1,1);      //Se pone el cursor en el punto (1,3)  
    Lcd_Write_String(" S1:   S2:  S3: ",8);  //Se escribe una cadena
    
    Lcd_Set_Cursor_8bits(2,3);                //Se posiciona el cursor en el punto 2,4
    Lcd_Write_String("S",8);                  //Se pone las unidades de voltaje
    Lcd_Set_Cursor_8bits(2,7);                //Se posiciona el cursor en el punto 2,4
    Lcd_Write_String(".",8);                  //Se pone las unidades de voltaje
    
    Lcd_Set_Cursor_8bits(2,9);  
    Lcd_Write_Char(223,8);
    Lcd_Set_Cursor_8bits(2,10);                //Se posiciona el cursor en el punto 2,4
    Lcd_Write_String("C",8);                  //Se pone las unidades de voltaje
    //LOOP PRINCPAL
    while (1){
        
        
        PORTEbits.RE2 =0;       //Slave select 

        spiWrite(0x01);        //indicar al otro pic que quiero Recibir pot 1
        valor1= spiRead();     //Recibir el valor del pot 1
        __delay_ms(5);
        
        spiWrite(0x02);        //indicar al otro pic que quiero Recibir pot 1
        valor2= spiRead();     //Recibir el valor del pot 1
        __delay_ms(5);
        
        spiWrite(0x03);        //indicar al otro pic que quiero Recibir pot 1
        valor3= spiRead();     //Recibir el valor del pot 1
        __delay_ms(5);
        /*
        spiWrite(0x04);        //indicar al otro pic que quiero Recibir pot 1
        valor4= spiRead();     //Recibir el valor del pot 1
        __delay_ms(5);
        
        spiWrite(0x05);        //indicar al otro pic que quiero Recibir pot 1
        valor5= spiRead();     //Recibir el valor del pot 1
        __delay_ms(5);
        */
        PORTEbits.RE2 = 1;       //Slave Deselect
        
        
        PORTA = valor2;       //En el inicio es Valor1 
        PORTD = valor3;       //En el inicio es Valor2
        //-----------Valor del sensor 1-------------
        num_to_string(valor2);
        ConvNum(data1,2);
        ConvNum(data2,1);
        //-----------Valor del sensor 2-------------
        /*ADC_TEMP_V = ADC_TEMP_BIN/51.00; //Obtener el valor de voltaje del ADC
        ADC_TEMP = ADC_TEMP_V/0.01; //Obtener el valor de temperatura del sensor
        
        float_to_string(ADC_TEMP);
        ConvNum(data1,4);
        ConvNum(data2,3);
        ConvNum(data3,1);*/
        Lcd_Set_Cursor_8bits(2,5);                //Se posiciona el cursor en el punto 2,4
        Lcd_Write_String("2",8);                  //Se pone las unidades de voltaje
        
         Lcd_Set_Cursor_8bits(2,6);                //Se posiciona el cursor en el punto 2,4
        Lcd_Write_String("6",8);                  //Se pone las unidades de voltaje
        
         Lcd_Set_Cursor_8bits(2,8);                //Se posiciona el cursor en el punto 2,4
        Lcd_Write_String("4",8);                  //Se pone las unidades de voltaje
        
        //-----------Valor del sensor 3-------------
        if (valor1 == 1){ //En el inicio Valor3
            Lcd_Set_Cursor_8bits(2,14);      //Se pone el cursor en el punto (2,3) 
            Lcd_Write_String("ON ",8);     //Se escribe una cadena
            PORTCbits.RC0 =1; 
        }else {
            Lcd_Set_Cursor_8bits(2,14);      //Se pone el cursor en el punto (2,3) 
            Lcd_Write_String("OFF",8);     //Se escribe una cadena
            PORTCbits.RC0 =0;
        }
        
        
        
        
        __delay_ms(50);
        

    }
    
        
    return;
}

void init(void){
    // Definiendo entradas y salidas
    TRISA = 0;              //Puerto A como salida para mostrar un valor del POT1
    TRISB = 0;          ////Puerto B como salida para mostrar dato cargado de la compu
    TRISD = 0;              //Puerto D como salida para para mostrar el valor del POT2
   
    //Puerto c como puerto de comunicacion
    TRISC = 0;              //Puerto C como RX y TX
    TRISCbits.TRISC7 = 1;   //RX como entrada
    TRISCbits.TRISC6 = 1;   //TX como entrada -> el software lo cambia
    
    TRISCbits.TRISC5 = 0; //Serial data out de SPI
    TRISCbits.TRISC4 = 1; //Serial data in de SPI
    TRISCbits.TRISC3 = 0; //Salida de reloj del master
    
    
    TRISE = 0;              //Puerto E como salida, utilizar el bit 1 como SS
    
    ANSEL = 0;
    ANSELH = 0;         // Puertos Digitales
    // inicializando puertos
    PORTA = 0;
    PORTB = 0;    
    PORTD = 0;
    PORTC = 0;
    PORTE = 0;
    
    
    
    //
    spiInit(SPI_MASTER_OSC_DIV4, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
    return;
}
//-------------------------------Funcion que convierte el valor a string
void float_to_string(float val1){
    int data;
    data = val1*100.f;  //254.00
    data = data/1;      //254
    data1 = data%10;       //residuo 4
    data = data/10;     //25
    data2 = data%10;       //residuo 5
    data3 = data/10;       //2
}

void num_to_string(int numero){
    data1 = numero%10;       //residuo 4
    numero = numero/10;     //25
    data2 = numero%10;       //residuo 5
    data3 = numero/10;       //2
}


void ConvNum(int digito, char punt){
    Lcd_Set_Cursor_8bits(2,punt);
    int numerito = 0;
    numerito = digito + 48;
    Lcd_Write_Char(numerito,8);
}