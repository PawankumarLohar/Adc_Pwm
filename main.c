
// PIC10LF322 Configuration Bit Settings

// CONFIG
#pragma config FOSC = INTOSC        // Oscillator Selection bits (EC: CLKIN function enabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable (Brown-out Reset disabled)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // MCLR Pin Function Select bit (MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config LVP = OFF        // Low-Voltage Programming Enable (High-voltage on MCLR/VPP must be used for programming)
#pragma config LPBOR = OFF      // Brown-out Reset Selection bits (BOR disabled)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <stdint.h>

#define _XTAL_FREQ 8000000


void ClockConfig(void);
void ConfigAdcPins(void);
void ConfigAdcModule(void);
void ConfigAdcInterrupt(void);
void StartConverion(void);
void StopConverion(void);
void ConfigPwm(void); 

uint32_t Sum;
uint8_t Counter;

void __interrupt() isr_routine(void) 
{
    
    if(PIR1bits.ADIF == 1)
    {
        
        PIR1bits.ADIF = 0;
        ADCONbits.GO_nDONE = 1;
        Counter++;
        Sum += ADRES;
            if(Counter==254)
            {
                Sum = Sum >> 4;
                PWM2DCH = Sum >> 2 ;
                PWM2DCL = Sum & 0x0003;
                Counter = 0;
            }   
    }    
}

int main()
{
    ClockConfig();
    TRISAbits.TRISA2 = 0;
    ConfigAdcPins();
    ConfigAdcModule();
    ConfigAdcInterrupt();
    StartConverion();
    ConfigPwm();

    while(1)
    {
    }    
    return 0;
}

void ClockConfig()
{
    OSCCON = 0x70;
}

void ConfigAdcPins()
{
    TRISAbits.TRISA0 = 1;
    ANSELAbits.ANSA0 = 1;
    WPUA = 0x00;
}

void ConfigAdcModule()
{
    ADCONbits.ADCS0 = 1; //clock source FOSC/16 and channel AN0
    ADCONbits.ADCS1 = 0;
    ADCONbits.ADCS2 = 0;
    
    ADCONbits.CHS2 = 0;
    ADCONbits.CHS1 = 0;
    ADCONbits.CHS0 = 0;
    
    ADCONbits.ADON = 1;
}
    
void ConfigAdcInterrupt()
{
    PIR1bits.ADIF = 0;
    PIE1bits.ADIE = 1;
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
}

void StartConverion()
{
    ADCONbits.GO_nDONE = 1;
}

void StopConverion()
{
    ADCONbits.GO_nDONE = 0;
}

void ConfigPwm()
{
    CLC1CON = 0x00;
    CWG1CON0 = 0x00;
    TRISAbits.TRISA1 = 1;
    PWM2CON = 0x00;
    PR2 = 56;
    PWM2DCH = 0x00;
    PWM2DCL = 0x00;
    
    
    PIR1bits.TMR2IF = 0;
    T2CONbits.T2CKPS1 = 0;
    T2CONbits.T2CKPS0 = 0;
    
    T2CONbits.TMR2ON = 1;
    TRISAbits.TRISA1 = 0;
    PWM2CONbits.PWM2EN = 1;
    PWM2CONbits.PWM2OE = 1;
    
}