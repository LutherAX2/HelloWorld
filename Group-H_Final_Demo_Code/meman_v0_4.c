
#include <p18f452.h>
#include "meman_v0_4.h"
#include "SST39S_v1.h"
#include <delays.h>

float tempDispL;
float glucDispL;
float hrvDispL;
unsigned int hrDispL;

//state variables
volatile char HRScrollSTATE;
volatile char HRVScrollSTATE;
volatile char TEMPScrollSTATE;
volatile char GLUCScrollSTATE;

//enum trkSTATE HRtrkSTATE;
//enum trkSTATE HRVtrkSTATE;
//enum trkSTATE TEMPtrkSTATE;
//enum trkSTATE GLUCtrkSTATE;

// record counts
volatile unsigned char HRreccnt;
volatile unsigned char HRVreccnt;
volatile unsigned char TEMPreccnt;
volatile unsigned char GLUCreccnt;

 

// Max allowable records
volatile unsigned char HRMaxRecords; 
volatile unsigned char HRVMaxRecords; 
volatile unsigned char TEMPMaxRecords;
volatile unsigned char GLUCMaxRecords; 

// Unions for measured data
union uINT HR;
union uFLOAT HRV;
union uFLOAT TEMP;
union uFLOAT GLUC;

//write addresses
volatile unsigned int HRwriteAdd;
volatile unsigned int HRVwriteAdd;
volatile unsigned int TEMPwriteAdd;
volatile unsigned int GLUCwriteAdd;

//scrollRead limits
volatile unsigned int HRreadAddX;
volatile unsigned int HRVreadAddX;
volatile unsigned int TEMPreadAddX;
volatile unsigned int GLUCreadAddX;

//scrollRead Pointers
volatile unsigned int HRreadAdd;
volatile unsigned int HRVreadAdd;
volatile unsigned int TEMPreadAdd;
volatile unsigned int GLUCreadAdd;

volatile unsigned char HRscrollCnt;
volatile unsigned char HRVscrollCnt;
volatile unsigned char TEMPscrollCnt;
volatile unsigned char GLUCscrollCnt;

/*Q0 The system should tolerate a power/brown-out reset without corrupting information display.
 
Q7.FRreq8: The management module must to track the current write address for the next value to be written in a FlashRAM sector
Q8.FRreq1: The Storage Management Module must coordinate retrieval of data during scroll operations 
Q0.FRreq1: On startup the Storage Management Module will read user defined sectors to restore the previous state of storage processes.
Q0.FRreq2: The Storage Management Module must record the user defined maximum number of records on the flash RAM.
Q0.FRreq3:  keep track of the number of entries for each quantity (HRV,HR, Glucose, Temperature) 

 */

//Q0.FRreq1 The Storage Management Module must record the user defined maximum number of records on the flash RAM.



void meman_init(void)
{
    HRreccnt = 45;
    HRVreccnt = 0;
    TEMPreccnt = 0;
    GLUCreccnt = 0;
    
}

void eeprom_init()
{
   Delay1KTCYx(72); 
   PIE2bits.EEIE = 0;
   EECON1bits.EEPGD = 0;
   EECON1bits.CFGS = 0;
}

char readEEPROM(unsigned char ADDRESS)
{
    while(EECON1bits.WR || EECON1bits.RD);
    EEADR = ADDRESS;
    EECON1bits.RD = 1;
    return (EEDATA);

}

void writeEEPROM(unsigned char ADDRESS, unsigned char DATA)
{
    unsigned char intcontemp;
    while(EECON1bits.WR);
    EEADR = ADDRESS;
    EEDATA = DATA;
    EECON1bits.WREN = 1;
    intcontemp = INTCON;
    INTCONbits.GIE = 0;
    EECON2 = 0x55;
    EECON2 = 0xAA;
    EECON1bits.WR = 1;
    INTCON = intcontemp;
    while(!PIR2bits.EEIF);
    PIR2bits.EEIF = 0;
    EECON1bits.WREN = 0;    
}






