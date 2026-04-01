#include <xc.h>
#define _XTAL_FREQ 4000000
#pragma config FOSC=XT, WDTE=OFF, PWRTE=ON, BOREN=ON, LVP=OFF
void __interrupt() ISR(void) {
    if (PIR1bits.TMR1IF) {
        PORTBbits.RB1 = !PORTBbits.RB1;   // LED toggle
        PIR1bits.TMR1IF = 0;              // Bayrağı temizle
        TMR1H = 0x00;                     // Sayaç sıfırla
        TMR1L = 0x00;
    }
}
void main(void) {
    TRISB1 = 0;                 // RB1 çıkış
    PORTBbits.RB1 = 0;
T1CON = 0b00110001;         // Timer1 ON, Fosc/4, 1:8 prescaler
    PIR1bits.TMR1IF = 0;        // Bayrak temizle
    PIE1bits.TMR1IE = 1;        // Timer1 kesmesi aktif
    INTCONbits.PEIE = 1;        // Çevresel kesmelere izin
    INTCONbits.GIE  = 1;        // Genel kesmeleri aç
while(1) {
        // Ana döngüde farklı işler yapılabilir
        __delay_ms(50);
    }
}
