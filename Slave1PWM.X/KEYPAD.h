#include <xc.h>

#define _XTAL_FREQ 500000 //Crystal Frequency, used in delay
#define X_1    RD0
#define X_2    RD1
#define X_3    RD2
#define X_4    RD3
#define Y_1    RD4
#define Y_2    RD5
#define Y_3    RD6
#define Y_4    RD7
#define Keypad_PORT          PORTD
#define Keypad_PORT_Direction     TRISD 

char keypad_scanner(void);
void InitKeypad(void);
char switch_press_scan(void);