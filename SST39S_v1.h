#ifndef __SST39S_h
#define __SST39S_h

#include <delays.h>
#include <p18cxxx.h>


#define DATAPIN_OUT PORTEbits.RE1
#define DATAPIN_OUT_TRIS TRISEbits.RE1
#define DATAPIN_IN PORTCbits.RC3
#define DATAPIN_IN_TRIS TRISCbits.RC3
#define DATAOE_TRIS TRISEbits.RE2
#define DATAOE PORTEbits.RE2
#define DATA_CLOCK PORTDbits.RD0
#define DATA_CLOCK_TRIS TRISDbits.RD0
#define ADDRESS_CLOCK PORTDbits.RD1
#define ADDRESS_CLOCK_TRIS TRISDbits.RD1

#define ADDRESSPIN_TRIS TRISEbits.RE0
#define ADDRESSPIN_OUT PORTEbits.RE0

#define S01 PORTAbits.RA4
#define S01_TRIS TRISAbits.RA4

#define OE_TRIS TRISDbits.RD3
#define OE PORTDbits.RD3
#define WE_TRIS TRISDbits.RD2
#define WE PORTDbits.RD2

#define SECTOR0 0x00
#define SECTOR1 0x01
#define SECTOR2 0x02
#define SECTOR3 0x03
#define SECTOR4 0x04
#define SECTOR5 0x05
#define SECTOR6 0x06
#define SECTOR7 0x07


void clockDataOUT(void);
void clockAddressOUT(void);
void dataOUT_init(void);
void dataIN_init(void);
void addressOUT_init(void);   
void ram_init(void);
void strobeWE(void);
void clearOE(void);
void clearAddress(void);
void dataOUT(void);
void pushAddress(char , char *, int);
void pushAddressCMD(char, unsigned int);
void writeDATA(char, unsigned int, char );
void readDATA(char, unsigned int);
void sectorErase(char);
void chipErase(void);
void setOE(void);
void pullData(void);
void pushData(char);
void byteProgramDly(void);
void sectorEraseDly(void);
void chipEraseDly(void);
char softwareID(void);
void softwareIDExit(void);

volatile union uFloat{
    float floatVal;
    char floatCHAR[4];
};

volatile union uFloat{
    float floatVal;
    char floatCHAR[4];
};

volatile union uADDRESS{
    char adCHAR[2];
    unsigned int adINT;
};


volatile union uCHAR{
    struct{
        unsigned B0 : 1;
        unsigned B1 : 1;
        unsigned B2 : 1;
        unsigned B3 : 1;
        unsigned B4 : 1;
        unsigned B5 : 1;
        unsigned B6 : 1;
        unsigned B7 : 1;
    };
    unsigned char CHARBITS;
};
extern union uCHAR RXbits;
extern union uCHAR TXbitsADDR;
extern union uCHAR TXbitsDATA;
#endif