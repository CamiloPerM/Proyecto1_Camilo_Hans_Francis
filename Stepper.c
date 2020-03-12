
#include <xc.h>
#include <pic16f887.h>
#include<stdint.h>
#include"Stepper.h"

//Código basado en: circuitdigest.com/microcontroller-projects/interfacing-stepper-motor-with-pic16f877a

//***********************************************************************************
//                  FULL DRIVE
//**********************************************************************************

void FULL_DRIVE(uint8_t direccion){         
    if(direccion == 1){                     //anti_clockwise
        PUERTO = 0b00000011;
        __delay_ms(speed);
        PUERTO = 0b00000110;
        __delay_ms(speed);
        PUERTO = 0b00001100;
        __delay_ms(speed);
        PUERTO = 0b00001001;
        __delay_ms(speed);
        PUERTO = 0b00000011;
        __delay_ms(speed);   
    }
    
    if(direccion == 0){                     //clockwise
        PUERTO = 0b00001001;
        __delay_ms(speed);
        PUERTO = 0b00001100;
        __delay_ms(speed);
        PUERTO = 0b00000110;
        __delay_ms(speed);
        PUERTO = 0b00000011;
        __delay_ms(speed);
        PUERTO = 0b00001001;
        __delay_ms(speed);
    }
    
}

//***********************************************************************************
//                  HALF DRIVE
//**********************************************************************************

void HALF_DRIVE(uint8_t direccion){
    if(direccion == 1){                     //anti clockwise
        PUERTO = 0b00000001;
        __delay_ms(speed);
        PUERTO = 0b00000011;
        __delay_ms(speed);
        PUERTO = 0b00000010;
        __delay_ms(speed);
        PUERTO = 0b00000110;
        __delay_ms(speed);
        PUERTO = 0b00000100;
        __delay_ms(speed);
        PUERTO = 0b00001100;
        __delay_ms(speed);
        PUERTO = 0b00001000;
        __delay_ms(speed);
        PUERTO = 0b00001001;
        __delay_ms(speed);
     
    }
    
    if(direccion == 0){                 //CLOCKWISE
        PUERTO = 0b00001001;
        __delay_ms(speed);
        PUERTO = 0b00001000;
        __delay_ms(speed);
        PUERTO = 0b00001100;
        __delay_ms(speed); 
        PUERTO = 0b00000100;
        __delay_ms(speed);
        PUERTO = 0b00000110;
        __delay_ms(speed);
        PUERTO = 0b00000010;
        __delay_ms(speed);
        PUERTO = 0b00000011;
        __delay_ms(speed);
        PUERTO = 0b00000001;
        __delay_ms(speed);
    }
    
    
}

//***********************************************************************************
//                  WAVE DRIVE
//**********************************************************************************
void WAVE_DRIVE(uint8_t direccion){
    if(direccion == 1){                 //ANTI CLOCKWISE
        PUERTO = 0b00000001;
         __delay_ms(speed);
        PUERTO = 0b00000010;
         __delay_ms(speed);
        PUERTO = 0b00000100;
         __delay_ms(speed);
        PUERTO = 0b00001000;
         __delay_ms(speed);
    }
    
    if(direccion ==0){                  //CLOCKWISE
        PUERTO = 0b00001000;
        __delay_ms(speed);
        PUERTO = 0b00000100;
        __delay_ms(speed);
        PUERTO = 0b00000010;
        __delay_ms(speed);
        PUERTO = 0b00000001;
        __delay_ms(speed);
    }
}