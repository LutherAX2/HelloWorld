#include <p18cxxx.h>
//#include <stdio.h>
//#include <stdlib.h>
#include "meman_v0_1.h"
#include "SST39S_v1.h"
/*
 * 
 */

#pragma config OSC = HS
#pragma config WDT = OFF
#pragma config LVP = OFF

#define _XTAL_FREQ 4000000



float TestFloats[10] = {0x01100110,0x02200220,0x03300330,0x04400440,0x05500550,0x06600660,0x07700770,0x08800880,0x09900990,0x0AA00AA0};
unsigned int TestInts[10] = {0x01,0x20,0x03,0x40,0x05,0x60,0x07,0x80,0x09,0xA0};
int a;
int b;
int c;
int d;

union uFLOAT load;
union uINT look;
union uINT showme; 
void main (void)
{
    TRISB = 0x00;
    ram_init();
    PORTB = softwareID();
    Delay10KTCYx(50);
    softwareIDExit();
    //sectorErase(HRV_TRACK_SECTOR);
    //chipErase();
 
    RecordTrackInitHRV(HRV_TRACK_SECTOR, HRV_TRACK_ADDR, FLOATSIZE);
        PORTB = RXbits.CHARBITS;
        Delay10KTCYx(100);
        PORTB = 0x01;
        Delay10KTCYx(25);
    RecordTrackInitHR(HR_TRACK_SECTOR, HR_TRACK_ADDR,INTSIZE);
        PORTB = RXbits.CHARBITS;
        Delay10KTCYx(100);
        PORTB = 0x01;
        Delay10KTCYx(25);
    RecordTrackInitTEMP(TEMP_TRACK_SECTOR, TEMP_TRACK_ADDR,FLOATSIZE);
        PORTB = RXbits.CHARBITS;
        Delay10KTCYx(100);
        PORTB = 0x01;
        Delay10KTCYx(25);
    RecordTrackInitGLUC(GLUC_TRACK_SECTOR, GLUC_TRACK_ADDR, FLOATSIZE);    
        PORTB = RXbits.CHARBITS;
        Delay10KTCYx(100);
        PORTB = 0x01;
        Delay10KTCYx(25);
   
        for (a = 0; a < 5; a++)
        {
        load.floatVAL = TestFloats[a];
        MakeRecordHRV(HRV_STORAGE_SECTOR, HRV_TRACK_SECTOR, HRV_TRACK_ADDR, load);
        PORTB = TXbitsDATA.CHARBITS;
        Delay10KTCYx(50);
        }

        for (b = 0; b < 9; b++)
        {
        load.floatVAL = TestFloats[b];
        MakeRecordGLUC(GLUC_STORAGE_SECTOR, GLUC_TRACK_SECTOR, GLUC_TRACK_ADDR, load);
        Delay10KTCYx(50);
        }


        for (c = 0; c < 7; c++)
        {
        load.floatVAL = TestFloats[c];
        MakeRecordTEMP(TEMP_STORAGE_SECTOR, TEMP_TRACK_SECTOR, TEMP_TRACK_ADDR, load);
        PORTB = TXbitsDATA.CHARBITS;
        Delay10KTCYx(50);
        }

        for (d = 0; d < 3; d++)
        { 
        look.intVAL = TestInts[d];
        MakeRecordHR(HR_STORAGE_SECTOR, HR_TRACK_SECTOR, HR_TRACK_ADDR, look);
        PORTB = TXbitsDATA.CHARBITS;
        Delay10KTCYx(50);
        }
       
    //writeDATA(HRV_TRACK_SECTOR,HRV_TRACK_ADDR,WRT5);
        a = 0;
        do
        {
            readDATA(HRV_TRACK_SECTOR, a);
            PORTB = RXbits.CHARBITS;
            Delay10KTCYx(100);
            a++;
        } while(RXbits.CHARBITS != 0xFF);
        
        a = 0;
        do
        {
            readDATA(TEMP_TRACK_SECTOR, a);
            PORTB = RXbits.CHARBITS;
            Delay10KTCYx(100);
            a++;
        } while(RXbits.CHARBITS != 0xFF);
        
        a = 0;
        do
        {
            readDATA(GLUC_TRACK_SECTOR, a);
            PORTB = RXbits.CHARBITS;
            Delay10KTCYx(100);
            a++;
        } while(RXbits.CHARBITS != 0xFF);
        
        a = 0;
        do
        {
            readDATA(HR_TRACK_SECTOR, a);
            PORTB = RXbits.CHARBITS;
            Delay10KTCYx(100);
            a++;
        }while(RXbits.CHARBITS != 0xFF);
 
        PORTB = 0x55;
        while(1);
    
}