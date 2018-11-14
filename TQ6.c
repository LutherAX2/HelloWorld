#include <p18cxxx.h>
//#include <stdio.h>
//#include <stdlib.h>
#include "meman_v0_2.h"
#include "SST39S_v1.h"
/*
 * 
 */

#pragma config OSC = HS
#pragma config WDT = OFF
#pragma config LVP = OFF

#define _XTAL_FREQ 4000000



float TestFloats[10] = {0x01100110,0x02200220,0x03300330,0x04400440,0x05500550,0x06600660,0x07700770,0x08800880,0x09900990,0x0AA00AA0};
unsigned int TestInts[10] = {0x0110,0x2002,0x0330,0x0440,0x5005,0x0660,0x7007,0x8008,0x0990,0xA00A};
unsigned int a;
int b;
int c;
int d;

union uFLOAT load;
union uINT look;
union uINT showme; 


//
//#define HR_TRACK_STATUS 0x02
//#define HR_MAX_STORE 0x04
//#define HR_RCCNT 0x06
//
//#define HRV_TRACK_STATUS 0x08
//#define HRV_MAX_STORE 0x0A
//#define HRV_RCCNT 0x0C
//
//#define TEMP_TRACK_STATUS 0x0E
//#define TEMP_MAX_STORE 0x10
//#define TEMP_RCCNT 0x12
//
//#define GLUC_TRACK_STATUS 0x14
//#define GLUC_MAX_STORE 0x16
//#define GLUC_RCCNT 0x18
//
//#define TRACKING 0x0F
//#define NOT_TRACKING 0xF0


void main (void)
{
    TRISB = 0x00;
    ram_init();
    eeprom_init();
 //   sectorErase(HR_STORAGE_SECTOR);
    writeEEPROM(HR_TRACK_STATUS,TRACKING);
    writeEEPROM(HR_MAX_STORE, 0x08);
    writeEEPROM(HR_RCCNT,0x00);    
            
//    writeEEPROM(HRV_TRACK_STATUS,TRACKING);
//    writeEEPROM(HRV_MAX_STORE, 0x00);
//    writeEEPROM(HRV_RCCNT,0x45);  
//    
//    writeEEPROM(TEMP_TRACK_STATUS,TRACKING);
//    writeEEPROM(TEMP_MAX_STORE, 0x00);
//    writeEEPROM(TEMP_RCCNT,0x07);  
//    
//    writeEEPROM(GLUC_TRACK_STATUS,TRACKING);
//    writeEEPROM(GLUC_MAX_STORE, 0x00);
//    writeEEPROM(GLUC_RCCNT,0x41);  
//    
    
    //MakeRecordHR(unsigned char storeSECTOR, unsigned char RECCNT_ADDRESS, union uINT VALUE)
    
    RecordTrackInit(HR_TRACK_STATUS,HR_MAX_STORE,HR_RCCNT,INTSIZE,&HRreccnt,&HRMaxRecords,&HRwriteAdd, &HRreadAddX);
    //RecordTrackInit(HR_TRACK_STATUS,HR_MAX_STORE,HR_RCCNT,INTSIZE,&HRreccnt,&HRMaxRecords,&HRwriteAdd, &HRreadAddX);
    
    for (a = 0 ; a <10; a++)
    {
        look.intVAL = TestInts[a];
        MakeRecordHR(HR_STORAGE_SECTOR, HR_RCCNT, look);
        //PORTB = readEEPROM(HR_RCCNT);
       // Delay10KTCYx(50);
    }

    //PORTB = TXbitsDATA.CHARBITS;
    //Delay10KTCYx(50);
    
//    void RecordTrackInit(unsigned char STATUS_ADDRESS, unsigned char MAX_STORE_ADDRESS, unsigned char RECCNT_ADDRESS, unsigned char TYPESIZE, 
//        unsigned char* reccnt, unsigned char* maxcnt, unsigned int* writeadd, unsigned int* scrollLimit);
//    
//    void RecordTrackInit(unsigned char STATUS_ADDRESS, unsigned char MAX_STORE_ADDRESS, unsigned char RECCNT_ADDRESS, unsigned char TYPESIZE, 
//        unsigned char* reccnt, unsigned char* maxcnt, unsigned int* writeadd, unsigned int* scrollLimit);
//    
//    void RecordTrackInit(unsigned char STATUS_ADDRESS, unsigned char MAX_STORE_ADDRESS, unsigned char RECCNT_ADDRESS, unsigned char TYPESIZE, 
//        unsigned char* reccnt, unsigned char* maxcnt, unsigned int* writeadd, unsigned int* scrollLimit);
//    
    
    PORTB = HRreccnt;
    Delay10KTCYx(50);
    a = 0;
    do 
    {
        readDATA(HR_STORAGE_SECTOR,a);
        PORTB = RXbits.CHARBITS;
        Delay10KTCYx(50);
        a++;
    }while(RXbits.CHARBITS != 0xFF);
    PORTB = HRreccnt;
    Delay10KTCYx(50);
    PORTB = readEEPROM(HR_TRACK_STATUS);
    Delay10KTCYx(50);
    PORTB = readEEPROM(HR_RCCNT);
    while(1);
    
}