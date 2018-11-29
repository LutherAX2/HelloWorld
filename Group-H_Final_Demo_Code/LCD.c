#include <p18f452.h>
#include <delays.h>
#include "xlcd3.h"
#include "LCD_Functions.h"
#include <stdio.h>
#include <stdlib.h>
#include "SST39S_v1.h"
#include "meman_v0_4.h"


extern int scrolled_disp;
extern int state;
extern int interval;
extern int num_measures;
char interval_disp[8];
char num_disp[8];
char page[8];
char totalpages[8];
char numtodisp[8];
int integers;
int decimals;
char integerchar[8];
char decimalchar[8];



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

void startup_display(void)
{
    while( BusyXLCD() );
    WriteCmdXLCD(0b00000001);
    while( BusyXLCD() );
    SetDDRamAddr(0x04);
    putrsXLCD("GROUP H:");
    while( BusyXLCD() );
    SetDDRamAddr(0x42);
    putrsXLCD("HELLO-WORLD");
    while( BusyXLCD() );
    SetDDRamAddr(0x13);
    putrsXLCD("Heart Rate");
    while( BusyXLCD() );
    SetDDRamAddr(0x54);
    putrsXLCD("Monitor");
    Delay10KTCYx(255);
    
    
}

void display_options (void)
{
    while( BusyXLCD() );
    WriteCmdXLCD(0b00000001);
    while( BusyXLCD() );
    SetDDRamAddr(0x04);
    putrsXLCD("MAIN MENU");
    if (scrolled_disp == 0)
    {
        while( BusyXLCD() );
        SetDDRamAddr(0x40);
        putrsXLCD("1: HR/HRV");
        while( BusyXLCD() );
        SetDDRamAddr(0x10);
        putrsXLCD("2: Glucose");
        while( BusyXLCD() );
        SetDDRamAddr(0x50);
        putrsXLCD("3: Temperature");
    }
    if (scrolled_disp == 1)
    {
        while( BusyXLCD() );
        SetDDRamAddr(0x40);
        putrsXLCD("4: Live Mode");
        while( BusyXLCD() );
        SetDDRamAddr(0x10);
        putrsXLCD("5: Records");
        while( BusyXLCD() );
        SetDDRamAddr(0x50);
        putrsXLCD("6: Settings");
    }
    
}

void settings_display(void)
{
    INTCONbits.GIEH = 1;
    while ( state==6 )
    {
    while( BusyXLCD() );
    WriteCmdXLCD(0b00000001);
    while( BusyXLCD() );
    SetDDRamAddr(0x04);
    putrsXLCD("SETTINGS");
    while( BusyXLCD() );
    SetDDRamAddr(0x40);
    putrsXLCD("7: Intervals");
    while( BusyXLCD() );
    SetDDRamAddr(0x10);
    putrsXLCD("8: # of Readings");
    while( BusyXLCD() );
    SetDDRamAddr(0x50);
    putrsXLCD("9: Clear Records");
    Delay1KTCYx(255);
    }
    
}


void storage_interval(void)
{
    INTCONbits.GIEH = 1;
    while (state == 7)
    {
    while( BusyXLCD() );
    WriteCmdXLCD(0b00000001);
    while( BusyXLCD() );
    SetDDRamAddr(0x00);
    putrsXLCD("STORAGE INTERVAL");
    while( BusyXLCD() );
    SetDDRamAddr(0x40);
    putrsXLCD("1: No Storage");
    while( BusyXLCD() );
    SetDDRamAddr(0x10);
    putrsXLCD("2: Every Record");
    while( BusyXLCD() );
    SetDDRamAddr(0x50);
    putrsXLCD("3: Alternating");
     Delay1KTCYx(255);
    }
}


void storage_number (void)
{
    INTCONbits.GIEH = 1;
    while (state == 8)
    {
    while( BusyXLCD() );
    WriteCmdXLCD(0b00000001);
    while( BusyXLCD() );
    SetDDRamAddr(0x02);
    putrsXLCD("# OF READINGS");
    while( BusyXLCD() );
    SetDDRamAddr(0x40);
    putrsXLCD("1: 1");
    while( BusyXLCD() );
    SetDDRamAddr(0x10);
    putrsXLCD("2: 5");
    while( BusyXLCD() );
    SetDDRamAddr(0x50);
    putrsXLCD("3: 10");
     Delay1KTCYx(255);
    }
}
    
void clear_records (void)
{
    INTCONbits.GIEH = 1;
    while (state == 9)
    {
    while( BusyXLCD() );
    WriteCmdXLCD(0b00000001);
    while( BusyXLCD() );
    SetDDRamAddr(0x02);
    putrsXLCD("CLEAR RECORDS");
    while( BusyXLCD() );
    SetDDRamAddr(0x40);
    putrsXLCD("Are you sure?");
    while( BusyXLCD() );
    SetDDRamAddr(0x10);
    putrsXLCD("C   : Yes");
    while( BusyXLCD() );
    SetDDRamAddr(0x50);
    putrsXLCD("Home: No");
     Delay1KTCYx(255);
    }
}

void live_mode(void)
{
    INTCONbits.GIEH = 1;
    itoa(interval,interval_disp);
    itoa(num_measures, num_disp);
    while( BusyXLCD() );
    WriteCmdXLCD(0b00000001);
    while( BusyXLCD() );
    SetDDRamAddr(0x03);
    putrsXLCD("LIVE MODE");
    while( BusyXLCD() );
    SetDDRamAddr(0x40);
    putrsXLCD("Refresh Rate:10s");
    while( BusyXLCD() );
    SetDDRamAddr(0x10);
    putrsXLCD("Interval    : ");
    while( BusyXLCD() );
    putsXLCD(interval_disp);
    while( BusyXLCD() );
    SetDDRamAddr(0x50);
    putrsXLCD("# Measures  :");
    while( BusyXLCD() );
    if (num_measures != 10) SetDDRamAddr(0x5E);
    putsXLCD(num_disp);
    Delay10KTCYx(255);
    Delay10KTCYx(255);
}

