#include <p18f452.h>
#include <portb.h>
#include <delays.h>
#include <timers.h>
#include <pwm.h>
#include "xlcd2.h"

#pragma config WDT = OFF
#pragma config OSC = HS
#pragma config LVP = OFF

#define XTAL_FREQ 4000000
void DelayFor18TCY (void)
{
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
}

void DelayPORXLCD (void)
{
    Delay1KTCYx(15);
    return;
}

void DelayXLCD (void)
{
    Delay1KTCYx(5);
    return;
}


void init_XLCD (void)
{
    OpenXLCD (FOUR_BIT & LINES_5X7);
    while( BusyXLCD() );
    WriteCmdXLCD( FOUR_BIT & LINES_5X7 );
    while( BusyXLCD() );
    WriteCmdXLCD( BLINK_ON );
    while( BusyXLCD() );
    WriteCmdXLCD( SHIFT_DISP_LEFT );
    
}

void speaker_config(void)
{
    TRISCbits.TRISC2 = 0;
    SetDCPWM1(30);
    OpenTimer2(TIMER_INT_OFF & T2_PS_1_16 & T2_POST_1_1);
}




void InterruptServiceHigh(void);
//void InterruptServiceLow(void);

//int x;

void main(void)
{
    init_XLCD();
    speaker_config();
INTCON3bits.INT1IF = 0;
INTCONbits.INT0IE = 1;
INTCON2bits.INTEDG1 = 1;

INTCON2bits.RBPU = 0;
INTCON3bits.INT1IP = 1;
INTCON3bits.INT1IE = 1;
//EnablePullups();

TRISA = 0;
PORTA = 0; 
TRISBbits.TRISB1 = 1;
TRISCbits.TRISC7 = 1;
TRISCbits.TRISC6 = 1;
TRISCbits.TRISC5 = 1;
TRISCbits.TRISC4 = 1;



RCONbits.IPEN = 1;
INTCONbits.GIEH = 1;
INTCONbits.PEIE = 1;

while(1)	
{
    

}

}


// High priority interrupt vector

#pragma code InterruptVectorHigh = 0x08
void InterruptVectorHigh(void)
{
_asm
goto InterruptServiceHigh
_endasm
}


// Low priority interrupt vector

//#pragma code InterruptVectorLow = 0x18
//void InterruptVectorLow(void)
//{
//_asm
//goto InterruptServiceLow
//_endasm
//}



// Interrupt Service Routine

// Interrupt pragma for high priority

