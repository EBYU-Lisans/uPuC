#include <xc.h>
#define _XTAL_FREQ 4000000
unsigned int ADC_Read(unsigned char ch) {
    ADCON0 = (ch << 3);
    ADCON0 |= 0x01;
    __delay_us(30);
    GO_nDONE = 1;
    while(GO_nDONE);
    return ((ADRESH << 8) + ADRESL);
}
void main(void) {
    TRISA = 0xFF;
    TRISB = 0x00;
    ADCON1 = 0b10000000;
    unsigned int value;
while(1) {
        value = ADC_Read(0);
        PORTB = value >> 2;   // 10-bit → 8-bit
        __delay_ms(50);
    }
}
