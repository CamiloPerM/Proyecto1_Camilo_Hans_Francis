/*
 * File:   MainMaster.c
 * Author: hansb
 *
 * Created on 29 de febrero de 2020, 06:25 PM
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
#include"I2C.h"
#include"LCD.h"

#define _XTAL_FREQ 8000000


//***********************************************************************************************
//              PROTITPO DE FUNCIONES Y DEF. DE VARIABLES
//***********************************************************************************************
void Init(void);
int BCD_2_DEC(int to_convert);
int DEC_2_BCD (int to_convert);
void SET_TIME_DATE();
void Update_Current_Date_Time();

int sec = 0;                //ESTABLECER LA FECHA Y HORA ACTUALES
int min = 59;
int hour = 22;
int date = 29;
int month = 02;
int year = 20;

//*******************************************************************************
//                              VARIABLES RTC
//******************************************************************************
uint8_t sec_0 = 0;
uint8_t sec_1 = 0;
uint8_t min_0 = 0;
uint8_t min_1 = 0;
uint8_t hour_0 = 0;
uint8_t hour_1 = 0;
uint8_t date_0 = 0;
uint8_t date_1 = 0;
uint8_t month_0 = 0;
uint8_t month_1 = 0;
uint8_t year_0 = 0;
uint8_t year_1 = 0;

//******************************************************************************
//                              VARIABLES FOTORESISTENCIA
//*******************************************************************************
uint8_t ADC_FOTO = 0;
float VAR1 = 0;
float VAR2 = 0;
uint8_t ILU_ENT = 0;
float ILU_TEN = 0;
int DEC1_ILUM = 0;
uint8_t DECIMAL1 = 0;
uint8_t ILUM_E_U = 0;
float ILU_E_1 = 0;
uint8_t ILU_E_1_2 = 0;
uint8_t ILU_E_DEC = 0;
float ILU_E_2 = 0;
uint8_t ILU_E_CENT = 0;

//******************************************************************************
//                              VARIABLES TEMPERATURA
//******************************************************************************

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

//*********************************************************************************
//                      SENSOR DE RUIDO
//*********************************************************************************

uint8_t ESTADO_RUIDO = 0;


//**********************************************************************************
//                      ESTADOS
//*********************************************************************************

uint8_t AntiB = 0;

uint8_t ESTADOS = 0;


//***********************************************************************************************
//              INTERRUPCIONES
//**********************************************************************************************

void __interrupt()isr(void){
    if(RBIF==1){                                    //INTERRUPCIÓN PARA REALIZAR EL CONTADOR
    RBIF=0;
        if(AntiB==0){
            if(PORTBbits.RB0==1){                   //REVISAR SI EL BOTON DE INCREMENTO FUE PRESIONADO
                AntiB=1;
                ESTADOS++;                      //INCREMENTAR EL CONTADOR
            }
            
        }    
        
    }
    
    
}





//**********************************************************************************************
//              MAIN PRINCIPAL
//*********************************************************************************************

void main(void) {
    Init();                         //FUNCION DE INICILIZACIÓN DE PUERTOS Y RELOJ INTERNO
    LCD_INI();                      //INICIALIZAR LCD
    I2C_Master_Init(100000);        //INICIALIZAR COMUNICACIÓN I2C
    
    PORTA = 0;                      //INICIALIZAR PUERTOS     
    PORTB = 0;
    PORTC = 0;
    PORTE = 0;
    PORTD = 0;
    
    SET_TIME_DATE();                //SET TIME AND DATE ON THE RTC MODULE
    CLEAR_LCD();                    //LIMPIAR LCD
    
    
    while(1){
        
        AntiB = 0;
        
        if(ESTADOS >5){
            ESTADOS = 0;
        }
        

        Update_Current_Date_Time();              //LEER LOS DATOS PROVENIENTES DEL RTC
        
        //DIVISIÓN DE UNIDADES Y DECENAS
        sec_0 = sec%10;
        sec_1 = (sec/10);
        min_0 = min%10;
        min_1 = min/10;
        hour_0 = hour%10;
        hour_1 = hour/10;
        date_0 = date%10;
        date_1 = date/10;
        month_0 = month%10;
        month_1 = month/10;
        year_0 = year%10;
        year_1 = year/10;
        
        //LECTURA DE DATOS DE LA FOTORESISTENCIA
            
        I2C_Master_Start();
        I2C_Master_Write(0x41);                 //INICIAR COMUNICACIÓN CON EL SLAVE DE LA FOTORESISTENCIA
        ADC_FOTO = I2C_Master_Read(0);          //GUARDAR DATOS PROVENIENTES DEL MÓDULO 
        I2C_Master_Stop();                      //DETENER COMUNICACIÓN
        __delay_ms(10);     
            
            
        //CONVERSIÓN AL PORCENTAJE DE ILUMINACIÓN
            
        VAR1 = 100.0/255;
        VAR2 = VAR1*ADC_FOTO;       //PORCENTAJE DE ILUMINACIÓN

        ILU_ENT = (int)(VAR2);      //OBTENER EL VALOR ENTERO DEL PORCENTAJE DE ILUMINACIÓN
        ILU_TEN = VAR2*10.0;        //MULTIPLICAR POR 10 
        DEC1_ILUM = (int)(ILU_TEN); //OBTENER ENTERO DEL NÚMERO ANTERIOR
        DECIMAL1 = (uint8_t)(DEC1_ILUM%10);     //PRIMER DECIMAL DEL PORCENTAJE

        ILUM_E_U = ILU_ENT%10;      //UNIDADES DEL NÚMERO ENTERO
        ILU_E_1 = ILU_ENT/10.0;
        ILU_E_1_2 = (int)(ILU_E_1);
        ILU_E_DEC = ILU_E_1_2%10;       //DECENAS DEL ENTERO
        ILU_E_2  = ILU_ENT/100.0;
        ILU_E_CENT = (int)(ILU_E_2);    //CENTENAS DEL ENTERO
        
        //LECTURA DE DATOS TEMPERATURA
        
        I2C_Master_Start();
        I2C_Master_Write(0x51);                 //INICIAR COMUNICACIÓN CON EL SLAVE DE TEMPERATURA
        ADC_TEMP_BIN = I2C_Master_Read(0);      //GUARDAR DATOS PROVENIENTES DEL PIC DE TEMPERATURA
        I2C_Master_Stop();                      //DETENER COMUNICACIÓN
        __delay_ms(10);

        //CONVERSIÓN Y ENVIO DE DATOS A LA LCD

        ADC_TEMP_V = ADC_TEMP_BIN/51.00; //Obtener el valor de voltaje del ADC
        ADC_TEMP = ADC_TEMP_V/0.01; //Obtener el valor de temperatura del sensor

        TEMP_diez = ADC_TEMP/10.0; 
        TEMP_EN1 = (uint8_t)(TEMP_diez); //Se obtiene el valor del entero 1 de temp.

        TEMP_EN = (uint8_t)(ADC_TEMP);
        TEMP_EN2 = TEMP_EN%10; //Se obtiene el valor del entero 2 de temp.

        TEMP_cien = ADC_TEMP*10.0;
        DECI_1_TEMP = (int)(TEMP_cien);
        TEMP_D1 = DECI_1_TEMP%10; //Se obtiene el valor del decimal de temp.
        
        
         //LECTURA SENSOR DE SONIDO
        I2C_Master_Start();
        I2C_Master_Write(0x61);                 //INICIAR COMUNICACIÓN CON EL SLAVE DE TEMPERATURA
        ESTADO_RUIDO = I2C_Master_Read(0);      //GUARDAR DATOS PROVENIENTES DEL PIC DE TEMPERATURA
        I2C_Master_Stop();                      //DETENER COMUNICACIÓN
        __delay_ms(10);

        
    //****************************************************************************************
    //                      ESTADOS
    //****************************************************************************************
       
        if(ESTADOS == 0){                  //ESTADO PARA EL MÓDULO RTC
            CLEAR_LCD();
            LCD_SET_CURSOR(1,1);
            LCD_STRING("HORA:");
            LCD_SET_CURSOR(2,1);
            LCD_STRING("FECHA:");
            
        //ENVIAR LOS DATOS DE HORA A LA LCD
            LCD_SET_CURSOR(1,6);
            LCD_1CH(hour_1 + 48);
            LCD_1CH(hour_0 + 48);
            LCD_1CH(':');
            LCD_1CH(min_1 + 48);
            LCD_1CH(min_0 + 48);
            LCD_1CH(':');
            LCD_1CH(sec_1 + 48);
            LCD_1CH(sec_0 + 48);
        
        //ENVIAR LOS DATOS DE LA FECHA A LA LCD
            LCD_SET_CURSOR(2,7);
            LCD_1CH(date_1 + 48);
            LCD_1CH(date_0 + 48);
            LCD_1CH('/');
            LCD_1CH(month_1 + 48);
            LCD_1CH(month_0 + 48);
            LCD_1CH('/');
            LCD_1CH(year_1 + 48);
            LCD_1CH(year_0 + 48);
            
            /*
            if(TRISCbits.TRISC2 == 1){                       //REVISAR SI SE PRESIONA EL BOTON DE ESTADOS
                AntiB = 1;
                __delay_ms(20);
                
                if(TRISCbits.TRISC2 == 0 & AntiB == 1){
                    //AntiB = 0;
                    ESTADOS = estados[1];                   //CAMBIAR DE ESTADO
                }
                
            }
            */
        }
        
        if(ESTADOS == 1){                      //ESTADO DE LUMINOSIDAD
            CLEAR_LCD();
            LCD_SET_CURSOR(1,2);
            LCD_STRING("LUMINOSIDAD");
            LCD_SET_CURSOR(2,5);
            LCD_1CH('.');
            LCD_SET_CURSOR(2,7);
            LCD_1CH('%');

            //ENVIO DE DATOS LCD
            
            if(ILU_E_CENT == 1 & ILU_E_DEC ==0){            //ENVIAR EL 1 DEL CIEN POR CIENTO
                LCD_SET_CURSOR(2,2);
                LCD_1CH(ILU_E_CENT + 48);
            }else{
                LCD_SET_CURSOR(2,2);
                LCD_1CH(' ');
            }
        
            if(ILU_E_DEC ==0 & ILU_E_CENT ==0){             //ENVIAR LAS DECENAS
                LCD_SET_CURSOR(2,3);
                LCD_1CH(' ');
            }else{
                LCD_SET_CURSOR(2,3);
                LCD_1CH(ILU_E_DEC + 48);
            }
        
            LCD_SET_CURSOR(2,4);
            LCD_1CH(ILUM_E_U + 48);                         //ENVIAR UNIDADES DEL ENTERO
            LCD_SET_CURSOR(2,6);
            LCD_1CH(DECIMAL1 + 48); 
            
            /*
            if(TRISCbits.TRISC2 == 1){
                AntiB = 1;
                __delay_ms(20);
                
                if(TRISCbits.TRISC2 == 0 & AntiB == 1){
                    //AntiB = 0;
                    ESTADOS = estados[2];
                }
                
            }
            
             */   
        }
        
        
        if(ESTADOS == 3){                  //ESTADO TEMPERATURA
            CLEAR_LCD();
            LCD_SET_CURSOR(1,5);
            LCD_STRING("TEMPERATURA");
            LCD_SET_CURSOR(2,10);
            LCD_1CH('.');
            LCD_SET_CURSOR(2,12);
            LCD_1CH('C');

            LCD_SET_CURSOR(2,8);
            LCD_1CH(TEMP_EN1 + 48);
        
            LCD_SET_CURSOR(2,9);
            LCD_1CH(TEMP_EN2 + 48);        
        
            LCD_SET_CURSOR(2,11);
            LCD_1CH(TEMP_D1 + 48);
            
            
            
            /*
            if(TRISCbits.TRISC2 == 1){
                AntiB = 1;
                __delay_ms(20);
                
                if(TRISCbits.TRISC2 == 0 & AntiB == 1){
                    //AntiB = 0;
                    ESTADOS = estados[3];
                }
                
            }
           */     
        }
        
       if(ESTADOS == 4){                   //SENSOR DE RUIDO
           CLEAR_LCD();
           LCD_SET_CURSOR(1,5);
           LCD_STRING("ESTADO");
           
           
           //ENVIO DE DATOS A LA LCD
           LCD_SET_CURSOR(2,7);
           
           if(ESTADO_RUIDO == 0){
               LCD_STRING("OFF");
           }else{
               LCD_STRING("ON");
           }
           
           /*
           if(TRISCbits.TRISC2 == 1){
               AntiB = 1;
               __delay_ms(20);
               
               if(TRISCbits.TRISC2 == 0 & AntiB == 1){
                   //AntiB = 0;
                   ESTADOS = estados[4];
                }
               
            } 
           */
        }
        
        
        if(ESTADOS == 5){
            CLEAR_LCD();
            LCD_SET_CURSOR(1,5);
            LCD_STRING("ESTADO");
            
            LCD_SET_CURSOR(2,5);
            LCD_STRING("PEND");
            
            /*
            if(TRISCbits.TRISC2 == 1){
                AntiB = 1;
                __delay_ms(20);
                
                if(TRISCbits.TRISC2 == 0 & AntiB == 1){
                    AntiB = 0;
                    ESTADOS = estados[0];
                }
                
            }
            */
        }
     
              
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
    TRISA = 0;                  //PINES DE DATA CON LA LCD
    TRISCbits.TRISC7 = 0;       //PIN PARA EL BIT E DE LA LCD
    TRISCbits.TRISC6 = 0;       //PIN PARA EL BIT RS DE LA LCD
    
    TRISBbits.TRISB0 = 1;       //PIN BOTON DE ESTADOS
    
    INTCON = 0b11001000;
    
    IOCB = 0b00000001;
}

