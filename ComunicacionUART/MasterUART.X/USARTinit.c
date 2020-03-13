/*
 * File:   USARTinit.c
 * Author: Francis_Sanabria
 *
 * Created on 10 de febrero de 2020, 11:46 PM
 */
#define _XTAL_FREQ 8000000


#include "USARTinit.h"
#include <xc.h>

char UART_Init(const long int baudrate)
{
  unsigned int x;
  x = (_XTAL_FREQ - baudrate*64)/(baudrate*64);   //SPBRG for Low Baud Rate
  if(x>255)                                       //If High Baud Rage Required
  {
    x = (_XTAL_FREQ - baudrate*16)/(baudrate*16); //SPBRG for High Baud Rate
    TXSTAbits.BRGH=1;                                     //Setting High Baud Rate
  }
  if(x<256)
  {
    SPBRG = x;                                    //Writing SPBRG Register
    //TXSTA
    TXSTAbits.TX9=0;  //8 bit transmission  YA
    TXSTAbits.TXEN=1; //Transmit enable YA
    TXSTAbits.SYNC=0; //Async mode YA

    //RCSTA
    RCSTAbits.SPEN=1;   //Serial port enabled YA
    RCSTAbits.RX9=0;    //8 bit mode YA 
    RCSTAbits.CREN=1;   //Enable receive YA
    RCSTAbits.ADDEN=0;  //Disable address detection
    return 1;                                     //Returns 1 to indicate Successful Completion
  }
  return 0;                                       //Lo devuleve si no inicializa el puerto
}

char UART_TX_Empty()
{
  return TRMT;
}

void UART_Write(char data)
{
  while(!TRMT);
  TXREG = data;
}

void UART_Write_Text(char *text)
{
  int i;
  for(i=0;text[i]!='\0';i++)
    UART_Write(text[i]);
}

char UART_Data_Ready()
{
  return RCIF;
}

char UART_Read()
{
  while(!RCIF);
  return RCREG;
}
void UART_Read_Text(char *Output, unsigned int length)
{
  unsigned int i;
  for(int i=0;i<length;i++)
  Output[i] = UART_Read();
}