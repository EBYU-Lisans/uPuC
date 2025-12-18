#include <xc.h>

#pragma config FOSC = XT    // 4 MHz kristal veya SimulIDE internal fast
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

#define _XTAL_FREQ 4000000   // PIC clock 4 MHz

//---------------------- UART INIT --------------------------
void UART_Init(void)
{
    TRISC6 = 0;  // TX ç?k??
    TRISC7 = 1;  // RX giri?

    TXSTAbits.BRGH = 1;   // high speed  -> önemli
    SPBRG = 25;           // 9600 baud @ 4 MHz

    TXSTAbits.SYNC = 0;   // async
    TXSTAbits.TXEN = 1;   // transmitter enable

    RCSTAbits.SPEN = 1;   // serial port enable
    RCSTAbits.CREN = 1;   // continuous receive enable
}

void UART_PutC(char c)
{
    while(!TXSTAbits.TRMT);
    TXREG = c;
    __delay_us(104);  // 9600 baud için karakter süre yakla??k 104 us, bekleme art?r?ld?
}

//---------------------- UART WRITE STRING -------------------
void UART_Print(const char *s)
{
    while(*s)
        UART_PutC(*s++);
}

//------------------------------ MAIN -------------------------
void main(void)
{
    UART_Init();

    __delay_ms(200);   // ba?lang?ç stabilizasyonu

    while(1)
    {
        UART_Print("ab");
        __delay_ms(500);
        UART_Print("cd");
        __delay_ms(500);
        UART_Print("ef");
        __delay_ms(500);
    }
}