void RecordTrackInit(unsigned char STATUS_ADDRESS, unsigned char MAX_STORE_ADDRESS, unsigned char RECCNT_ADDRESS, unsigned char TYPESIZE, 
        volatile unsigned char* reccnt,volatile unsigned char* maxcnt,volatile unsigned int* writeadd, volatile unsigned int* scrollLimit)
{
    unsigned char records;
    unsigned int writeat;
   // unsigned char stat;
    records = 0x00;
    writeat = 0;
    
    
    if(readEEPROM(STATUS_ADDRESS) == 0x0F)
    {
        *maxcnt = readEEPROM(MAX_STORE_ADDRESS);
        records = readEEPROM(RECCNT_ADDRESS);
        *reccnt = records;
        writeat = records * TYPESIZE;
        *writeadd =  writeat;
        if(writeat != 0)
            *scrollLimit = (int) (writeat - TYPESIZE); 
        else
        {
            *scrollLimit = 0;
        }
    }
    else
    {
        *scrollLimit = 0x0000;
        *writeadd = 0x0000;
        *reccnt = 0x00;
        *maxcnt = 0x00;
    }
}


void MakeRecordHR(unsigned char storeSECTOR, unsigned char RECCNT_ADDRESS, union uINT VALUE)
{
    if (HRreccnt < HRMaxRecords)
    {
        int x;
        for (x = 0; x < 2; x++)
        {
            writeDATA(storeSECTOR, HRwriteAdd, VALUE.intCHAR[x]);
            HRwriteAdd = HRwriteAdd + 1; 
        }
        
        HRreccnt = HRreccnt + 1;
        writeEEPROM(RECCNT_ADDRESS,HRreccnt);
        HRreadAddX = HRwriteAdd - 2;
    }
}

void MakeRecordHRV(unsigned char storeSECTOR, unsigned char RECCNT_ADDRESS, union uFLOAT VALUE)
{
    if(HRVreccnt < HRVMaxRecords)
    {
        int x;
        for (x = 0; x < 4; x++)
        {
            writeDATA(storeSECTOR, HRVwriteAdd, VALUE.floatCHAR[x]);
            HRVwriteAdd = HRVwriteAdd + 1; 
        }
        HRVreccnt++;
        writeEEPROM(RECCNT_ADDRESS,HRVreccnt);
        HRVreadAddX = HRVwriteAdd - 4;
    }
}

void MakeRecordTEMP(unsigned char storeSECTOR, unsigned char RECCNT_ADDRESS, union uFLOAT VALUE)
{
    if (TEMPreccnt < TEMPMaxRecords)
    {
        int x;
        for (x = 0; x < 4; x++)
        {
            writeDATA(storeSECTOR, TEMPwriteAdd, VALUE.floatCHAR[x]);
            TEMPwriteAdd = TEMPwriteAdd + 1; 
        }
        TEMPreccnt++;
        writeEEPROM(RECCNT_ADDRESS,TEMPreccnt);
        TEMPreadAddX = TEMPwriteAdd - 4;
    }
}

void MakeRecordGLUC(unsigned char storeSECTOR, unsigned char RECCNT_ADDRESS, union uFLOAT VALUE)
{
    if(GLUCreccnt < GLUCMaxRecords)
    {
        int x;
        for (x = 0; x < 4; x++)
        {
            writeDATA(storeSECTOR, GLUCwriteAdd, VALUE.floatCHAR[x]);
            GLUCwriteAdd = GLUCwriteAdd + 1; 
        }
        GLUCreccnt++;
        writeEEPROM(RECCNT_ADDRESS,GLUCreccnt);
        GLUCreadAddX = GLUCwriteAdd - 4;
    }
}


void openRecordsINTLite(unsigned char SECTOR, unsigned int* dispL, volatile unsigned int* scrollreadAdd, volatile unsigned char* reccnt, volatile unsigned char* scrollcount, volatile char* scrollState)
{
    union uINT RXint;
    int x;
    int y;
    unsigned int records;
    unsigned int nextreadADD;
    records = *reccnt;
    *scrollcount = 1;
    *dispL= 0xFFFF; 
    nextreadADD = 0;
    if(*reccnt != 0)
    {
        for(y = 0; y < INTSIZE; y++)
        {
            readDATA(SECTOR,nextreadADD);
            RXint.intCHAR[y] = RXbits.CHARBITS;
            nextreadADD++;
        }
        *dispL = RXint.intVAL;
        *scrollreadAdd = nextreadADD - INTSIZE;
        *scrollState = SCROLL_DOWN;
    }
}




