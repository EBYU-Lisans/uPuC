/*
 * PIC16F877A + ADC(AN0) + 16x2 LCD (HD44780) 4-bit
 * MPLAB X + XC8, Fosc = 20MHz (HS)
 *
 * LCD 16x2 (4-bit):
 *   RB0 -> RS
 *   RB1 -> EN
 *   RB2 -> D4
 *   RB3 -> D5
 *   RB4 -> D6
 *   RB5 -> D7
 *   RW  -> GND
 *
 * ADC:
 *   RA0/AN0 -> potentiometer wiper (ends to +5V/GND)
 */

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>

// ===================== CONFIG BITS =======================
#pragma config FOSC = HS
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

#define _XTAL_FREQ 20000000UL

// ===================== LCD PINS (PORTB) ==================
#define LCD_RS      RB0
#define LCD_EN      RB1
#define LCD_D4      RB2
#define LCD_D5      RB3
#define LCD_D6      RB4
#define LCD_D7      RB5

#define LCD_RS_TRIS TRISB0
#define LCD_EN_TRIS TRISB1
#define LCD_D4_TRIS TRISB2
#define LCD_D5_TRIS TRISB3
#define LCD_D6_TRIS TRISB4
#define LCD_D7_TRIS TRISB5

// ===================== LCD (HD44780) 4-bit ===============
static void LCD_PulseEnable(void)
{
    LCD_EN = 1;
    __delay_us(5);
    LCD_EN = 0;
    __delay_us(200);
}

static void LCD_SendNibble(uint8_t n)
{
    LCD_D4 = (n >> 0) & 1;
    LCD_D5 = (n >> 1) & 1;
    LCD_D6 = (n >> 2) & 1;
    LCD_D7 = (n >> 3) & 1;
    LCD_PulseEnable();
}

static void LCD_SendByte(uint8_t isData, uint8_t b)
{
    LCD_RS = isData ? 1 : 0;
    LCD_SendNibble((b >> 4) & 0x0F);
    LCD_SendNibble(b & 0x0F);
}

static void LCD_Command(uint8_t cmd)
{
    LCD_SendByte(0, cmd);
    if (cmd == 0x01 || cmd == 0x02) __delay_ms(3);
    else __delay_us(200);
}

static void LCD_Char(char c)
{
    LCD_SendByte(1, (uint8_t)c);
    __delay_us(200);
}

static void LCD_String(const char *s)
{
    while (*s) LCD_Char(*s++);
}

static void LCD_SetCursor(uint8_t row, uint8_t col)
{
    uint8_t addr = (row == 0) ? 0x00 : 0x40;
    LCD_Command(0x80 | (addr + col));
}

static void LCD_Clear(void)
{
    LCD_Command(0x01);
}

static void LCD_Init(void)
{
    LCD_RS_TRIS = 0; LCD_EN_TRIS = 0;
    LCD_D4_TRIS = 0; LCD_D5_TRIS = 0; LCD_D6_TRIS = 0; LCD_D7_TRIS = 0;

    LCD_RS = 0; LCD_EN = 0;
    __delay_ms(50);

    // 4-bit init sequence
    LCD_SendNibble(0x03); __delay_ms(5);
    LCD_SendNibble(0x03); __delay_us(150);
    LCD_SendNibble(0x03); __delay_us(150);
    LCD_SendNibble(0x02); __delay_us(150);

    LCD_Command(0x28); // 4-bit, 2-line
    LCD_Command(0x0C); // display on
    LCD_Command(0x06); // entry mode
    LCD_Clear();
}

// ===================== ADC (AN0) =========================
static void ADC_Init_AN0(void)
{
    TRISA0 = 1; // RA0 input

    // AN0 analog, others digital + RIGHT JUSTIFIED
    // ADFM=1 (bit7), PCFG=1110 -> AN0 analog, Vref=VDD/VSS
    ADCON1 = 0x8E;

    // ADCON0: ADCS=Fosc/32, CHS=AN0, ADON=1
    ADCON0 = 0b10000001;
}

static uint16_t ADC_Read_AN0(void)
{
    ADCON0bits.CHS = 0;   // AN0
    __delay_us(30);       // acquisition

    ADCON0bits.GO_nDONE = 1;
    while (ADCON0bits.GO_nDONE) { ; }

    // right-justified 10-bit
    return ((uint16_t)ADRESH << 8) | ADRESL;
}

// ===================== FORMAT HELPERS ====================
static void u16_to_4digits(uint16_t v, char out[5])
{
    out[0] = (char)('0' + (v / 1000));
    out[1] = (char)('0' + (v / 100) % 10);
    out[2] = (char)('0' + (v / 10)  % 10);
    out[3] = (char)('0' + (v % 10));
    out[4] = '\0';
}

static void adc_to_volt_str(uint16_t adc, char out[6])
{
    // mV = adc * 5000 / 1023
    uint32_t mv = (uint32_t)adc * 5000UL / 1023UL;

    uint16_t vi = (uint16_t)(mv / 1000);        // 0..5
    uint16_t vf = (uint16_t)((mv % 1000) / 10); // 00..99

    out[0] = (char)('0' + vi);
    out[1] = '.';
    out[2] = (char)('0' + (vf / 10));
    out[3] = (char)('0' + (vf % 10));
    out[4] = 'V';
    out[5] = '\0';
}

// ===================== MAIN ===============================
void main(void)
{
    uint16_t adc;
    char s_adc[5];
    char s_v[6];

    TRISB = 0x00;
    PORTB = 0x00;

    ADC_Init_AN0();
    LCD_Init();

    LCD_SetCursor(0, 0);
    LCD_String("ADC + LCD Demo");
    LCD_SetCursor(1, 0);
    LCD_String("AN0 reading...");
    __delay_ms(1200);
    LCD_Clear();

    while (1)
    {
        adc = ADC_Read_AN0();

        u16_to_4digits(adc, s_adc);
        adc_to_volt_str(adc, s_v);   // <<< EKS?K OLAN BUYDU

        LCD_SetCursor(0, 0);
        LCD_String("ADC=");
        LCD_String(s_adc);
        LCD_String("    ");          // sat?r temizle

        LCD_SetCursor(1, 0);
        LCD_String("V=");
        LCD_String(s_v);
        LCD_String("     ");

        __delay_ms(300);
    }
}
