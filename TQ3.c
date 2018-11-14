#include <p18cxxx.h>
//#include <stdio.h>
//#include <stdlib.h>
#include "meman.h"
#include "SST39S_v1.h"
/*
 * 
 */

#pragma config OSC = HS
#pragma config WDT = OFF
#pragma config LVP = OFF

#define _XTAL_FREQ 4000000

//state variables
unsigned int HRtrkSTATE;
unsigned int HRVtrkSTATE;
unsigned int TEMPtrkSTATE;
unsigned int GLUCtrkSTATE;

//enum trkSTATE HRtrkSTATE;
//enum trkSTATE HRVtrkSTATE;
//enum trkSTATE TEMPtrkSTATE;
//enum trkSTATE GLUCtrkSTATE;

// record counts
unsigned char HRreccnt;
unsigned char HRVreccnt;
unsigned char TEMPreccnt;
unsigned char GLUCreccnt;

// Bit track count offsets
unsigned char HRTrkcntOffset;
unsigned char HRVTrkcntOffset;
unsigned char TEMPTrkcntOffset;
unsigned char GLUCTrkcntOffset;

// Max allowable records offsets
unsigned char HRMaxoffset; 
unsigned char HRVMaxoffset; 
unsigned char TEMPMaxoffset;
unsigned char GLUCMaxoffset; 

// Max allowable records
unsigned char HRMaxRecords; 
unsigned char HRVMaxRecords; 
unsigned char TEMPMaxRecords;
unsigned char GLUCMaxRecords; 

// Unions for measured data
union uINT HR;
union uFLOAT HRV;
union uFLOAT TEMP;
union uFLOAT GLUC;

//write addresses
unsigned int HRwriteAdd;
unsigned int HRVwriteAdd;
unsigned int TEMPwriteAdd;
unsigned int GLUCwriteAdd;

//scrollRead limits
unsigned int HRreadAddX;
unsigned int HRVreadAddX;
unsigned int TEMPreadAddX;
unsigned int GLUCreadAddX;

//scrollRead Pointers
unsigned int HRreadAdd;
unsigned int HRVreadAdd;
unsigned int TEMPreadAdd;
unsigned int GLUCreadAdd;

float TestFloats[10] = {0x01100110,0x02200220,0x03300330,0x04400440,0x05500550,0x06600660,0x07700770,0x08800880,0x09900990,0x0AA00AA0};
unsigned int TestInts[10] = {456, 24, 55, 2, 798, 43, 25, 5432, 9, 22};
int x;
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
    chipErase();
 
    HRreccnt = 0;
    HRVreccnt = 0;
    TEMPreccnt = 0;
    GLUCreccnt = 0;
    
    HRTrkcntOffset = 0;
    HRVTrkcntOffset = 0;
    TEMPTrkcntOffset = 0;
    GLUCTrkcntOffset = 0;   

    HRtrkSTATE = 0;
    HRVtrkSTATE = 0;
    TEMPtrkSTATE = 0;
    GLUCtrkSTATE = 0;
    
    HRMaxoffset = 0; 
    HRVMaxoffset = 0; 
    TEMPMaxoffset = 0;
    GLUCMaxoffset = 0;
   
    for(x=0; x<10; x++)
    {
        load.floatVAL = TestFloats[x];
        MakeRecordFloat(HRV_STORAGE_SECTOR, &HRVwriteAdd, &HRVreadAddX,HRV_TRACK_SECTOR, HRV_TRACK_ADDR, 
            &HRVTrkcntOffset, &HRVtrkSTATE,&HRVreccnt, load);
    }
    
    for(x=0; x<9; x++)
    {
        load.floatVAL = TestFloats[x];
        MakeRecordFloat(GLUC_STORAGE_SECTOR, &GLUCwriteAdd, &GLUCreadAddX,GLUC_TRACK_SECTOR, GLUC_TRACK_ADDR, 
            &GLUCTrkcntOffset, &GLUCtrkSTATE,&GLUCreccnt, load);
    }
    
    for(x=0; x<7; x++)
    {
        load.floatVAL = TestFloats[x];
        MakeRecordFloat(TEMP_STORAGE_SECTOR, &TEMPwriteAdd, &TEMPreadAddX,TEMP_TRACK_SECTOR, TEMP_TRACK_ADDR, 
            &TEMPTrkcntOffset, &TEMPtrkSTATE, &TEMPreccnt, load);
    }
    
    for(x=0; x<3; x++)
    {
        look.intVAL = TestInts[x];
        MakeRecordInt(HR_STORAGE_SECTOR, &HRwriteAdd, &HRreadAddX,HR_TRACK_SECTOR, HR_TRACK_ADDR, 
            &HRTrkcntOffset, &HRtrkSTATE, &HRreccnt, look);
    }
    
    while(1);
    
}