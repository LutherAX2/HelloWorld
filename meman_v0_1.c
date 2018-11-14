#include "meman_v0_1.h"
#include "SST39S_v1.h"

float tempDisp[4] = {0.0, 0.0, 0.0, 0.0};
float glucDisp[4] = {0.0, 0.0, 0.0, 0.0};
float hrvDisp[4] = {0.0, 0.0, 0.0, 0.0};
int hrDisp[4] = {0, 0, 0, 0};

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
}



void RecordTrackInitHR(unsigned char SECTOR, unsigned int ADDRESS1, unsigned char TypeSize)
{
    HRTrkcntOffset = 0;
    HRwriteAdd = 0;
    HRreccnt = 0;
    readDATA(SECTOR,ADDRESS1);
    if(RXbits.CHARBITS == 0xFF)
    {
        return;
    }
    else
    {
        while((RXbits.CHARBITS != 0xFF) && (HRreccnt < 254))
        {
            if(RXbits.CHARBITS == WRT1)
            {
                HRreccnt = HRreccnt + 1;
                HRtrkSTATE = W1;
            }
            else if(RXbits.CHARBITS == WRT2)
            {
                HRreccnt = HRreccnt + 2;
                HRtrkSTATE = W2;
            }
            else if(RXbits.CHARBITS == WRT3)
            {
                HRreccnt = HRreccnt + 3;
                HRtrkSTATE = W3;
            }
            else if(RXbits.CHARBITS == WRT4)
            {
                HRreccnt = HRreccnt + 4;
                HRtrkSTATE = W4;
            }
            else if(RXbits.CHARBITS == WRT5)
            {
                HRreccnt = HRreccnt + 5;
                HRtrkSTATE = W5;
            }
            else if(RXbits.CHARBITS == WRT6)
            {
                HRreccnt = HRreccnt + 6;
                HRtrkSTATE = W6;
            }
            else if(RXbits.CHARBITS == WRT7)
            {
                HRreccnt = HRreccnt + 7;
                HRtrkSTATE = W7;
            }
            else if(RXbits.CHARBITS == WRT8)
            {
                HRreccnt = HRreccnt + 8;
                HRtrkSTATE = W0;
            }
            else
            {
                HRreccnt = 0;
                HRTrkcntOffset = 0;
                HRMaxoffset = 0;
                HRtrkSTATE = W0;
                sectorErase(SECTOR);// erase the sector because something has been w
                break; // error
            }
            HRTrkcntOffset = HRTrkcntOffset + 0x01;
            readDATA(SECTOR,(ADDRESS1+(HRTrkcntOffset)));
        }
        HRTrkcntOffset = HRTrkcntOffset - 0x01;
        HRwriteAdd = HRwriteAdd + ((HRreccnt) * TypeSize); 
    }
}



