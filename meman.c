#include "meman.h"
#include "SST39S_v1.h"

float tempDisp[4] = {0.0, 0.0, 0.0, 0.0};
float glucDisp[4] = {0.0, 0.0, 0.0, 0.0};
float hrvDisp[4] = {0.0, 0.0, 0.0, 0.0};
int hrDisp[4] = {0, 0, 0, 0};

/*Q0 The system should tolerate a power/brown-out reset without corrupting information display.
 
Q7.FRreq8: The management module must to track the current write address for the next value to be written in a FlashRAM sector
Q8.FRreq1: The Storage Management Module must coordinate retrieval of data during scroll operations 
Q0.FRreq1: On startup the Storage Management Module will read user defined sectors to restore the previous state of storage processes.
Q0.FRreq2: The Storage Management Module must record the user defined maximum number of records on the flash RAM.
Q0.FRreq3:  keep track of the number of entries for each quantity (HRV,HR, Glucose, Temperature) 

 */

//Q0.FRreq1 The Storage Management Module must record the user defined maximum number of records on the flash RAM.
void RecordTrackInit(unsigned char SECTOR, unsigned int ADDRESS1, unsigned char* offsetTRK, unsigned int *stateVAR, unsigned char *reccnt, 
        unsigned char* offsetMax, unsigned int* writeAddress, unsigned char TypeSize)
{
    *offsetTRK = 0;
    *writeAddress = 0;
    *reccnt = 0;
    readDATA(SECTOR,ADDRESS1);
    if(RXbits.CHARBITS == 0xFF)
    {
        return;
    }
    else
    {
        while((RXbits.CHARBITS != 0xFF) && (*reccnt < 254))
        {
            if(RXbits.CHARBITS == WRT1)
            {
                *reccnt = *reccnt + 1;
                *stateVAR = W1;
            }
            else if(RXbits.CHARBITS == WRT2)
            {
                *reccnt = *reccnt + 2;
                *stateVAR = W2;
            }
            else if(RXbits.CHARBITS == WRT3)
            {
                *reccnt = *reccnt + 3;
                *stateVAR = W3;
            }
            else if(RXbits.CHARBITS == WRT4)
            {
                *reccnt = *reccnt + 4;
                *stateVAR = W4;
            }
            else if(RXbits.CHARBITS == WRT5)
            {
                *reccnt = *reccnt + 5;
                *stateVAR = W5;
            }
            else if(RXbits.CHARBITS == WRT6)
            {
                *reccnt = *reccnt + 6;
                *stateVAR = W6;
            }
            else if(RXbits.CHARBITS == WRT7)
            {
                *reccnt = *reccnt + 7;
                *stateVAR = W7;
            }
            else if(RXbits.CHARBITS == WRT8)
            {
                *reccnt = *reccnt + 8;
                *stateVAR = W0;
            }
            else
            {
                *reccnt = 0;
                *offsetTRK = 0;
                *offsetMax = 0;
                *stateVAR = W0;
                sectorErase(SECTOR);// erase the sector because something has been w
                break; // error
            }
            *offsetTRK = *offsetTRK + 0x01;
            readDATA(SECTOR,(ADDRESS1+(*offsetTRK)));
        }
        *offsetTRK = *offsetTRK - 0x01;
        *writeAddress = *writeAddress + ((*reccnt) * TypeSize); 
    }
}

void MaxFinder(unsigned char SECTOR, unsigned int ADDRESS1, unsigned char maxRecord,unsigned char* offsetMAX)
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


void RecordTrack(unsigned char trackSECTOR, unsigned int trackAddress, unsigned char* offsetTRK, unsigned int *stateVAR)
{
    int temp;
    temp = *stateVAR;
    switch (temp){
        case W0:
            writeDATA(trackSECTOR, (trackAddress + *offsetTRK), WRT1);
            *stateVAR = W1;
            break;
            
        case W1:
            writeDATA(trackSECTOR, (trackAddress + *offsetTRK), WRT2);
            *stateVAR = W2;
            break;           

        case W2:
            writeDATA(trackSECTOR, (trackAddress + *offsetTRK), WRT3);
            *stateVAR = W3;
            break; 

        case W3:
            writeDATA(trackSECTOR, (trackAddress + *offsetTRK), WRT4);
            *stateVAR = W4;
            break;
        
        case W4:
            writeDATA(trackSECTOR, (trackAddress + *offsetTRK), WRT5);
            *stateVAR = W5;
            break;             
            
        case W5:
            writeDATA(trackSECTOR, (trackAddress + *offsetTRK), WRT6);
            *stateVAR = W6;
            break; 
            
        case W6:
            writeDATA(trackSECTOR, (trackAddress + *offsetTRK), WRT7);
            *stateVAR = W7;
            break;  

        case W7:
            writeDATA(trackSECTOR, (trackAddress + *offsetTRK), WRT8);
            *stateVAR = W0;
            *offsetTRK = *offsetTRK + 1;
            break;              
            
        default:
            return; // should return some kind of exception
    }
    
}



void MakeRecordFloat(unsigned char storeSECTOR, unsigned int* writeAddress, unsigned int* scrollLimit,unsigned char trackSECTOR, unsigned int trackAddress, 
        unsigned char* offsetTRK, unsigned int *stateVAR,unsigned char *reccnt, union uFLOAT VALUE)
{
    int x;
    for (x = 0; x < 4; x++)
    {
        writeDATA(storeSECTOR, *writeAddress, VALUE.floatCHAR[x]);
        *writeAddress = *writeAddress + 1; 
    }
    RecordTrack(trackSECTOR, trackAddress, offsetTRK, stateVAR);
    *reccnt = *reccnt + 1;
    *scrollLimit = *writeAddress - 4;
}