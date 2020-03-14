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
#pragma config BOREN = OFF       // Brown Out Reset Selection bits (BOR enabled)
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
void initTMR0(void);
//---------------------------VARIABLES-----------------------------------------
uint8_t conteo = 0;
uint8_t res = 0;
uint8_t z;
uint8_t numerito;
uint8_t contraAct = 0;
uint8_t cont = 0;
uint8_t i = 0;
uint8_t valor1 = 0;
uint8_t valor2 = 0;
uint8_t Abierto = 0;
int a;
//char contrasena[]= {1+48,7+48,3+48,0+48,8+48};
char contrasena[]= {49,55,51,48,56};
char compara[]= {0,0,0,0,0};

//-------------------------------INTERRUPCION-----------------------------------
void __interrupt() interrupciones(void){
    //--------------------- Interrupcion del I2C
    if(PIR1bits.SSPIF == 1){ 

        SSPCONbits.CKP = 0;
       
        if ((SSPCONbits.SSPOV) || (SSPCONbits.WCOL)){
            z = SSPBUF;                 // Read the previous value to clear the buffer
            SSPCONbits.SSPOV = 0;       // Clear the overflow flag
            SSPCONbits.WCOL = 0;        // Clear the collision bit
            SSPCONbits.CKP = 1;         // Enables SCL (Clock)
        }
        //------------------Lee un dato----------------------------------------
        if(!SSPSTATbits.D_nA && !SSPSTATbits.R_nW) {
            //__delay_us(7);
            z = SSPBUF;                 // Lectura del SSBUF para limpiar el buffer y la bandera BF
            //__delay_us(2);
            PIR1bits.SSPIF = 0;         // Limpia bandera de interrupción recepción/transmisión SSP
            SSPCONbits.CKP = 1;         // Habilita entrada de pulsos de reloj SCL
            while(!SSPSTATbits.BF);     // Esperar a que la recepción se complete
            PORTD = SSPBUF;             // Guardar en el PORTD el valor del buffer de recepción
            __delay_us(250);
        //------------------Escribe un dato----------------------------------------  
            
        }else if(!SSPSTATbits.D_nA && SSPSTATbits.R_nW){
            z = SSPBUF;
            BF = 0;
            //SSPBUF = PORTB;
            SSPBUF = Abierto;
            SSPCONbits.CKP = 1;
            __delay_us(250);
            while(SSPSTATbits.BF);
        }   
        PIR1bits.SSPIF = 0;    
    }
    
   // Interrupcion del ADC
   
    if (PIR1bits.ADIF){
        PIR1bits.ADIF = 0;
        res = ADRESH;
        if (res < 155){
            PORTEbits.RE2 = 1;
        }else{
            PORTEbits.RE2 = 0;
        }
        ADCON0bits.GO_nDONE = 1; 
    }
    //-------------INTERRUP TMR0
    if (INTCONbits.TMR0IF ){ 
        INTCONbits.TMR0IF = 0;
        TMR0		 = 200;
        
        if (PORTAbits.RA1 == 1){
            CCPR1L = 15;
            Abierto = 1;
        } else if (PORTAbits.RA2 == 1){
            CCPR1L = 6;
            Abierto = 0;
        }
        
    
    }

   
}
//-----------------------------LOOP PRINCIPAL----------------------------------
void main(void) {
    init();
    initOsc(3); //Reloj de 500kHz  
    funcAdc(2,0,0); //Inicia ADC
    PWMconf();
    Lcd_Init_8bits();  //Se inicializa la LCD en modo de 8bits
    InitKeypad();
    initTMR0();
    I2C_Slave_Init(0x70);
    
    //---------------------Mensaje de Bienvenida----------------------------
    
    Lcd_Clear_8bits();              //Se limpia la LCD
    Lcd_Set_Cursor_8bits(1,1);      //Se pone el cursor en el punto (1,2)  
    Lcd_Write_String("Bienvenido Atte.",8);  //Se escribe una cadena
    Lcd_Set_Cursor_8bits(2,1);      //Se pone el cursor en el punto (2,3) 
    Lcd_Write_String("Familia Sanabria",8);     //Se escribe una cadena
    __delay_ms(3000);               //Se espera 5 segundos  
    Lcd_Clear_8bits();              //Se limpia la pantalla de la lCD
    
    
    char Key = 'n';
    

    while(1){
        Inicio:
        Lcd_Set_Cursor_8bits(1,1);      //Se pone el cursor en el punto (1,2)  
        Lcd_Write_String("Presione # para",8);  //Se escribe una cadena
        Lcd_Set_Cursor_8bits(2,1);      //Se pone el cursor en el punto (2,3) 
        Lcd_Write_String("iniciar",8);     //Se escribe una cadena
        
        Presiona:
        Key = switch_press_scan();
        
        if (Key == 35){
            Intentelo: //SE reinicia la cuetna para que se ingrese la contrasena
            
            contraAct = 1;
            cont = 0;
            Lcd_Clear_8bits();
            Lcd_Set_Cursor_8bits(1,1);      //Se pone el cursor en el punto (1,2)  
            Lcd_Write_String("Ingrese clave:",8);  //Se escribe una cadena
            
            IngresarC:
            
            Key = switch_press_scan();
            Lcd_Set_Cursor_8bits(2,cont + 1); //Va moviendo el cursor poco a poco
            Lcd_Write_Char(Key,8);
            cont ++;
            
            if (Key == 42){ //Si el usuario se confundio y quiere reiniciar la cuenta
                goto Intentelo;
            }
            
            compara[cont - 1] = Key; //Gurada en el arreglo cada tecla que se inicia
            if (cont == 5){ //Si se llena el arreglo compara
                __delay_ms(1000);
                for (int i = 0; i < 5; i++) { //Compara los 5 valores 1 por 1
                // Obtener elementos de ambos arreglos en misma posición o índice
                    int valor1 = contrasena[i], valor2 = compara[i];

        // Comparar 
                    if (valor1 != valor2) { //Si al menos un valor no coincide la contrasena esta mala
                        Lcd_Clear_8bits();
                        Lcd_Set_Cursor_8bits(1,1);      //Se pone el cursor en el punto (1,2)  
                        Lcd_Write_String("Contrasena mala",8);  //Se escribe una cadena
                        
                        Lcd_Set_Cursor_8bits(2,1);      //Se pone el cursor en el punto (1,2)  
                        Lcd_Write_String("intentelo",8);  //Se escribe una cadena
                        __delay_ms(3000);
                        goto Intentelo;
                        
                    }
                }
                goto Bueno; //Si no tuvo errores en la comparacion se va a otro segmento
                
            }
            
            
            
            if (contraAct == 1){
                goto IngresarC;
            }
            
        } else {
            goto Presiona;
        } 
 
        Bueno: //muestra que funciono la contrasena
        
        Lcd_Clear_8bits();
        Lcd_Set_Cursor_8bits(1,1);      //Se pone el cursor en el punto (1,2)  
        Lcd_Write_String("Acceso brindado,",8);  //Se escribe una cadena
        Lcd_Set_Cursor_8bits(2,1);      //Se pone el cursor en el punto (1,2)  
        Lcd_Write_String("Introduzca llave",8);  //Se escribe una cadena
        //Ahora el usuario puede ingresar la llave
        while (PORTEbits.RE2 == 0){
            Key = keypad_scanner();
            if (Key == 42){ //Si olvido la llave puede apachar *  para reiniciar la contrasena
                goto Intentelo;
            }
        }
        Lcd_Clear_8bits();
        Lcd_Set_Cursor_8bits(1,1);      //Se pone el cursor en el punto (1,2)  
        Lcd_Write_String("Abriendo puerta",8);  //Se escribe una cadena
        Lcd_Set_Cursor_8bits(2,1);      //Se pone el cursor en el punto (1,2)  
        Lcd_Write_String("Bienvenido",8);  //Se escribe una cadena
        
        CCPR1L = 15; //Abre la puerta
        Abierto = 1;
        
        __delay_ms(5000);
        Lcd_Clear_8bits();
        CCPR1L = 6; //Cierra la puerta
        Abierto = 0;
        goto Inicio;
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

void initTMR0(void){
    OPTION_REG	 = 0x85;
    TMR0 = 200;     
    
    INTCONbits.GIE = 1;
    INTCONbits.T0IE = 1;  //Habilitamos interrupcion del T0
    INTCONbits.T0IF = 0; //Limpiamos la bandera de interrupcion del T0
    
    return;
}