void RecordTrackInitHRV(unsigned char SECTOR, unsigned int ADDRESS1, unsigned char TypeSize)
{
    HRVTrkcntOffset = 0;
    HRVwriteAdd = 0;
    HRVreccnt = 0;
    readDATA(SECTOR,ADDRESS1);
    if(RXbits.CHARBITS == 0xFF)
    {
        return;
    }
    else
    {
        while((RXbits.CHARBITS != 0xFF) && (HRVreccnt < 254))
        {
            if(RXbits.CHARBITS == WRT1)
            {
                HRVreccnt = HRreccnt + 1;
                HRVtrkSTATE = W1;
            }
            else if(RXbits.CHARBITS == WRT2)
            {
                HRVreccnt = HRreccnt + 2;
                HRVtrkSTATE = W2;
            }
            else if(RXbits.CHARBITS == WRT3)
            {
                HRVreccnt = HRreccnt + 3;
                HRVtrkSTATE = W3;
            }
            else if(RXbits.CHARBITS == WRT4)
            {
                HRVreccnt = HRreccnt + 4;
                HRVtrkSTATE = W4;
            }
            else if(RXbits.CHARBITS == WRT5)
            {
                HRVreccnt = HRreccnt + 5;
                HRVtrkSTATE = W5;
            }
            else if(RXbits.CHARBITS == WRT6)
            {
                HRVreccnt = HRreccnt + 6;
                HRVtrkSTATE = W6;
            }
            else if(RXbits.CHARBITS == WRT7)
            {
                HRVreccnt = HRreccnt + 7;
                HRVtrkSTATE = W7;
            }
            else if(RXbits.CHARBITS == WRT8)
            {
                HRVreccnt = HRreccnt + 8;
                HRVtrkSTATE = W0;
            }
            else
            {
                HRVreccnt = 0;
                HRVTrkcntOffset = 0;
                HRVMaxoffset = 0;
                HRVtrkSTATE = W0;
                sectorErase(SECTOR);// erase the sector because something has been w
                break; // error
            }
            HRVTrkcntOffset = HRVTrkcntOffset + 0x01;
            readDATA(SECTOR,(ADDRESS1+(HRVTrkcntOffset)));
        }
        HRVTrkcntOffset = HRVTrkcntOffset - 0x01;
        HRVwriteAdd = HRVwriteAdd + ((HRVreccnt) * TypeSize); 
    }
}




void RecordTrackInitTEMP(unsigned char SECTOR, unsigned int ADDRESS1, unsigned char TypeSize)
{
    TEMPTrkcntOffset = 0;
    TEMPwriteAdd = 0;
    TEMPreccnt = 0;
    readDATA(SECTOR,ADDRESS1);
    if(RXbits.CHARBITS == 0xFF)
    {
        return;
    }
    else
    {
        while((RXbits.CHARBITS != 0xFF) && (TEMPreccnt < 254))
        {
            if(RXbits.CHARBITS == WRT1)
            {
                TEMPreccnt = TEMPreccnt + 1;
                TEMPtrkSTATE = W1;
            }
            else if(RXbits.CHARBITS == WRT2)
            {
                TEMPreccnt = TEMPreccnt + 2;
                TEMPtrkSTATE = W2;
            }
            else if(RXbits.CHARBITS == WRT3)
            {
                TEMPreccnt = TEMPreccnt + 3;
                TEMPtrkSTATE = W3;
            }
            else if(RXbits.CHARBITS == WRT4)
            {
                TEMPreccnt = TEMPreccnt + 4;
                TEMPtrkSTATE = W4;
            }
            else if(RXbits.CHARBITS == WRT5)
            {
                TEMPreccnt = TEMPreccnt + 5;
                TEMPtrkSTATE = W5;
            }
            else if(RXbits.CHARBITS == WRT6)
            {
                TEMPreccnt = TEMPreccnt + 6;
                TEMPtrkSTATE = W6;
            }
            else if(RXbits.CHARBITS == WRT7)
            {
                TEMPreccnt = TEMPreccnt + 7;
                TEMPtrkSTATE = W7;
            }
            else if(RXbits.CHARBITS == WRT8)
            {
                TEMPreccnt = TEMPreccnt + 8;
                TEMPtrkSTATE = W0;
            }
            else
            {
                TEMPreccnt = 0;
                TEMPTrkcntOffset = 0;
                TEMPMaxoffset = 0;
                TEMPtrkSTATE = W0;
                sectorErase(SECTOR);// erase the sector because something has been w
                break; // error
            }
            TEMPTrkcntOffset = TEMPTrkcntOffset + 0x01;
            readDATA(SECTOR,(ADDRESS1+(TEMPTrkcntOffset)));
        }
        TEMPTrkcntOffset = TEMPTrkcntOffset - 0x01;
        TEMPwriteAdd = TEMPwriteAdd + ((TEMPreccnt) * TypeSize); 
    }
}


