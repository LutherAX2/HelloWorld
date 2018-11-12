#include <p18f452.h>
//#include <pwm.h>
#include <timers.h>
#include <portb.h>
#include <delays.h>
#include "xlcd2.h"
#include "Keypad_Functions.h"
//#include "ow.h"
//#include "Speaker_Functions.h"

//extern int live;
void keypad_config(void)
{
INTCON3bits.INT1IF = 0;
INTCONbits.INT0IE = 1;
INTCON2bits.INTEDG1 = 1;

INTCON2bits.RBPU = 0;
INTCON3bits.INT1IP = 1;
INTCON3bits.INT1IE = 1;

INTCONbits.RBIE = 0;


TRISA = 0;
PORTA = 0; 
TRISBbits.TRISB1 = 1;
TRISCbits.TRISC7 = 1;
TRISCbits.TRISC6 = 1;
TRISCbits.TRISC5 = 1;
TRISCbits.TRISC4 = 1;



RCONbits.IPEN = 1;
RCONbits.TO=1;
INTCONbits.GIEH = 1;
INTCONbits.PEIE = 1;
}


void keypad_lcd(void)
{     
    {
        PORTC = PORTC & 0xF0;
        if (PORTC == 0xF0)
        {
            PORTA = 15;
            while( BusyXLCD() );
            WriteCmdXLCD(0b00000001);
            while( BusyXLCD() );
            SetDDRamAddr(0x43);
            putrsXLCD( "D Pressed");
//            OpenPWM1(16);
//            Delay1KTCYx(255);
//            ClosePWM1();
        }
        if (PORTC == 0xE0)
        {
           PORTA = 14;
            while( BusyXLCD() );
            WriteCmdXLCD(0b00000001);
            while( BusyXLCD() );
            SetDDRamAddr(0x43);
            putrsXLCD( "E Pressed");
//            OpenPWM1(32);
//            Delay1KTCYx(255);
//            ClosePWM1();
        }
        if (PORTC == 0xD0)
        {
           PORTA = 13;
            while( BusyXLCD() );
            WriteCmdXLCD(0b00000001);
            while( BusyXLCD() );
            SetDDRamAddr(0x43);
            putrsXLCD( "F Pressed");
//            OpenPWM1(48);
//            Delay1KTCYx(255);
//            ClosePWM1();
        }
        if (PORTC == 0xC0)
        {
            PORTA = 12;
            while( BusyXLCD() );
            WriteCmdXLCD(0b00000001);
            while( BusyXLCD() );
            SetDDRamAddr(0x43);
            putrsXLCD( "0 Pressed");
//            OpenPWM1(64);
//            Delay1KTCYx(255);
//            ClosePWM1();
            
        }
        if (PORTC == 0xB0)
        {
            PORTA = 11;
            while( BusyXLCD() );
            WriteCmdXLCD(0b00000001);
            while( BusyXLCD() );
            SetDDRamAddr(0x46);
            putrsXLCD( "LIVE");
//            OpenPWM1(80);
//            Delay1KTCYx(255);
//            ClosePWM1();
//            Delay10KTCYx(255);
//            live=1;
//            livereadings();
        }
        if (PORTC == 0xA0)
        {
            PORTA = 10;
            while( BusyXLCD() );
            WriteCmdXLCD(0b00000001);
            while( BusyXLCD() );
            SetDDRamAddr(0x43);
            putrsXLCD( "9 Pressed");
//            OpenPWM1(96);
//            Delay10KTCYx(100);
//            ClosePWM1();
        }
        if (PORTC == 0x90)
        {
            PORTA = 9;
            while( BusyXLCD() );
            WriteCmdXLCD(0b00000001);
            while( BusyXLCD() );
            SetDDRamAddr(0x43);
            putrsXLCD( "8 Pressed");
//            OpenPWM1(112);
//            Delay1KTCYx(255);
//            ClosePWM1();
        }
        if (PORTC == 0x80)
        {
            PORTA = 8;
            while( BusyXLCD() );
            WriteCmdXLCD(0b00000001);
            while( BusyXLCD() );
            SetDDRamAddr(0x43);
            putrsXLCD( "7 Pressed");
//            OpenPWM1(127);
//            Delay1KTCYx(255);
//            ClosePWM1();
        }
        if (PORTC == 0x70)
        {
            PORTA = 7;
            while( BusyXLCD() );
            WriteCmdXLCD(0b00000001);
            while( BusyXLCD() );
            SetDDRamAddr(0x43);
            putrsXLCD( "HEART RATE");
//            OpenPWM1(208);
//            Delay1KTCYx(255);
//            ClosePWM1();
//            Delay10KTCYx(255);
//            while( BusyXLCD() );
//            WriteCmdXLCD(0b00000001);
//            Heart_Monitor();
           

        }
        if (PORTC == 0x60)
        {
            PORTA = 6;
            while( BusyXLCD() );
            WriteCmdXLCD(0b00000001);
            while( BusyXLCD() );
            SetDDRamAddr(0x43);
            putrsXLCD( "6 Pressed");
//            OpenPWM1(160);
//            Delay1KTCYx(255);
//            ClosePWM1();
        }
        if (PORTC == 0x50)
        {
            PORTA = 5;
           while( BusyXLCD() );
            WriteCmdXLCD(0b00000001);
            while( BusyXLCD() );
            SetDDRamAddr(0x43);
            putrsXLCD( "5 Pressed");
//            OpenPWM1(176);
//            Delay1KTCYx(255);
//            ClosePWM1();
        }
        if (PORTC == 0x40)
        {
            PORTA = 4;
            while( BusyXLCD() );
            WriteCmdXLCD(0b00000001);
            while( BusyXLCD() );
            SetDDRamAddr(0x43);
            putrsXLCD( "4 Pressed");
//            OpenPWM1(192);
//            Delay1KTCYx(255);
//            ClosePWM1();
        }
        if (PORTC == 0x30)
        {
            PORTA = 3;
            while( BusyXLCD() );
            WriteCmdXLCD(0b00000001);
            while( BusyXLCD() );
            SetDDRamAddr(0x42);
            putrsXLCD( "TEMPERATURE");
//            OpenPWM1(208);
//            Delay1KTCYx(255);
//            ClosePWM1();
//            Delay10KTCYx(255);
//            while( BusyXLCD() );
//            WriteCmdXLCD(0b00000001);
//            temp();
            

        }
        if (PORTC == 0x20)
        {
            PORTA = 2;
            while( BusyXLCD() );
            WriteCmdXLCD(0b00000001);
            while( BusyXLCD() );
            SetDDRamAddr(0x43);
            putrsXLCD( "3 Pressed");
//            OpenPWM1(224);
//            Delay1KTCYx(255);
//            ClosePWM1();
        }
        if (PORTC == 0x10)
        {
           PORTA = 1;
            while( BusyXLCD() );
            WriteCmdXLCD(0b00000001);
            while( BusyXLCD() );
            SetDDRamAddr(0x43);
            putrsXLCD( "2 Pressed");
//            OpenPWM1(240);
//            Delay1KTCYx(255);
//            ClosePWM1();
//        }
        if (PORTC == 0x00)
        {
            PORTA = 0;
            while( BusyXLCD() );
            WriteCmdXLCD(0b00000001);
            while( BusyXLCD() );
            SetDDRamAddr(0x43);
            putrsXLCD( "1 Pressed");
//            OpenPWM1(255);
//            Delay1KTCYx(255);
//            ClosePWM1();
        }
    }
   
}