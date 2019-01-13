#define _XTAL_FREQ 200000   // frequenza oscillatore RC smarzo = 200kHz

// configurazione pin LCD
#define RS RA0  
#define EN RA1
#define D4 RA2
#define D5 RA3
#define D6 RA4
#define D7 RA5

#include <xc.h>
#include "lcd.h";
#include "eggometer.h"

//MPLABX GENERATED CONFIG
#pragma config FOSC = EXTRC // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF // Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = OFF // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = ON // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF // Flash Program Memory Code Protection bit (Code protection off)
//END CONFIG

int main() {
    TRISA = 0x00;   //  pin 2-7 in modalità output per scrivere sull'LCD
    TRISB = 0xFF;   //  pin 21-28 in modalità input per ricevere i conti del rpi
    TRISCbits.TRISC0 = 1;   //    
    TRISCbits.TRISC1 = 1;   //  pin 11-13 come sopra
    TRISCbits.TRISC2 = 1;   //
    TRISCbits.TRISC3 = 0;   //
    TRISCbits.TRISC4 = 0;   //  pin 14-14 in modalità output per inviare al rpi la conversione dal magazzino
    
    //  inizializzazione
    Lcd_Init();
    char uova;
    char sposta;
    char mag;
    char c;
    
    //test lcd
    Lcd_Clear();
    Lcd_Set_Cursor(1, 1);
    Lcd_Write_String("Ciao :D");
    __delay_ms(2000);

    while (1) {
        
        //conteggio e conversione magazzino
        PORTCbits.RC3 = 0;
        PORTCbits.RC4 = 0;
        c = countEggs(PORTCbits.RC0, PORTCbits.RC1, PORTCbits.RC2);
        switch (c) {
            case "0":
                break;
            case "1":
                PORTCbits.RC3 = 1;
                PORTCbits.RC4 = 0;
                break;
            case "2":
                PORTCbits.RC3 = 0;
                PORTCbits.RC4 = 1;
                break;
            case "3":
                PORTCbits.RC3 = 1;
                PORTCbits.RC4 = 1;
                break;
        }

        // codifica dalle info del rpi a caratteri stampabili
        uova = countEggs(PORTBbits.RB0, PORTBbits.RB1, PORTBbits.RB2);
        sposta = countEggs(PORTBbits.RB3, PORTBbits.RB4);
        mag = countEggs(PORTBbits.RB5, PORTBbits.RB6, PORTBbits.RB7);

        
        //stampa caratteri
        Lcd_Clear();
        Lcd_Set_Cursor(1, 1);
        Lcd_Write_String("Uova:");
        Lcd_Write_Char(uova);
        Lcd_Write_String(" Mag:");
        Lcd_Write_Char(mag);
        Lcd_Set_Cursor(2, 1);
        Lcd_Write_String("Spostamento:");
        Lcd_Write_Char(sposta);
        __delay_ms(2000);

    }
    return 0;
}