#pragma code
#pragma interrupt InterruptServiceHigh
void InterruptServiceHigh()
{
// function statements

if(INTCON3bits.INT1IF)
{
//    x++;
//    if (16 == x)
//        x = 1;
//    PORTA = x;
//    Delay1KTCYx(255);
    
    PORTC = PORTC & 0xF0;
    
    
    if (PORTC == 0xF0) 
    {
        PORTA = 15; 
        while( BusyXLCD() );
    WriteCmdXLCD(0b00000001);
    while( BusyXLCD() );
    SetDDRamAddr(0x43);
    putrsXLCD("D Pressed");
    OpenPWM1(15);
        Delay1KTCYx(255);
        ClosePWM1();
    }
    
    if (PORTC == 0xE0)
    {
        PORTA = 14; 
        while( BusyXLCD() );
    WriteCmdXLCD(0b00000001);
    while( BusyXLCD() );
    SetDDRamAddr(0x43);
    putrsXLCD("E Pressed");
    OpenPWM1(31);
        Delay1KTCYx(255);
        ClosePWM1();
    }
    
    
    if (PORTC == 0xD0)
    {
        PORTA = 13; 
        while( BusyXLCD() );
    WriteCmdXLCD(0b00000001);
    while( BusyXLCD() );
    SetDDRamAddr(0x43);
    putrsXLCD("F Pressed");
    OpenPWM1(47);
        Delay1KTCYx(255);
        ClosePWM1();
    }
    
    
    if (PORTC == 0xC0)
     {
        PORTA = 12; 
        while( BusyXLCD() );
    WriteCmdXLCD(0b00000001);
    while( BusyXLCD() );
    SetDDRamAddr(0x43);
    putrsXLCD("0 Pressed");
    OpenPWM1(63);
        Delay1KTCYx(255);
        ClosePWM1();
    }
    if (PORTC == 0xB0) 
    {
        PORTA = 11; 
        while( BusyXLCD() );
    WriteCmdXLCD(0b00000001);
    while( BusyXLCD() );
    SetDDRamAddr(0x43);
    putrsXLCD("C Pressed");
    OpenPWM1(79);
        Delay1KTCYx(255);
        ClosePWM1();
    }
    
    if (PORTC == 0xA0) 
    {
        PORTA = 10; 
        while( BusyXLCD() );
    WriteCmdXLCD(0b00000001);
    while( BusyXLCD() );
    SetDDRamAddr(0x43);
    putrsXLCD("9 Pressed");
    OpenPWM1(95);
        Delay1KTCYx(255);
        ClosePWM1();
    }
    
    
    if (PORTC == 0x90)
    {
        PORTA = 9; 
        while( BusyXLCD() );
    WriteCmdXLCD(0b00000001);
    while( BusyXLCD() );
    SetDDRamAddr(0x43);
    putrsXLCD("8 Pressed");
    OpenPWM1(111);
        Delay1KTCYx(255);
        ClosePWM1();
    }
    
    
    if (PORTC == 0x80) 
    {
        PORTA = 8; 
        while( BusyXLCD() );
    WriteCmdXLCD(0b00000001);
    while( BusyXLCD() );
    SetDDRamAddr(0x43);
    putrsXLCD("7 Pressed");
    OpenPWM1(127);
        Delay1KTCYx(255);
        ClosePWM1();
    }
    
    
    if (PORTC == 0x70) 
    {
        PORTA = 7; 
        while( BusyXLCD() );
    WriteCmdXLCD(0b00000001);
    while( BusyXLCD() );
    SetDDRamAddr(0x43);
    putrsXLCD("B Pressed");
    OpenPWM1(143);
        Delay1KTCYx(255);
        ClosePWM1();
    }
    
    if (PORTC == 0x60) 
    {
        PORTA = 6; 
        while( BusyXLCD() );
    WriteCmdXLCD(0b00000001);
    while( BusyXLCD() );
    SetDDRamAddr(0x43);
    putrsXLCD("6 Pressed");
    OpenPWM1(159);
        Delay1KTCYx(255);
        ClosePWM1();
    }
    
    
    if (PORTC == 0x50)
    {
        PORTA = 5; 
        while( BusyXLCD() );
    WriteCmdXLCD(0b00000001);
    while( BusyXLCD() );
    SetDDRamAddr(0x43);
    putrsXLCD("5 Pressed");
    OpenPWM1(175);
        Delay1KTCYx(255);
        ClosePWM1();
    }
    
    
    if (PORTC == 0x40)
     {
        PORTA = 4; 
        while( BusyXLCD() );
    WriteCmdXLCD(0b00000001);
    while( BusyXLCD() );
    SetDDRamAddr(0x43);
    putrsXLCD("4 Pressed");
    OpenPWM1(191);
        Delay1KTCYx(255);
        ClosePWM1();
    }
    
    
    if (PORTC == 0x30)
    {
        PORTA = 3; 
        while( BusyXLCD() );
    WriteCmdXLCD(0b00000001);
    while( BusyXLCD() );
    SetDDRamAddr(0x43);
    putrsXLCD("A Pressed");
    OpenPWM1(207);
        Delay1KTCYx(255);
        ClosePWM1();
    }
    
    
    if (PORTC == 0x20)
    {
        PORTA = 2; 
        while( BusyXLCD() );
    WriteCmdXLCD(0b00000001);
    while( BusyXLCD() );
    SetDDRamAddr(0x43);
    putrsXLCD("3 Pressed");
    OpenPWM1(223);
        Delay1KTCYx(255);
        ClosePWM1();
    }
    
    
    if (PORTC == 0x10)
     {
        PORTA = 1; 
        while( BusyXLCD() );
    WriteCmdXLCD(0b00000001);
    while( BusyXLCD() );
    SetDDRamAddr(0x43);
    putrsXLCD("2 Pressed");
    OpenPWM1(239);
        Delay1KTCYx(255);
        ClosePWM1();
    }
     
    if (PORTC == 0x00)
         {
        PORTA = 0; 
        while( BusyXLCD() );
    WriteCmdXLCD(0b00000001);
    while( BusyXLCD() );
    SetDDRamAddr(0x43);
    putrsXLCD("1 Pressed");
    OpenPWM1(255);
        Delay1KTCYx(255);
        ClosePWM1();
    }
    
    

    
    
    
    
    
    
    
    
    
//x = 0;
//
//while(x <= 2)
//{
//PORTE = 0x01;
//
//Delay10KTCYx(120);	// Delay of 1 sec with 12 MHz
//
//PORTE = 0x02;
//
//Delay10KTCYx(120);
//
//x++;
    
INTCON3bits.INT1IF = 0;
}

// return from high priority interrupt

}


// Interrupt pragma for low priority

//#pragma code
//#pragma interrupt InterruptServiceLow
//void InterruptServiceLow()
//{
//// function statements
//
//// return from low priority interrupt
//
//}