void openRecordsFLOATLite(unsigned char SECTOR, float* dispL, volatile unsigned int* scrollreadAdd, volatile unsigned char* reccnt, volatile unsigned char* scrollcount, volatile char* scrollState)
{
    union uFLOAT RXfloat;
    int x;
    int y;
    unsigned int records;
    unsigned int nextreadADD;
    records = *reccnt;
    *scrollcount = 1;
    *dispL = 0xFFFFFFFF; 
    if (*reccnt != 0)
    {
        nextreadADD = 0;
        for(y = 0; y < FLOATSIZE; y++)
        {
            readDATA(SECTOR,nextreadADD);
            RXfloat.floatCHAR[y] = RXbits.CHARBITS;
            nextreadADD++;
        }     
        *dispL = RXfloat.floatVAL;
        *scrollreadAdd = nextreadADD - FLOATSIZE;
        *scrollState = SCROLL_DOWN;
    }
}



void scrollRecordsHRLite(char SECTOR, char message, volatile char* scrollState, unsigned char TYPESIZE) /* message this will be replaced with void*/
{
    union uINT RXint;
    unsigned int x;
    
    if (message == SCROLL_UP)
    {
        if (HRreadAdd != 0)
        {
            *scrollState = SCROLL_UP;
            HRreadAdd = (HRreadAdd - TYPESIZE);
            for (x = 0; x < TYPESIZE; x++)
            {
                readDATA(SECTOR, (HRreadAdd + x));
                RXint.intCHAR[x] = RXbits.CHARBITS;
            }
            hrDispL = RXint.intVAL;
            HRscrollCnt--;  
        }
    }
        
    else if(message == SCROLL_DOWN)
    {
        if(HRreadAdd != HRreadAddX)
        {
            *scrollState = SCROLL_DOWN;
            HRreadAdd = HRreadAdd + TYPESIZE;
            for (x = 0; x < TYPESIZE; x++)
            {
                readDATA(SECTOR, HRreadAdd + x);
                RXint.intCHAR[x] = RXbits.CHARBITS;
            }
            hrDispL =  RXint.intVAL; 
            HRscrollCnt++;
        }  
    }
    else
    {
        return; // misra C 2004 rules 14.10 else if constructs shall be terminated with else
    }
}
    

void scrollRecordsHRVLite(char SECTOR, char message, volatile char* scrollState, unsigned char TYPESIZE) /* message this will be replaced with void*/
{
    union uFLOAT RXfloat;
    unsigned int x;
    
    if (message == SCROLL_UP)
    {
        if (HRVreadAdd != 0)
        {
            *scrollState = SCROLL_UP;
            HRVreadAdd = HRVreadAdd - TYPESIZE;
            for (x = 0; x < TYPESIZE; x++)
            {
                readDATA(SECTOR, HRVreadAdd + x);
                RXfloat.floatCHAR[x] = RXbits.CHARBITS;
            }
            hrvDispL = RXfloat.floatVAL;
            HRVscrollCnt--;  
        }
    }
        
    else if(message == SCROLL_DOWN)
    {
        if(HRVreadAdd != HRVreadAddX)
        {
            *scrollState = SCROLL_DOWN;
            HRVreadAdd = HRVreadAdd + TYPESIZE;
            for (x = 0; x < TYPESIZE; x++)
            {
                readDATA(SECTOR, HRVreadAdd + x);
                RXfloat.floatCHAR[x] = RXbits.CHARBITS;
            }
            hrvDispL =  RXfloat.floatVAL;
            HRVscrollCnt++;
        }     
    }
    else
    {
        return; // misra C 2004 rules 14.10 else if constructs shall be terminated with else
    }
}
    