void RecordTrackInitGLUC(unsigned char SECTOR, unsigned int ADDRESS1, unsigned char TypeSize)
{
    GLUCTrkcntOffset = 0;
    GLUCwriteAdd = 0;
    GLUCreccnt = 0;
    readDATA(SECTOR,ADDRESS1);
    if(RXbits.CHARBITS == 0xFF)
    {
        return;
    }
    else
    {
        while((RXbits.CHARBITS != 0xFF) && (GLUCreccnt < 254))
        {
            if(RXbits.CHARBITS == WRT1)
            {
                GLUCreccnt = GLUCreccnt + 1;
                GLUCtrkSTATE = W1;
            }
            else if(RXbits.CHARBITS == WRT2)
            {
                GLUCreccnt = GLUCreccnt + 2;
                GLUCtrkSTATE = W2;
            }
            else if(RXbits.CHARBITS == WRT3)
            {
                GLUCreccnt = GLUCreccnt + 3;
                GLUCtrkSTATE = W3;
            }
            else if(RXbits.CHARBITS == WRT4)
            {
                GLUCreccnt = GLUCreccnt + 4;
                GLUCtrkSTATE = W4;
            }
            else if(RXbits.CHARBITS == WRT5)
            {
                GLUCreccnt = GLUCreccnt + 5;
                GLUCtrkSTATE = W5;
            }
            else if(RXbits.CHARBITS == WRT6)
            {
                GLUCreccnt = GLUCreccnt + 6;
                GLUCtrkSTATE = W6;
            }
            else if(RXbits.CHARBITS == WRT7)
            {
                GLUCreccnt = GLUCreccnt + 7;
                GLUCtrkSTATE = W7;
            }
            else if(RXbits.CHARBITS == WRT8)
            {
                GLUCreccnt = GLUCreccnt + 8;
                GLUCtrkSTATE = W0;
            }
            else
            {
                GLUCreccnt = 0;
                GLUCTrkcntOffset = 0;
                GLUCMaxoffset = 0;
                GLUCtrkSTATE = W0;
                sectorErase(SECTOR);// erase the sector because something has been w
                break; // error
            }
            GLUCTrkcntOffset = GLUCTrkcntOffset + 0x01;
            readDATA(SECTOR,(ADDRESS1+(GLUCTrkcntOffset)));
        }
        GLUCTrkcntOffset = GLUCTrkcntOffset - 0x01;
        GLUCwriteAdd = GLUCwriteAdd + ((GLUCreccnt) * TypeSize); 
    }
}



void RecordTrackHR(unsigned char trackSECTOR, unsigned int trackAddress)
{
    switch (HRtrkSTATE){
        case W0:
            writeDATA(trackSECTOR, (trackAddress +  HRTrkcntOffset), WRT1);
            HRtrkSTATE = W1;
            break;
            
        case W1:
            writeDATA(trackSECTOR, (trackAddress +  HRTrkcntOffset), WRT2);
            HRtrkSTATE = W2;
            break;           

        case W2:
            writeDATA(trackSECTOR, (trackAddress +  HRTrkcntOffset), WRT3);
            HRtrkSTATE = W3;
            break; 

        case W3:
            writeDATA(trackSECTOR, (trackAddress +  HRTrkcntOffset), WRT4);
            HRtrkSTATE = W4;
            break;
        
        case W4:
            writeDATA(trackSECTOR, (trackAddress +  HRTrkcntOffset), WRT5);
            HRtrkSTATE = W5;
            break;             
            
        case W5:
            writeDATA(trackSECTOR, (trackAddress +  HRTrkcntOffset), WRT6);
            HRtrkSTATE = W6;
            break; 
            
        case W6:
            writeDATA(trackSECTOR, (trackAddress +  HRTrkcntOffset), WRT7);
            HRtrkSTATE = W7;
            break;  

        case W7:
            writeDATA(trackSECTOR, (trackAddress +  HRTrkcntOffset), WRT8);
            HRtrkSTATE = W0;
             HRTrkcntOffset =  HRTrkcntOffset + 1;
            break;              
            
        default:
            return; // should return some kind of exception
    }
    return;
    
}


