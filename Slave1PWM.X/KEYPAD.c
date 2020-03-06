/*
 * File:   KEYPAD.c
 * Author: Francis_Sanabria
 *
 * Created on 27 de febrero de 2020, 03:29 PM
 */

#include "KEYPAD.h"
#include <xc.h>




char keypad_scanner(void)  
{           
            X_1 = 0; X_2 = 1; X_3 = 1; X_4 = 1;    
            if (Y_1 == 0) { 
                __delay_ms(100); 
                while (Y_1==0); 
                return '1'; }
            if (Y_2 == 0) {
                __delay_ms(100); 
                while (Y_2==0); 
                return '2'; }
            if (Y_3 == 0) { 
                __delay_ms(100); 
                while (Y_3==0); 
                return '3'; }
            if (Y_4 == 0) { 
                __delay_ms(100); 
                while (Y_4==0); 
                return 'A'; }
            
            X_1 = 1; X_2 = 0; X_3 = 1; X_4 = 1;    
            if (Y_1 == 0) { 
                __delay_ms(100); 
                while (Y_1==0); 
                return '4'; }
            if (Y_2 == 0) { 
                __delay_ms(100); 
                while (Y_2==0); 
                return '5'; }
            if (Y_3 == 0) { 
                __delay_ms(100); 
                while (Y_3==0); 
                return '6'; }
            if (Y_4 == 0) { 
                __delay_ms(100); 
                while (Y_4==0); 
                return 'B'; }
            
            X_1 = 1; X_2 = 1; X_3 = 0; X_4 = 1;    
            if (Y_1 == 0) { 
                __delay_ms(100); 
                while (Y_1==0); 
                return '7'; }
            if (Y_2 == 0) { 
                __delay_ms(100); 
                while (Y_2==0); 
                return '8'; }
            if (Y_3 == 0) { 
                __delay_ms(100); 
                while (Y_3==0); 
                return '9'; }
            if (Y_4 == 0) { 
                __delay_ms(100); 
                while (Y_4==0); 
                return 'C'; }
           
            X_1 = 1; X_2 = 1; X_3 = 1; X_4 = 0;    
            if (Y_1 == 0) { 
                __delay_ms(100); 
                while (Y_1==0); 
                return '*'; }
            if (Y_2 == 0) { 
                __delay_ms(100); 
                while (Y_2==0); 
                return '0'; }
            if (Y_3 == 0) { 
                __delay_ms(100); 
                while (Y_3==0); 
                return '#'; }
            if (Y_4 == 0) { 
                __delay_ms(100); 
                while (Y_4==0); 
                return 'D'; }
            
    return 'n';                   
}
char switch_press_scan(void){                       // Get key from user
            char key = 'n';              // Assume no key pressed
            while(key=='n')              // Wait untill a key is pressed
            key = keypad_scanner();   // Scan the keys again and again
            return key;                  //when key pressed then return its value
}

void InitKeypad(void){
            Keypad_PORT = 0x00;        // Set Keypad port pin values zero
            Keypad_PORT_Direction = 0xF0;      // Last 4 pins input, First 4 pins output        
            OPTION_REG &= 0xFF;
}