//************************************************************************************
//              RTC; OBTENIDA DE: circuitdigest.com/microcontroller-projects/pic16f877a-ds3231-rtc-digital-clock
//***********************************************************************************

//CONVERSIÓN DE BINARIO A DECIMAL

int  BCD_2_DEC(int to_convert){
   return (to_convert >> 4) * 10 + (to_convert & 0x0F); 
}

//CONVERSIÓN DE DECIMAL A BINARIO

int DEC_2_BCD (int to_convert){
   return ((to_convert / 10) << 4) + (to_convert % 10);
}

//FUNCIÓN PARA CONFIGURAR HORA

void SET_TIME_DATE(){
    I2C_Master_Start();
    I2C_Master_Write(0xD0);                 //DIRECCIÓN PRE DEFINIDA DEL RTC
    I2C_Master_Write(0);
    I2C_Master_Write(DEC_2_BCD(sec));       //update sec
    I2C_Master_Write(DEC_2_BCD(min));       //update min
    I2C_Master_Write(DEC_2_BCD(hour));      //update hour
    I2C_Master_Write(1);                    //ACTUALIZA EL DÍA CON 1 SIEMPRE
    I2C_Master_Write(DEC_2_BCD(date));      //ACTUALIZAR FECHA
    I2C_Master_Write(DEC_2_BCD(month));     //ACTUALIZAR MES
    I2C_Master_Write(DEC_2_BCD(year));      //ACTUALIZAR AÑO
    I2C_Master_Stop();
}

