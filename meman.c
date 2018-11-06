#include "meman.h"
#include "SST39S_v1.h"

float tempDisp[4] = {0.0, 0.0, 0.0, 0.0};
float glucDisp[4] = {0.0, 0.0, 0.0, 0.0};
float hrvDisp[4] = {0.0, 0.0, 0.0, 0.0};
int hrDisp[4] = {0.0, 0.0, 0.0, 0.0};

void RecordTrackInit(unsigned char SECTOR, unsigned int ADDRESS1, unsigned char* offsetTRK, trkSTATE stateVAR, unsigned int *reccnt, 
        unsigned char* offsetMax, unsigned char* offsetStore, unsigned char TypeSize)
{
    *offsetTRK = 0;
    *offsetStore = 0;
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
                stateVAR = W1;
            }
            else if(RXbits.CHARBITS == WRT2)
            {
                *reccnt = *reccnt + 2;
                stateVAR = W2;
            }
            else if(RXbits.CHARBITS == WRT3)
            {
                *reccnt = *reccnt + 3;
                stateVAR = W3;
            }
            else if(RXbits.CHARBITS == WRT4)
            {
                *reccnt = *reccnt + 4;
                stateVAR = W4;
            }
            else if(RXbits.CHARBITS == WRT5)
            {
                *reccnt = *reccnt + 5;
                stateVAR = W5;
            }
            else if(RXbits.CHARBITS == WRT6)
            {
                *reccnt = *reccnt + 6;
                stateVAR = W6;
            }
            else if(RXbits.CHARBITS == WRT7)
            {
                *reccnt = *reccnt + 7;
                stateVAR = W7;
            }
            else if(RXbits.CHARBITS == WRT8)
            {
                *reccnt = *reccnt + 8;
                stateVAR = W0;
            }
            else
            {
                *reccnt = 0;
                *offsetTRK = 0;
                *offsetMax = 0;
                stateVAR = W0;
                sectorErase(SECTOR);// erase the sector because something has been w
                break; // error
            }
            *offset = *offsetTRK + 1;
            readDATA(SECTOR,(ADDRESS1+(*offsetTRK)));
        }
        *offsetTRK = *offsetTRK - 1;
        *offsetStore = *offsetStore + (reccnt*TypeSize); 
    }
}