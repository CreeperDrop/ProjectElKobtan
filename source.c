/*
 * File:   source.c
 * Author: Ahmed Elsousy
 *
 * Created on November 8, 2021, 5:34 PM
 */
// PIC16F877A Configuration Bit Settings

// 'C' source line config statements

// CONFIG
#pragma config FOSC = EXTRC     // Oscillator Selection bits (RC oscillator)
#pragma config WDTE = OFF        // Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = ON         // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3/PGM pin has PGM function; low-voltage programming enabled)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.
#define _XTAL_FREQ 8000000
#include <xc.h>

void adcGo();
void adcInit();
unsigned int adcRead();
unsigned int lightLevel(unsigned int);

void main(void) {
    
    unsigned int voltageReading, lightReading;
    
    TRISA = 1;      //A is input 
    TRISB = 0;      //B is output
    
    adcInit();      //Initialise the ADC with the desired settings.
    
    while(1){
        
        voltageReading = adcRead();     //Get the value read by the ADC
        
        __delay_ms(100);
        
        lightReading = lightLevel(voltageReading);
        
        if(lightReading == 0)
            PORTB = 0x00;
        else if (lightReading > 0 && lightReading <= 341)
            PORTB = 0x07;
        else if(lightReading > 341 && lightReading <= 682)
            PORTB = 0x03;
        else if(lightReading > 682)
            PORTB = 0x01;
    }
    
    return;
}


void adcGo(void){
    ADCON0bits.GO = 1;
}
void adcInit(void){
    
    ADCON0 = 0x81;          //= 10 000 0 0 1        ClockSetting Channel Go/Done n/a ADCisON
    ADCON1 = 0x8E;          //= 1 000 1110;         RightJust n/a AN0channelOnlyIsAnalogue
    
}

unsigned int adcRead(){
    
    unsigned int reading;
    
    __delay_ms(1);          //delay to make sure everything is okay
    adcGo();                //Start ADC
    while(ADCON0bits.GO_DONE == 1);         //Wait until conversion is done 
 
    reading = (unsigned int)((ADRESH << 8) + ADRESL); //Get the reading the 2 High bits shifted by 8 + the low bits (HHLLLLLLLL)
    
    return reading;
}

unsigned int lightLevel(unsigned int adcReading){
    
    return 1024 - adcReading;       //10 bits so 1024 max, get the delta between 1024 and the voltage value that would be the light level 

}