//FUNCIÓN PARA LEER HORA
void Update_Current_Date_Time(){
    //START to Read 
   I2C_Master_Start(); 
   I2C_Master_Write(0xD0);                  //DIRECCIÓN PRE DEFINIDA DEL RTC
   I2C_Master_Write(0);
   I2C_Master_Stop();
   
   //READ
   I2C_Master_Start();                     //BLOQUE PARA LEER LOS SEGUNDOS
   I2C_Master_Write(0xD1);                 //GUARDAR DATOS PROVENIENTES DEL RTC
   sec = BCD_2_DEC(I2C_Master_Read(0));    //CONVIERTE EL DATO DE BINARIO A DECIMAL
   I2C_Master_Stop();
   
   I2C_Master_Start();                     //BLOQUE PARA LEER MINUTOS 
   I2C_Master_Write(0xD1);    
   min = BCD_2_DEC(I2C_Master_Read(0));     //CONVIERTE EL DATO DE BINARIO A DECIMAL
   I2C_Master_Stop();
   
   I2C_Master_Start();                      //BLOQUE PARA LEER LA HORA
   I2C_Master_Write(0xD1);                     
   hour = BCD_2_DEC(I2C_Master_Read(0));        
   I2C_Master_Stop();
   
   I2C_Master_Start();                      //NO ES DE INTERES GUARDAR EL DÍA
   I2C_Master_Write(0xD1);                     
   I2C_Master_Read(0);                       
   I2C_Master_Stop();
   
   I2C_Master_Start();                      //BLOQUE PARA LEER EL DÍA (FECHA)
   I2C_Master_Write(0xD1);                     
   date = BCD_2_DEC(I2C_Master_Read(0));        
   I2C_Master_Stop();
   
   I2C_Master_Start();                      //BLOQUE PARA LEER EL MES
   I2C_Master_Write(0xD1);                     
   month = BCD_2_DEC(I2C_Master_Read(0));        
   I2C_Master_Stop();
   
   I2C_Master_Start();                      //BLOQUE PARA LEER EL AÑO
   I2C_Master_Write(0xD1);                     
   year = BCD_2_DEC(I2C_Master_Read(0));        
   I2C_Master_Stop();
   
    I2C_Master_Stop();
    
    //End reading
   I2C_Master_Start(); 
   I2C_Master_Write(0xD1);
   I2C_Master_Read(0);
   I2C_Master_Stop();
}