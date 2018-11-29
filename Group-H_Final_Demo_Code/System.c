#include <p18f452.h>
#include <portb.h>
#include <delays.h>
#include <timers.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "xlcd3.h"
#include "HeartRate_Functions.h"
#include "Keypad_Functions.h"
#include "LCD_Functions.h"
#include "Speaker_Functions.h"
#include "SST39S_v1.h"
#include "meman_v0_4.h"
#define FIRST_BOOT 0xFF
#pragma config WDT = OFF
#pragma config OSC = HS
#pragma config LVP = OFF

#define XTAL_FREQ 4000000


int keypad_input;
int scrolled_disp = 0;
int state;
int interval = 0;
int num_measures = 1;

//char BPMMessage [8];
//char HRVMessage [8];
//char PulseMessage [8];
extern int T0_finished;
extern int Beatcnt;
extern int ccpcnt;
extern int ccp1;
extern int ccp2;
extern int ccp3;
extern int ccpdif;
extern int NN50;
extern int NNcnt;


void InterruptServiceLow(void);
void InterruptServiceHigh(void);


// Interrupt Service Routine

// Interrupt pragma for low priority
// Low priority interrupt vector

#pragma code InterruptVectorLow = 0x18
void InterruptVectorLow(void)
{
_asm
goto InterruptServiceLow
_endasm
}

#pragma code


#pragma interruptlow InterruptServiceLow
void InterruptServiceLow(void)
{
  if(INTCONbits.TMR0IF == 1)
    {
        INTCONbits.TMR0IF = 0;
        T0_finished = 1;
    }
// return from low priority interrupt

}




// High priority interrupt vector

#pragma code InterruptVectorHigh = 0x08
void InterruptVectorHigh(void)
{
_asm
goto InterruptServiceHigh
_endasm
}

// Interrupt pragma for high priority

#pragma code

#pragma interrupt InterruptServiceHigh
void InterruptServiceHigh(void)
{
// function statements

if(INTCON3bits.INT1IF)
{
    INTCON3bits.INT1IF = 0;
    keypad_lcd();
   
}

if(INTCONbits.INT0IF == 1)
	{
        INTCONbits.INT0IF = 0;
        Beatcnt++;
        
        switch(ccpcnt)
        {
            case 1:
                ccp1 = ReadTimer0();
                ccpcnt++;
                break;
                
            case 2 :
                ccp2 = ReadTimer0();
                ccpcnt++;
                break;
                
            case 3 :
                ccp3 = ReadTimer0();
                ccpdif = fabs(fabs(ccp3 - ccp2) - fabs(ccp2 - ccp1));
                if(ccpdif > 196)
                {
                    NN50++;
                }
                ccp1 = ccp2;
                ccp2 = ccp3;
                break;
        }
    } 


// return from high priority interrupt
}










void main(void)
{

state = 0;
eeprom_init();
writeEEPROM(HR_TRACK_STATUS,TRACKING);
writeEEPROM(HRV_TRACK_STATUS,TRACKING);
writeEEPROM(TEMP_TRACK_STATUS,TRACKING);
writeEEPROM(GLUC_TRACK_STATUS,TRACKING);
writeEEPROM(HR_MAX_STORE, 0xFE); // first argument is 8 bit address second argument is the data to write
writeEEPROM(HRV_MAX_STORE, 0xFE);
writeEEPROM(TEMP_MAX_STORE, 0xFE);
writeEEPROM(GLUC_MAX_STORE, 0xFE);


if(readEEPROM(FIRST_BOOT) ==  FIRST_BOOT)
{
    writeEEPROM(HR_RCCNT, 0x00);
    writeEEPROM(HRV_RCCNT, 0x00);
    writeEEPROM(TEMP_RCCNT, 0x00);
    writeEEPROM(GLUC_RCCNT, 0x00);
    writeEEPROM(FIRST_BOOT, 0x00); 
}


ram_init();
HR.intVAL = 0xFFFF;
HRV.floatCHAR[0] = 0xFF;
HRV.floatCHAR[1] = 0xFF;
HRV.floatCHAR[2] = 0xFF;
HRV.floatCHAR[3] = 0xFF;

TEMP.floatCHAR[0] = 0xFF;
TEMP.floatCHAR[1] = 0xFF;
TEMP.floatCHAR[2] = 0xFF;
TEMP.floatCHAR[3] = 0xFF;

GLUC.floatCHAR[0] = 0xFF;
GLUC.floatCHAR[1] = 0xFF;
GLUC.floatCHAR[2] = 0xFF;
GLUC.floatCHAR[3] = 0xFF;

RecordTrackInit(HR_TRACK_STATUS,HR_MAX_STORE,HR_RCCNT,INTSIZE,&HRreccnt,&HRMaxRecords,&HRwriteAdd, &HRreadAddX);
RecordTrackInit(HRV_TRACK_STATUS,HRV_MAX_STORE,HRV_RCCNT,FLOATSIZE,&HRVreccnt,&HRVMaxRecords,&HRVwriteAdd, &HRVreadAddX);
RecordTrackInit(TEMP_TRACK_STATUS,TEMP_MAX_STORE,TEMP_RCCNT,FLOATSIZE,&TEMPreccnt,&TEMPMaxRecords,&TEMPwriteAdd, &TEMPreadAddX);
RecordTrackInit(GLUC_TRACK_STATUS,GLUC_MAX_STORE,GLUC_RCCNT,FLOATSIZE,&GLUCreccnt,&GLUCMaxRecords,&GLUCwriteAdd, &GLUCreadAddX);

    MakeRecordHR(HR_STORAGE_SECTOR,HR_RCCNT,HR);
    MakeRecordHRV(HRV_STORAGE_SECTOR,HRV_RCCNT,HRV);
    MakeRecordTEMP(TEMP_STORAGE_SECTOR,TEMP_RCCNT,TEMP);
    MakeRecordGLUC(GLUC_STORAGE_SECTOR,GLUC_RCCNT,GLUC);


    if (HRreccnt < HRMaxRecords)
    {
        HRreccnt = HRreccnt - 1;
        HRwriteAdd = HRwriteAdd - 2;
        writeEEPROM(HR_RCCNT,HRreccnt);
        HRreadAddX = HRwriteAdd - 2;
    }




    if (HRVreccnt < HRVMaxRecords)
    {    
        HRVreccnt = HRreccnt - 1;
        HRVwriteAdd = HRVwriteAdd - 4;
        writeEEPROM(HRV_RCCNT,HRVreccnt);
        HRVreadAddX = HRVwriteAdd - 4;
    }


    if (TEMPreccnt < TEMPMaxRecords)
    {
        TEMPreccnt = TEMPreccnt - 1;
        TEMPwriteAdd = TEMPwriteAdd - 4;
        writeEEPROM(TEMP_RCCNT,TEMPreccnt);
        TEMPreadAddX = TEMPwriteAdd - 4;       
    }   
        
   
    if (GLUCreccnt < GLUCMaxRecords)
    {    
        GLUCreccnt = GLUCreccnt - 1;
        GLUCwriteAdd = GLUCwriteAdd - 4;
        writeEEPROM(GLUC_RCCNT,GLUCreccnt);
        GLUCreadAddX = GLUCwriteAdd - 4; 
    }


init_XLCD();
keypad_config();
speaker_config();
startup_display();
while(1)	
{
    display_options();
    Delay1KTCYx(255);

}

}






