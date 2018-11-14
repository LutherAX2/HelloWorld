#include <p18cxxx.h>
//#include <stdio.h>
//#include <stdlib.h>
#include "meman_v0.h"
#include "SST39S_v1.h"
/*
 * 
 */

#pragma config OSC = HS
#pragma config WDT = OFF
#pragma config LVP = OFF

#define _XTAL_FREQ 4000000



float TestFloats[10] = {4.5, 3.4, 52.9, 48.5, 5.9, 87.1, 0.2, 78.9, 52.2, 100.1};
unsigned int TestInts[10] = {456, 24, 55, 2, 798, 43, 25, 5432, 9, 22};
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
 
    RecordTrackInit(HRV_TRACK_SECTOR, HRV_TRACK_ADDR, &HRVTrkcntOffset, &HRVtrkSTATE, 
            &HRVreccnt,&HRVMaxoffset, &HRVwriteAdd, FLOATSIZE);
        PORTB = RXbits.CHARBITS;
        Delay10KTCYx(100);
        PORTB = 0x01;
        Delay10KTCYx(25);
    RecordTrackInit(HR_TRACK_SECTOR, HR_TRACK_ADDR, &HRTrkcntOffset, &HRtrkSTATE, 
            &HRreccnt,&HRMaxoffset, &HRwriteAdd, INTSIZE);
        PORTB = RXbits.CHARBITS;
        Delay10KTCYx(100);
        PORTB = 0x01;
        Delay10KTCYx(25);
    RecordTrackInit(TEMP_TRACK_SECTOR, TEMP_TRACK_ADDR, &TEMPTrkcntOffset, &TEMPtrkSTATE, 
        &TEMPreccnt,&TEMPMaxoffset, &TEMPwriteAdd, FLOATSIZE);
        PORTB = RXbits.CHARBITS;
        Delay10KTCYx(100);
        PORTB = 0x01;
        Delay10KTCYx(25);
    RecordTrackInit(GLUC_TRACK_SECTOR, GLUC_TRACK_ADDR, &GLUCTrkcntOffset, &GLUCtrkSTATE, 
        &GLUCreccnt,&GLUCMaxoffset, &GLUCwriteAdd, FLOATSIZE);    
        PORTB = RXbits.CHARBITS;
        Delay10KTCYx(100);
        PORTB = 0x01;
        Delay10KTCYx(25);
   
        for (a = 0; a < 5; a++)
        {
        load.floatVAL = TestFloats[a];
        MakeRecordFloat(HRV_STORAGE_SECTOR, &HRVwriteAdd, &HRVreadAddX,&HRVreccnt, load);
        RecordTrack(HRV_TRACK_SECTOR, HRV_TRACK_ADDR, &HRVTrkcntOffset, &HRVtrkSTATE);
        PORTB = TXbitsDATA.CHARBITS;
        Delay10KTCYx(50);
        }

        for (b = 0; b < 9; b++)
        {
        load.floatVAL = TestFloats[b];
        MakeRecordFloat(GLUC_STORAGE_SECTOR, &GLUCwriteAdd, &GLUCreadAddX,&GLUCreccnt, load);
        RecordTrack(GLUC_TRACK_SECTOR, GLUC_TRACK_ADDR,&GLUCTrkcntOffset, &GLUCtrkSTATE);
        PORTB = TXbitsDATA.CHARBITS;
        Delay10KTCYx(50);
        }


        for (c = 0; c < 7; c++)
        {
        load.floatVAL = TestFloats[c];
        MakeRecordFloat(TEMP_STORAGE_SECTOR, &TEMPwriteAdd, &TEMPreadAddX, &TEMPreccnt, load);
        RecordTrack(TEMP_TRACK_SECTOR, TEMP_TRACK_ADDR,&TEMPTrkcntOffset, &TEMPtrkSTATE);
        PORTB = TXbitsDATA.CHARBITS;
        Delay10KTCYx(50);
        }

        for (d = 0; d < 3; d++)
        { 
        look.intVAL = TestInts[d];
        MakeRecordInt(HR_STORAGE_SECTOR, &HRwriteAdd, &HRreadAddX, &HRreccnt, look);
        RecordTrack(HR_TRACK_SECTOR, HR_TRACK_ADDR,&HRTrkcntOffset, &HRtrkSTATE);
        PORTB = TXbitsDATA.CHARBITS;
        Delay10KTCYx(50);
        }
       
    //writeDATA(HRV_TRACK_SECTOR,HRV_TRACK_ADDR,WRT5);
    
    while(1)
    {
//        readDATA(HRV_TRACK_SECTOR,HRV_TRACK_ADDR);
//        PORTB = RXbits.CHARBITS;
//        Delay10KTCYx(100);
//        readDATA(HRV_TRACK_SECTOR,HRV_TRACK_ADDR + 1);
//        PORTB = RXbits.CHARBITS;
//        Delay10KTCYx(100);
        
        PORTB = 0xFF;
        Delay10KTCYx(200);
        PORTB = HRVreccnt;
        Delay10KTCYx(200);
        PORTB = GLUCreccnt;
        Delay10KTCYx(200);
        PORTB = TEMPreccnt;
        Delay10KTCYx(200);
        PORTB = HRreccnt;
        Delay10KTCYx(200);
    }
 
    
}