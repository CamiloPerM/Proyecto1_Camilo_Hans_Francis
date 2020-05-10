/* 
 * File: LCD.c  
 * Se utilizó y se adaptaron las librerías de Ligo George 
 * de la página www.electrosome.com
 * Enlace: https://electrosome.com/lcd-pic-mplab-xc8/
 * Revision history: 
 */

#include <xc.h>



//LCD Functions Developed by electroSome
//Modificaciones realizadas por Pablo Mazariegos
void Lcd_Port(char a); //Funcion que verifica que los datos de los puertos sean correctos
void Lcd_Cmd(char a); //Escribe comandos a la LCD de 4 bits, su paramatero de en entrada es el comando
void Lcd_Clear(void); //Limpia pantalla en LCD de 4 bits.
void Lcd_Set_Cursor(char a, char b); //Cambia la posicion del cursor en la LCD de 4 bits
//a es la fila
//b es la columna
void Lcd_Init(void); //Funcion de inicializacion de LCD de 4 bits
void Lcd_Write_Char(char a,int modo); //Escribe un caracter a la LCD ya sea de 4 u 8 bits correspondiente al segundo parametro
void Lcd_Write_String(char *a,int modo); //Escribe un string a la LCD ya sea para 4 u 8 bits
void Lcd_Shift_Right(void); //Shift a la derecha
void Lcd_Shift_Left(void); //Shift a la izquierda
void Lcd_Cmd_8bits(unsigned char comando); //Escribe comando a la LCD de 8 bits
void Lcd_Clear_8bits(void); //Limpia pantalla de la LCD de 8 bits
void Lcd_Set_Cursor_8bits(unsigned char y, unsigned char x); //Para moverse de cursor en la LCD de 8 bits.
void Lcd_Init_8bits(void); //Inicializacion de la LCD de 8 bits
void Lcd_Shift_Right_8bits(void);
void Lcd_Shift_Left_8bits(void);


