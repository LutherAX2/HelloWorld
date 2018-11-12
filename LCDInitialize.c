#include <p18f452.h>
#include "xlcd.h"
#include <delays.h>
//#include "Speaker_Functions.h"







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

void startup_display (void)
{
    init_XLCD();
    //speaker_startup();
    while( BusyXLCD() );
    SetDDRamAddr(0x00);
    putrsXLCD("GROUP K - KRATOS");
    while( BusyXLCD() );
    SetDDRamAddr(0x43);
    putrsXLCD("HEART RATE");
    while( BusyXLCD() );
    SetDDRamAddr(0x14);
    putrsXLCD("MONITOR");
    Delay10KTCYx(255);
    while( BusyXLCD() );
    WriteCmdXLCD(0b00000001);
   
}

void default_display (void)
{
    while( BusyXLCD() );
    WriteCmdXLCD(0b00000001);
    while( BusyXLCD() );
    SetDDRamAddr(0x00);
    putrsXLCD("Select an Option");
    while( BusyXLCD() );
    SetDDRamAddr(0x40);
    putrsXLCD("A: TEMP");
    while( BusyXLCD() );
    SetDDRamAddr(0x10);
    putrsXLCD("B: HR & HRV");
    while( BusyXLCD() );
    SetDDRamAddr(0x50);
    putrsXLCD("C: LIVE");
}