void scrollRecordsTEMPLite(char SECTOR, char message, volatile char* scrollState, unsigned char TYPESIZE) /* message this will be replaced with void*/
{
    union uFLOAT RXfloat;
    unsigned int x;
    
    if (message == SCROLL_UP)
    {
        if (TEMPreadAdd != 0)
        {
            *scrollState = SCROLL_UP;
            TEMPreadAdd = TEMPreadAdd - TYPESIZE;
            for (x = 0; x < TYPESIZE; x++)
            {
                readDATA(SECTOR, TEMPreadAdd + x);
                RXfloat.floatCHAR[x] = RXbits.CHARBITS;
            }
            tempDispL = RXfloat.floatVAL;
            TEMPscrollCnt--;  
        }
    }
        
    else if(message == SCROLL_DOWN)
    {
        if(TEMPreadAdd != TEMPreadAddX)
        {
            *scrollState = SCROLL_DOWN;
            TEMPreadAdd = TEMPreadAdd + TYPESIZE;
            for (x = 0; x < TYPESIZE; x++)
            {
                readDATA(SECTOR, TEMPreadAdd + x);
                RXfloat.floatCHAR[x] = RXbits.CHARBITS;
            }
            tempDispL =  RXfloat.floatVAL;
            TEMPscrollCnt++;
        }     
    }
    else
    {
        return; // misra C 2004 rules 14.10 else if constructs shall be terminated with else
    }
}
        
 

void scrollRecordsGLUCLite(char SECTOR, char message, volatile char* scrollState, unsigned char TYPESIZE) /* message this will be replaced with void*/
{
    union uFLOAT RXfloat;
    unsigned int x;
    
    if (message == SCROLL_UP)
    {
        if (GLUCreadAdd != 0)
        {
            *scrollState = SCROLL_UP;
            GLUCreadAdd = GLUCreadAdd - TYPESIZE;
            for (x = 0; x < TYPESIZE; x++)
            {
                readDATA(SECTOR, GLUCreadAdd + x);
                RXfloat.floatCHAR[x] = RXbits.CHARBITS;
            }
            glucDispL = RXfloat.floatVAL;
            GLUCscrollCnt--;  
        }
    }
        
    else if(message == SCROLL_DOWN)
    {
        if(GLUCreadAdd != GLUCreadAddX)
        {
            *scrollState = SCROLL_DOWN;
            GLUCreadAdd = GLUCreadAdd + TYPESIZE;
            for (x = 0; x < TYPESIZE; x++)
            {
                readDATA(SECTOR, GLUCreadAdd + x);
                RXfloat.floatCHAR[x] = RXbits.CHARBITS;
            }
            glucDispL =  RXfloat.floatVAL;
            GLUCscrollCnt++;
        }     
    }
    else
    {
        return; // misra C 2004 rules 14.10 else if constructs shall be terminated with else
    }
}

void openRecordsLite(void)
{
    openRecordsINTLite(HR_STORAGE_SECTOR, &hrDispL, &HRreadAdd, &HRreccnt, &HRscrollCnt, &HRScrollSTATE);
    openRecordsFLOATLite(HRV_STORAGE_SECTOR, &hrvDispL, &HRVreadAdd, &HRVreccnt, &HRVscrollCnt, &HRVScrollSTATE);
    openRecordsFLOATLite(TEMP_STORAGE_SECTOR, &tempDispL, &TEMPreadAdd, &TEMPreccnt, &TEMPscrollCnt, &TEMPScrollSTATE);
    openRecordsFLOATLite(GLUC_STORAGE_SECTOR, &glucDispL, &GLUCreadAdd, &GLUCreccnt, &GLUCscrollCnt, &GLUCScrollSTATE);

}


void pageScrollLite(char scrollmsg)
{
    scrollRecordsHRLite(HR_STORAGE_SECTOR, scrollmsg, &HRScrollSTATE, INTSIZE);
    scrollRecordsHRVLite(HRV_STORAGE_SECTOR, scrollmsg, &HRVScrollSTATE, FLOATSIZE);
    scrollRecordsTEMPLite(TEMP_STORAGE_SECTOR, scrollmsg, &TEMPScrollSTATE, FLOATSIZE);
    scrollRecordsGLUCLite(GLUC_STORAGE_SECTOR, scrollmsg,&GLUCScrollSTATE, FLOATSIZE);
}