void instructions (void)
{
    INTCONbits.GIEH = 1;    
    while( BusyXLCD() );
    WriteCmdXLCD(0b00000001);
    while( BusyXLCD() );
    SetDDRamAddr(0x02);
    putrsXLCD("INSERT FINGER");
    while( BusyXLCD() );
    SetDDRamAddr(0x40);
    putrsXLCD("Measuring will");
    while( BusyXLCD() );
    SetDDRamAddr(0x10);
    putrsXLCD("begin in:");
    while( BusyXLCD() );
    SetDDRamAddr(0x57);
    putrsXLCD("5");
    if (state!=0) Delay10KTCYx(100);
    if (state!=0) while( BusyXLCD() );
    if (state!=0) SetDDRamAddr(0x57);
    if (state!=0) putrsXLCD("4");
    if (state!=0) Delay10KTCYx(100);
    if (state!=0) while( BusyXLCD() );
    if (state!=0) SetDDRamAddr(0x57);
    if (state!=0) putrsXLCD("3");
    if (state!=0) Delay10KTCYx(100);
    if (state!=0) while( BusyXLCD() );
    if (state!=0) SetDDRamAddr(0x57);
    if (state!=0) putrsXLCD("2");
    if (state!=0) Delay10KTCYx(100);
    if (state!=0) while( BusyXLCD() );
    if (state!=0) SetDDRamAddr(0x57);
    if (state!=0) putrsXLCD("1");
    if (state!=0) Delay10KTCYx(100);
    if (state!=0) while( BusyXLCD() );
    if (state!=0) WriteCmdXLCD(0b00000001);
    if (state!=0) while( BusyXLCD() );
    if (state!=0) SetDDRamAddr(0x00);
    if (state!=0) putrsXLCD("Measuring...");
}



void display_records (void)
{
    INTCONbits.GIEH = 1;
    openRecordsLite();
    while (state==5)
    {
    RefreshLCD();
    Delay1KTCYx(255);
    }
    
}

void RefreshLCD(void)
 {
 //    for (i=0; i < 4; i++)
 //    {
     itoa(HRscrollCnt, page);
     itoa(HRreccnt, totalpages);
         while (BusyXLCD());
         WriteCmdXLCD(0b00000001);
         itoa(hrDispL, numtodisp);
         putsXLCD(numtodisp);      
         while( BusyXLCD() );
         SetDDRamAddr(0x00);
         putrsXLCD("HR  :");
         SetDDRamAddr(0x06);
         putsXLCD(numtodisp);
         
         while( BusyXLCD() );
         SetDDRamAddr(0x0B);
         putsXLCD(page);
         while( BusyXLCD() );
         SetDDRamAddr(0x0D);
         putrsXLCD("/");
         while( BusyXLCD() );
         SetDDRamAddr(0x0E);
         putsXLCD(totalpages);
         
         integers = hrvDispL;
         //decimals = ((values[i] * 100)%100);
         decimals = fmod((hrvDispL * 10),10);
         //itoa(values[i], output);
         itoa(integers, integerchar);
         itoa(decimals, decimalchar);
         SetDDRamAddr(0x40);
         putrsXLCD("HRV :");
         while( BusyXLCD() );         
         SetDDRamAddr(0x46);
         putsXLCD(integerchar);
         putrsXLCD(".");
         putsXLCD(decimalchar);  
         
         
         integers = tempDispL;
         //decimals = ((values[i] * 100)%100);
         decimals = fmod((tempDispL * 10),10);
         //itoa(values[i], output);
         itoa(integers, integerchar);
         itoa(decimals, decimalchar);       
         while( BusyXLCD() );         
         SetDDRamAddr(0x10);
         putrsXLCD("Temp:");
         while( BusyXLCD() );         
         SetDDRamAddr(0x16);
         putsXLCD(integerchar);
         putrsXLCD(".");
         putsXLCD(decimalchar); 
         
         integers = glucDispL;
         //decimals = ((values[i] * 100)%100);
         decimals = fmod((glucDispL * 10),10);
         //itoa(values[i], output);
         itoa(integers, integerchar);
         itoa(decimals, decimalchar);       
         while( BusyXLCD() );         
         SetDDRamAddr(0x50);
         putrsXLCD("Gluc:");
         while( BusyXLCD() );         
         SetDDRamAddr(0x56);
         putsXLCD(integerchar);
         putrsXLCD(".");
         putsXLCD(decimalchar);
     
 }

void clear_everything(void)
{
    chipErase();
    
    HRreccnt = 0;
    writeEEPROM(HR_RCCNT,HRreccnt);
    HRwriteAdd = 0;
    HRreadAdd = 0; 
    HRreadAddX = 0;
    
    HRVreccnt = 0;
    writeEEPROM(HRV_RCCNT,HRVreccnt);
    HRVwriteAdd = 0;
    HRVreadAdd = 0; 
    HRVreadAddX = 0;
    
    TEMPreccnt = 0;
    writeEEPROM(TEMP_RCCNT,TEMPreccnt);
    TEMPwriteAdd = 0;
    TEMPreadAdd = 0; 
    TEMPreadAddX = 0;
    
    GLUCreccnt = 0;
    writeEEPROM(GLUC_RCCNT,GLUCreccnt);
    GLUCwriteAdd = 0;
    GLUCreadAdd = 0; 
    GLUCreadAddX = 0;
}