void RecordTrackHRV(unsigned char trackSECTOR, unsigned int trackAddress)
{
    switch (HRVtrkSTATE){
        case W0:
            writeDATA(trackSECTOR, (trackAddress +  HRVTrkcntOffset), WRT1);
            HRVtrkSTATE = W1;
            break;
            
        case W1:
            writeDATA(trackSECTOR, (trackAddress +  HRVTrkcntOffset), WRT2);
            HRVtrkSTATE = W2;
            break;           

        case W2:
            writeDATA(trackSECTOR, (trackAddress +  HRVTrkcntOffset), WRT3);
            HRVtrkSTATE = W3;
            break; 

        case W3:
            writeDATA(trackSECTOR, (trackAddress +  HRVTrkcntOffset), WRT4);
            HRVtrkSTATE = W4;
            break;
        
        case W4:
            writeDATA(trackSECTOR, (trackAddress +  HRVTrkcntOffset), WRT5);
            HRVtrkSTATE = W5;
            break;             
            
        case W5:
            writeDATA(trackSECTOR, (trackAddress +  HRVTrkcntOffset), WRT6);
            HRVtrkSTATE = W6;
            break; 
            
        case W6:
            writeDATA(trackSECTOR, (trackAddress +  HRVTrkcntOffset), WRT7);
            HRVtrkSTATE = W7;
            break;  

        case W7:
            writeDATA(trackSECTOR, (trackAddress +  HRVTrkcntOffset), WRT8);
            HRVtrkSTATE = W0;
             HRVTrkcntOffset =  HRVTrkcntOffset + 1;
            break;              
            
        default:
            return; // should return some kind of exception
    }
    return;
    
}


void RecordTrackTEMP(unsigned char trackSECTOR, unsigned int trackAddress)
{
    switch (TEMPtrkSTATE){
        case W0:
            writeDATA(trackSECTOR, (trackAddress +  TEMPTrkcntOffset), WRT1);
            TEMPtrkSTATE = W1;
            break;
            
        case W1:
            writeDATA(trackSECTOR, (trackAddress +  TEMPTrkcntOffset), WRT2);
            TEMPtrkSTATE = W2;
            break;           

        case W2:
            writeDATA(trackSECTOR, (trackAddress +  TEMPTrkcntOffset), WRT3);
            TEMPtrkSTATE = W3;
            break; 

        case W3:
            writeDATA(trackSECTOR, (trackAddress +  TEMPTrkcntOffset), WRT4);
            TEMPtrkSTATE = W4;
            break;
        
        case W4:
            writeDATA(trackSECTOR, (trackAddress +  TEMPTrkcntOffset), WRT5);
            TEMPtrkSTATE = W5;
            break;             
            
        case W5:
            writeDATA(trackSECTOR, (trackAddress +  TEMPTrkcntOffset), WRT6);
            TEMPtrkSTATE = W6;
            break; 
            
        case W6:
            writeDATA(trackSECTOR, (trackAddress +  TEMPTrkcntOffset), WRT7);
            TEMPtrkSTATE = W7;
            break;  

        case W7:
            writeDATA(trackSECTOR, (trackAddress +  TEMPTrkcntOffset), WRT8);
            TEMPtrkSTATE = W0;
             TEMPTrkcntOffset =  TEMPTrkcntOffset + 1;
            break;              
            
        default:
            return; // should return some kind of exception
    }
    return;
    
}

