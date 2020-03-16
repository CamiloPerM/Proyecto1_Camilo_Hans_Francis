


char UART_Init(const long int baudrate); //Unicamente se incluye el baudrate que se va a utilizar
char UART_TX_Empty();
void UART_Write(char data);
void UART_Write_Text(char *text);
char UART_Data_Ready(void);
char UART_Read(void);
void UART_Read_Text(char *Output, unsigned int length);