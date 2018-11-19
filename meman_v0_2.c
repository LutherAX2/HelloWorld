
#include <p18f452.h>
#include "meman_v0_2.h"
#include "SST39S_v1.h"
#include <delays.h>

float tempDisp[4] = {4.6, 5.5, 6.4, 5.5};
float glucDisp[4] = {0.0, 0.0, 0.0, 0.0};
float hrvDisp[4] = {0.0, 0.0, 0.0, 0.0};
unsigned int hrDisp[4] = {28, 51, 55, 88};

//state variables
volatile unsigned int HRtrkSTATE;
volatile unsigned int HRVtrkSTATE;
volatile unsigned int TEMPtrkSTATE;
volatile unsigned int GLUCtrkSTATE;

//enum trkSTATE HRtrkSTATE;
//enum trkSTATE HRVtrkSTATE;
//enum trkSTATE TEMPtrkSTATE;
//enum trkSTATE GLUCtrkSTATE;

// record counts
volatile unsigned char HRreccnt;
volatile unsigned char HRVreccnt;
volatile unsigned char TEMPreccnt;
volatile unsigned char GLUCreccnt;

// Bit track count offsets
volatile unsigned char HRTrkcntOffset;
volatile unsigned char HRVTrkcntOffset;
volatile unsigned char TEMPTrkcntOffset;
volatile unsigned char GLUCTrkcntOffset;

// Max allowable records offsets
volatile unsigned char HRMaxoffset; 
volatile unsigned char HRVMaxoffset; 
volatile unsigned char TEMPMaxoffset;
volatile unsigned char GLUCMaxoffset; 

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


void openRecordsINT(unsigned char SECTOR, unsigned int* dispArr, volatile unsigned int* scrollreadAdd, volatile unsigned char* reccnt, volatile unsigned char* scrollcount)
{
    union uINT RXint;
    int x;
    int y;
    unsigned int records;
    unsigned int nextreadADD;
    records = *reccnt;
    for(x =0; x < 4; x++)
    {
        dispArr[x] = 0xFFFF; 
    }
    
    if (records <= 0x04)
    {
        *scrollcount = records;
    }
    else
    {
        *scrollcount = 0x04;
    }
    
    if (records < 5)
    {
        nextreadADD = 0;
        for(x = 0; x < records; x++)
        {
            for(y = 0; y < INTSIZE; y++)
            {
                readDATA(SECTOR,nextreadADD);
                RXint.intCHAR[y] = RXbits.CHARBITS;
            }
            dispArr[x] = RXint.intVAL;
            nextreadADD = nextreadADD + INTSIZE;
        }
    }
    else
    {
        nextreadADD = 0;
        for(x = 0; x < 4; x++)
        {
            for(y = 0; y < INTSIZE; y++)
            {
                readDATA(SECTOR,nextreadADD);
                RXint.intCHAR[y] = RXbits.CHARBITS;
            }
            dispArr[x] = RXint.intVAL;
            nextreadADD = nextreadADD + INTSIZE;
        }
    }
    *scrollreadAdd = nextreadADD;
}




void openRecordsFLOAT(unsigned char SECTOR, float* dispArr, volatile unsigned int* scrollreadAdd, volatile unsigned char* reccnt, volatile unsigned char* scrollcount)
{
    union uFLOAT RXfloat;
    int x;
    int y;
    unsigned int records;
    unsigned int nextreadADD;
    records = *reccnt;
    for(x =0; x < 4; x++)
    {
        dispArr[x] = 0xFFFFFFFF; 
    }
    
    if (records <= 0x04)
    {
        *scrollcount = records;
    }
    else
    {
        *scrollcount = 0x04;
    }
    
    if (records < 5)
    {
        nextreadADD = 0;
        for(x = 0; x < records; x++)
        {
            for(y = 0; y < FLOATSIZE; y++)
            {
                readDATA(SECTOR,nextreadADD);
                RXfloat.floatCHAR[y] = RXbits.CHARBITS;
            }
            dispArr[x] = RXfloat.floatVAL;
            nextreadADD = nextreadADD + FLOATSIZE;
        }
    }
    else
    {
        nextreadADD = 0;
        for(x = 0; x < 4; x++)
        {
            for(y = 0; y < FLOATSIZE; y++)
            {
                readDATA(SECTOR,nextreadADD);
                RXfloat.floatCHAR[y] = RXbits.CHARBITS;
            }
            dispArr[x] = RXfloat.floatVAL;
            nextreadADD = nextreadADD + FLOATSIZE;
        }
    }
    *scrollreadAdd = nextreadADD;
}



void scrollRecordsHR(char SECTOR, char message, unsigned char TYPESIZE) /* message this will be replaced with void*/
{
    /*pend on message box*/
    /* pend on flashRAM mutex*/
	union uINT RXint;
    int x;
    
    if (message == SCROLL_UP && HRreccnt > 4)
    {
    	if (HRreadAdd != 0)
    	{
    		HRreadAdd = HRreadAdd - TYPESIZE;
            for (x = 0; x < TYPESIZE; x++)
            {
                readDATA(SECTOR, HRreadAdd + x);
                RXint.intCHAR[x] = RXbits.CHARBITS;
            }
            hrDisp[3] = hrDisp[2];
            hrDisp[2] = hrDisp[1]; 
            hrDisp[1] = hrDisp[0];
            hrDisp[0] = RXint.intVAL;
    	}
        HRscrollCnt--;
    }
    else if(message == SCROLL_DOWN && HRreccnt > 4)
    {
        if(HRreadAdd != HRreadAddX)
        {
            HRreadAdd = HRreadAdd + TYPESIZE;
            for (x = 0; x < TYPESIZE; x++)
            {
                readDATA(SECTOR, HRreadAdd + x);
                RXint.intCHAR[x] = RXbits.CHARBITS;
            }
            hrDisp[0] = hrDisp[1];
            hrDisp[1] = hrDisp[2];
            hrDisp[2] = hrDisp[3];
            hrDisp[3] =  RXint.intCHAR[x];            
        }
        HRscrollCnt++;
    }
    else
    {
        return; // misra C 2004 rules 14.10 else if constructs shall be terminated with else
    }
}
    


    