void RecordTrackGLUC(unsigned char trackSECTOR, unsigned int trackAddress)
{
    switch (GLUCtrkSTATE){
        case W0:
            writeDATA(trackSECTOR, (trackAddress +  GLUCTrkcntOffset), WRT1);
            GLUCtrkSTATE = W1;
            break;
            
        case W1:
            writeDATA(trackSECTOR, (trackAddress +  GLUCTrkcntOffset), WRT2);
            GLUCtrkSTATE = W2;
            break;           

        case W2:
            writeDATA(trackSECTOR, (trackAddress +  GLUCTrkcntOffset), WRT3);
            GLUCtrkSTATE = W3;
            break; 

        case W3:
            writeDATA(trackSECTOR, (trackAddress +  GLUCTrkcntOffset), WRT4);
            GLUCtrkSTATE = W4;
            break;
        
        case W4:
            writeDATA(trackSECTOR, (trackAddress +  GLUCTrkcntOffset), WRT5);
            GLUCtrkSTATE = W5;
            break;             
            
        case W5:
            writeDATA(trackSECTOR, (trackAddress +  GLUCTrkcntOffset), WRT6);
            GLUCtrkSTATE = W6;
            break; 
            
        case W6:
            writeDATA(trackSECTOR, (trackAddress +  GLUCTrkcntOffset), WRT7);
            GLUCtrkSTATE = W7;
            break;  

        case W7:
            writeDATA(trackSECTOR, (trackAddress +  GLUCTrkcntOffset), WRT8);
            GLUCtrkSTATE = W0;
             GLUCTrkcntOffset =  GLUCTrkcntOffset + 1;
            break;              
            
        default:
            return; // should return some kind of exception
    }
    return;
    
}


void MakeRecordHR(unsigned char storeSECTOR, unsigned int trackSECTOR, unsigned int trackAddress, union uINT VALUE)
{
    
    int x;
    for (x = 0; x < 4; x++)
    {
        writeDATA(storeSECTOR, HRwriteAdd, VALUE.intCHAR[x]);
        HRwriteAdd = HRwriteAdd + 1; 
    }
    RecordTrackHR(trackSECTOR, trackAddress);
    HRreccnt++;
    HRreadAddX = HRwriteAdd - 2;
}

void MakeRecordHRV(unsigned char storeSECTOR, unsigned int trackSECTOR, unsigned int trackAddress, union uFLOAT VALUE)
{
    
    int x;
    for (x = 0; x < 4; x++)
    {
        writeDATA(storeSECTOR, HRVwriteAdd, VALUE.floatCHAR[x]);
        HRVwriteAdd = HRVwriteAdd + 1; 
    }
    RecordTrackHRV(trackSECTOR, trackAddress);
    HRVreccnt++;
    HRVreadAddX = HRVwriteAdd - 4;
}

void MakeRecordTEMP(unsigned char storeSECTOR, unsigned int trackSECTOR, unsigned int trackAddress, union uFLOAT VALUE)
{
    
    int x;
    for (x = 0; x < 4; x++)
    {
        writeDATA(storeSECTOR, TEMPwriteAdd, VALUE.floatCHAR[x]);
        TEMPwriteAdd = TEMPwriteAdd + 1; 
    }
    RecordTrackTEMP(trackSECTOR, trackAddress);
    TEMPreccnt++;
    TEMPreadAddX = TEMPwriteAdd - 4;
}

void MakeRecordGLUC(unsigned char storeSECTOR, unsigned int trackSECTOR, unsigned int trackAddress, union uFLOAT VALUE)
{
    
    int x;
    for (x = 0; x < 4; x++)
    {
        writeDATA(storeSECTOR, GLUCwriteAdd, VALUE.floatCHAR[x]);
        GLUCwriteAdd = GLUCwriteAdd + 1; 
    }
    RecordTrackGLUC(trackSECTOR, trackAddress);
    GLUCreccnt++;
    GLUCreadAddX = GLUCwriteAdd - 4;
}

void MaxFinder(unsigned char SECTOR, unsigned int ADDRESS1, volatile unsigned char maxRecord, volatile unsigned char* offsetMAX)
{
    *offsetMAX = 0;
    maxRecord = 0;
    readDATA(SECTOR, ADDRESS1);
    if (RXbits.CHARBITS == 0xFF)
    {
        return;
    }
    else
    {
        while(RXbits.CHARBITS != 0xFF)
        {
            maxRecord = RXbits.CHARBITS;
            *offsetMAX = *offsetMAX + 1;
            readDATA(SECTOR, ADDRESS1 + (*offsetMAX));
        }
    }   
}


