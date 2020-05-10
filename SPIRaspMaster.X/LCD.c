/*
 * File:   LCD.c
 * Author: Francis_Sanabria
 *
 * Created on 10 de febrero de 2020, 11:22 PM
 */
#define _XTAL_FREQ 500000 //Hay que definir el reloj tambien

#define RS PORTEbits.RE0
#define EN PORTEbits.RE1
#define D4 PORTBbits.RB4
#define D5 PORTBbits.RB5
#define D6 PORTBbits.RB6
#define D7 PORTBbits.RB7
#define portLCD PORTB       //Se usa el puerto B de 8 bits  

#include <xc.h>
#include "LCD.h"

void Lcd_Port(char a)
{
    
	if(a & 1)
		D4 = 1;
	else
		D4 = 0;

	if(a & 2)
		D5 = 1;
	else
		D5 = 0;

	if(a & 4)
		D6 = 1;
	else
		D6 = 0;

	if(a & 8)
		D7 = 1;
	else
		D7 = 0;
}
void Lcd_Cmd(char a)
{
	RS = 0;             // => RS = 0
	Lcd_Port(a);
	EN  = 1;             // => E = 1
        __delay_ms(4);
        EN  = 0;             // => E = 0
}

void Lcd_Clear(void)
{
	Lcd_Cmd(0);             //Se borra el puerto
	Lcd_Cmd(1);             //Se borra la pantalla
}

void Lcd_Set_Cursor(char a, char b)
{
	char temp,z,y;
	if(a == 1)
	{
	  temp = 0x80 + b - 1;
		z = temp>>4;
		y = temp & 0x0F;
		Lcd_Cmd(z);
		Lcd_Cmd(y);
	}
	else if(a == 2)
	{
		temp = 0xC0 + b - 1;
		z = temp>>4;
		y = temp & 0x0F;
		Lcd_Cmd(z);
		Lcd_Cmd(y);
	}
}

void Lcd_Init(void)
{
  Lcd_Port(0x00);
   __delay_ms(15);
  Lcd_Cmd(0x03);
	__delay_ms(4);
  Lcd_Cmd(0x03);
	__delay_us(100);
  Lcd_Cmd(0x03);
  /////////////////////////////////////////////////////
  Lcd_Cmd(0x02);
  Lcd_Cmd(0x02);
  Lcd_Cmd(0x08);
  Lcd_Cmd(0x00);
  Lcd_Cmd(0x0C);
  Lcd_Cmd(0x00);
  Lcd_Cmd(0x06);
}

void Lcd_Write_Char(char a,int modo)
{
    if(modo==4)
    {
   char temp,y;
   temp = a&0x0F;
   y = a&0xF0;
   RS = 1;             // => RS = 1
   Lcd_Port(y>>4);             //Data transfer
   EN = 1;
   __delay_us(40);
   EN = 0;
   Lcd_Port(temp);
   EN = 1;
   __delay_us(40);
   EN = 0;
    }
    
    //-----------------------------------------
    else if(modo==8)
    {
    RS = 1; //Activamos RS
    portLCD = a;  //Incluimos el valor del caracter al puerto 
    EN = 1; //Encendemos el reloj
    __delay_ms(1); //Delay Aplicado
    EN = 0; //Apagamos el reloj
    }
}

void Lcd_Write_String(char *a,int modo)
{
	int i;
	for(i=0;a[i]!='\0';i++)
	   Lcd_Write_Char(a[i],modo);
}

void Lcd_Shift_Right(void)
{
	Lcd_Cmd(0x01);
	Lcd_Cmd(0x0C);
}

void Lcd_Shift_Left(void)
{
	Lcd_Cmd(0x01);
	Lcd_Cmd(0x08);
}


//------------------COMANDOS SOLO PARA LA DE 8 Bits--------------------

void Lcd_Cmd_8bits(unsigned char comando)   //Manda comandos a la LCD
{
	RS = 0;                             
	portLCD = comando;              
	
    EN  = 1;                            
    __delay_ms(4);                      
    EN  = 0;                            
    
   }

void Lcd_Clear_8bits(void)
{
    Lcd_Cmd_8bits(0);                        //se limpia el puerto
	Lcd_Cmd_8bits(1);
}

void Lcd_Shift_Right_8bits(void)
{
	Lcd_Cmd_8bits(0x01);
	Lcd_Cmd_8bits(0x0C);
}

void Lcd_Shift_Left_8bits(void)
{
	Lcd_Cmd_8bits(0x01);
	Lcd_Cmd_8bits(0x08);
}


void Lcd_Set_Cursor_8bits(unsigned char y, unsigned char x)
{
	
    if(y == 1)
	{
        Lcd_Cmd_8bits(0x80 + x - 1);
	}
    
	else if(y == 2)
	{
		Lcd_Cmd_8bits(0xC0 + x - 1);
	}
}

void Lcd_Init_8bits(void)
{
    portLCD = 0;
    __delay_ms(20);
    Lcd_Cmd_8bits(0x030); //El bit BF no puede ser comprobado antes de esta instrucción
    __delay_ms(5);       //Viualizador se ajusta en modo de 8 bits
    Lcd_Cmd_8bits(0x030); //El bit BF no puede ser comprobado antes de esta instrucción
    __delay_us(160);     //Viualizador se ajusta en modo de 8 bits
    Lcd_Cmd_8bits(0x030);           //El bit BF se puede comprobar despues de las siguientes instrucciones
    __delay_us(160);
    
    Lcd_Cmd_8bits(0x38); //formato  
    
    //Lcd_Cmd_8bits(0x06);
    
    Lcd_Cmd_8bits(0x14);                                         
    Lcd_Cmd_8bits(0x6);     //Visualizador apagado                   
    Lcd_Cmd_8bits(0xc);     //visualizador borrado                  
    Lcd_Cmd_8bits(0x2);     //Ajustar el mode de introducir los caracteres               
}