void scrollRecordsTEMP(char SECTOR, char message, unsigned char TYPESIZE) /* message this will be replaced with void*/
{
    /*pend on message box*/
    /* pend on flashRAM mutex*/
	union uFLOAT RXfloat;
    int x;
    
    if (message == SCROLL_UP && TEMPreccnt > 4)
    {
    	if (TEMPreadAdd != 0)
    	{
    		TEMPreadAdd = TEMPreadAdd - TYPESIZE;
            for (x = 0; x < TYPESIZE; x++)
            {
                readDATA(SECTOR, TEMPreadAdd + x);
                RXfloat.floatCHAR[x] = RXbits.CHARBITS;
            }
            tempDisp[3] = tempDisp[2];
            tempDisp[2] = tempDisp[1]; 
            tempDisp[1] = tempDisp[0];
            tempDisp[0] = RXfloat.floatVAL;
    	}
        TEMPscrollCnt--;
    }
    else if(message == SCROLL_DOWN && TEMPreccnt > 4)
    {
        if(TEMPreadAdd != TEMPreadAddX)
        {
            TEMPreadAdd = TEMPreadAdd + TYPESIZE;
            for (x = 0; x < TYPESIZE; x++)
            {
                readDATA(SECTOR, TEMPreadAdd + x);
                RXfloat.floatCHAR[x] = RXbits.CHARBITS;
            }
            tempDisp[0] = tempDisp[1];
            tempDisp[1] = tempDisp[2];
            tempDisp[2] = tempDisp[3];
            tempDisp[3] =  RXfloat.floatCHAR[x];            
        }
        TEMPscrollCnt++;
    }
    else
    {
        return; // misra C 2004 rules 14.10 else if constructs shall be terminated with else
    }
}
    
void scrollRecordsHRV(char SECTOR, char message, unsigned char TYPESIZE) /* message this will be replaced with void*/
{
    /*pend on message box*/
    /* pend on flashRAM mutex*/
	union uFLOAT RXfloat;
    int x;
    
    if (message == SCROLL_UP && HRVreccnt > 4)
    {
    	if (HRVreadAdd != 0)
    	{
    		HRVreadAdd = HRVreadAdd - TYPESIZE;
            for (x = 0; x < TYPESIZE; x++)
            {
                readDATA(SECTOR, HRVreadAdd + x);
                RXfloat.floatCHAR[x] = RXbits.CHARBITS;
            }
            hrvDisp[3] = hrvDisp[2];
            hrvDisp[2] = hrvDisp[1]; 
            hrvDisp[1] = hrvDisp[0];
            hrvDisp[0] = RXfloat.floatVAL;
    	}
        HRVscrollCnt--;
    }
    else if(message == SCROLL_DOWN && HRVreccnt > 4)
    {
        if(HRVreadAdd != HRVreadAddX)
        {
            HRVreadAdd = HRVreadAdd + TYPESIZE;
            for (x = 0; x < TYPESIZE; x++)
            {
                readDATA(SECTOR, HRVreadAdd + x);
                RXfloat.floatCHAR[x] = RXbits.CHARBITS;
            }
            hrvDisp[0] = hrvDisp[1];
            hrvDisp[1] = hrvDisp[2];
            hrvDisp[2] = hrvDisp[3];
            hrvDisp[3] =  RXfloat.floatCHAR[x];            
        }
        HRVscrollCnt++;
    }
    else
    {
        return; // misra C 2004 rules 14.10 else if constructs shall be terminated with else
    }
}
    
void scrollRecordsGLUC(char SECTOR, char message, unsigned char TYPESIZE) /* message this will be replaced with void*/
{
    /*pend on message box*/
    /* pend on flashRAM mutex*/
	union uFLOAT RXfloat;
    int x;
    
    if (message == SCROLL_UP && GLUCreccnt > 4)
    {
    	if (GLUCreadAdd != 0)
    	{
    		GLUCreadAdd = GLUCreadAdd - TYPESIZE;
            for (x = 0; x < TYPESIZE; x++)
            {
                readDATA(SECTOR, GLUCreadAdd + x);
                RXfloat.floatCHAR[x] = RXbits.CHARBITS;
            }
            glucDisp[3] = glucDisp[2];
            glucDisp[2] = glucDisp[1]; 
            glucDisp[1] = glucDisp[0];
            glucDisp[0] = RXfloat.floatVAL;
    	}
        GLUCscrollCnt--;
    }
    else if(message == SCROLL_DOWN && GLUCreccnt > 4)
    {
        if(GLUCreadAdd != GLUCreadAddX)
        {
            GLUCreadAdd = GLUCreadAdd + TYPESIZE;
            for (x = 0; x < TYPESIZE; x++)
            {
                readDATA(SECTOR, GLUCreadAdd + x);
                RXfloat.floatCHAR[x] = RXbits.CHARBITS;
            }
            glucDisp[0] = glucDisp[1];
            glucDisp[1] = glucDisp[2];
            glucDisp[2] = glucDisp[3];
            glucDisp[3] =  RXfloat.floatCHAR[x];            
        }
        GLUCscrollCnt++;
    }
    else
    {
        return; // misra C 2004 rules 14.10 else if constructs shall be terminated with else
    }
}