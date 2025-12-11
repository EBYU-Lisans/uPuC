#include <xc.h>

#define _XTAL_FREQ     20000000UL   // 20 MHz kristal
#define PWM_FREQ       5000UL       // 5 kHz PWM frekans?
#define TMR2PRESCALE   4UL          // Timer2 prescaler

// CONFIG
#pragma config FOSC = HS, WDTE = OFF, PWRTE = ON, BOREN = ON
#pragma config LVP  = OFF, CPD = OFF, WRT = OFF, CP = OFF

//-------------------------------------------------------
// ADC AYARLARI
//-------------------------------------------------------
void ADC_Init(void)
{
    ADCON0 = 0b01000001;   // ADC ON, Fosc/8, kanal 0 seçili
    ADCON1 = 0b10000000;   // Right justified, Vref=Vdd, AN0 analog
}

unsigned int ADC_Read(unsigned char channel)
{
    if (channel > 7) return 0;

    ADCON0 &= 0b11000101;       // Kanal bitlerini temizle
    ADCON0 |= (channel << 3);   // Yeni kanal? yükle

    __delay_us(10);             // Acquisition time
    GO_nDONE = 1;               // Dönü?üm ba?lat
    while (GO_nDONE);           // Bitene kadar bekle

    return ((unsigned int)ADRESH << 8) | ADRESL;
}

//-------------------------------------------------------
// PWM AYARLARI
//-------------------------------------------------------
void PWM_Init(void)
{
    // 20 MHz ? 5 kHz PWM ? Prescaler 4 için PR2 = 249
    PR2 = (unsigned char)(_XTAL_FREQ / (PWM_FREQ * 4UL * TMR2PRESCALE) - 1UL);

    // CCP1 PWM modu
    CCP1CONbits.CCP1M3 = 1;
    CCP1CONbits.CCP1M2 = 1;

    // Timer2 ON, Prescaler 4
    T2CONbits.T2CKPS1 = 0;
    T2CONbits.T2CKPS0 = 1;
    T2CONbits.TMR2ON  = 1;

    TRISCbits.TRISC2 = 0;   // RC2 PWM ç?k???
}

void PWM_SetDuty(unsigned int duty)
{
    if (duty > 1023) duty = 1023;

    CCPR1L = duty >> 2;                   // Üst 8 bit
    CCP1CONbits.CCP1X = (duty >> 1) & 1;  // LSB1
    CCP1CONbits.CCP1Y = duty & 1;         // LSB0
}

//-------------------------------------------------------
// MAIN
//-------------------------------------------------------
void main(void)
{
    unsigned int adc_value;

    TRISA = 0xFF;   // AN0 giri?
    TRISC = 0x00;   // RC2 PWM ç?k???

    ADC_Init();
    PWM_Init();

    while (1)
    {
        adc_value = ADC_Read(0);   // Pot giri?ini oku
        PWM_SetDuty(adc_value);    // PWM duty güncelle
        __delay_ms(20);
    }
}

