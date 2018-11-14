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

float TestFloats[10] = {4.5, 3.4, 52.9, 48.5, 5.9, 87.1, 0.2, 78.9, 52.2, 100.1};
int x;
union uFLOAT load;

void main (void)
{
    TRISB = 0x00;
    ram_init();
    PORTB = softwareID();
    Delay10KTCYx(50);
    softwareIDExit();
    //chipErase();
    
    RecordTrackInit(HRV_TRACK_SECTOR, HRV_TRACK_ADDR, &HRVTrkcntOffset, &HRVtrkSTATE, 
            &HRVreccnt,&HRVMaxoffset, &HRVwriteAdd, FLOATSIZE);
 
//   Delay10KTCYx(200);
//   PORTB = HRVTrkcntOffset;
//       Delay10KTCYx(200);
//   for(x=0; x<10; x++)
//   {
//       load.floatVAL = TestFloats[x];
//       MakeRecordFloat(HRV_STORAGE_SECTOR, &HRVwriteAdd, &HRVreadAddX,HRV_TRACK_SECTOR, HRV_TRACK_ADDR, 
//           &HRVTrkcntOffset, &HRVtrkSTATE,&HRVreccnt, load);
//   }


    readDATA(HRV_TRACK_SECTOR,HRV_TRACK_ADDR);
    PORTB = RXbits.CHARBITS;
   
    
    
    while(1){
    Delay10KTCYx(200);
    PORTB = 0xFF;
    Delay10KTCYx(200);    
    PORTB = HRVreccnt;
    Delay10KTCYx(200);
    PORTB = HRVTrkcntOffset;
    Delay10KTCYx(200);
    PORTB = HRVtrkSTATE;
    }
    
    while(1);
    
}