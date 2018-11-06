/* 
 * File:   floaties_main.c
 * Author: Luther Jn. Baptiste
 *
 * Created on October 15, 2018, 12:47 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include <delays.h>
#include <p18cxxx.h>
#include "SST39S.h"



volatile union uFloat HRV;
volatile union uFloat glucose;
volatile union uFloat HR;
volatile union uFloat Temp;


volatile union uFloat HRV_disp;
volatile union uFloat glucose_disp;
volatile union uFloat HR_disp;
volatile union uFloat Temp_disp;

union uCHAR RXbits;
union uCHAR TXbitsADDR;
union uCHAR TXbitsDATA;

int clearAddressCount;
rom unsigned int softIDADD[3] = {0x5555,0x2AAA,0x5555};
rom char softIDDAT[3] = {0xAA,0x55,0x90};
rom unsigned int softIDExitADD[3] ={0x5555,0x2AAA,0x5555};
rom unsigned char softIDExitDAT[3] ={0xAA,0x55,0xF0};
rom unsigned int SectorEraseADD[5] = {0x5555,0x2AAA,0x5555,0x5555,0x2AAA};
rom char SectorEraseDAT[6] = {0xAA, 0x55, 0x80, 0xAA,0x55, 0x30};
rom unsigned int writeADD[3] = {0x5555,0x2AAA,0x5555};
rom char writeDAT[3] = {0xAA,0x55,0xA0};
rom unsigned int eraseChipADD[6] ={0x5555,0x2AAA,0x5555,0x5555,0x2AAA,0x5555};
rom char eraseChipDAT[6] = {0xAA,0x55,0x80,0xAA,0x55,0x10};



void clockDataOUT(void)
{
    DATA_CLOCK = 0;
    Nop();
    DATA_CLOCK = 1;
}

void clockAddressOUT(void)
{
    ADDRESS_CLOCK = 0;
    Nop();
    ADDRESS_CLOCK = 1;
}



void dataOUT_init()
{
   DATAOE_TRIS = 0; //inhibit output
   DATAOE = 1;
   S01_TRIS = 0;// configure for right shift
   S01 = 0;
   DATAPIN_OUT_TRIS = 0;// configure output pin
   DATA_CLOCK_TRIS = 0; // configure data clock pin
       
}



void addressOUT_init()
{
   ADDRESSPIN_TRIS = 0;// configure output pin
   ADDRESS_CLOCK_TRIS = 0; // configure data clock pin
       
}
void ram_init()
{
    OE_TRIS = 0;
    WE_TRIS = 0;
    WE = 1;
    OE = 1;   
}

void strobeWE()
{
    TRISDbits.RD2 = 0;
    _asm 
    BSF PORTD , 2, 0
    NOP
    BCF PORTD , 2, 0
    NOP
    BSF PORTD , 2, 0
    _endasm
}

void clearOE()
{
    OE = 0;
}
void setOE()
{
    OE = 1;
}

void clearAddress()
{
    ADDRESSPIN_OUT = 0;
    for(clearAddressCount = 0; clearAddressCount < 20; clearAddressCount++)
    {
        clockAddressOUT();
    }
    
}

void dataOUT()
{
    DATAOE = 0;// enable output
}


void pushAddressCMD(char MSB3, unsigned int restOfBits)
{
  union uADDRESS addrU;
  addrU.adINT = restOfBits;

  addressOUT_init();
  //clearAddress();
  TXbitsADDR.CHARBITS = MSB3;
  ADDRESSPIN_OUT = TXbitsADDR.B2;
  clockAddressOUT();
  ADDRESSPIN_OUT = TXbitsADDR.B1;
  clockAddressOUT();
  ADDRESSPIN_OUT = TXbitsADDR.B0;
  clockAddressOUT();
  
  TXbitsADDR.CHARBITS = addrU.adCHAR[1];
  ADDRESSPIN_OUT = TXbitsADDR.B7;
  clockAddressOUT();
  ADDRESSPIN_OUT = TXbitsADDR.B6;
  clockAddressOUT();
  ADDRESSPIN_OUT = TXbitsADDR.B5;
  clockAddressOUT();
  ADDRESSPIN_OUT = TXbitsADDR.B4;
  clockAddressOUT();
  ADDRESSPIN_OUT = TXbitsADDR.B3;
  clockAddressOUT();
  ADDRESSPIN_OUT = TXbitsADDR.B2;
  clockAddressOUT();
  ADDRESSPIN_OUT = TXbitsADDR.B1;
  clockAddressOUT();
  ADDRESSPIN_OUT = TXbitsADDR.B0;
  clockAddressOUT();
  
  TXbitsADDR.CHARBITS = addrU.adCHAR[0];
  ADDRESSPIN_OUT = TXbitsADDR.B7;
  clockAddressOUT();
  ADDRESSPIN_OUT = TXbitsADDR.B6;
  clockAddressOUT();
  ADDRESSPIN_OUT = TXbitsADDR.B5;
  clockAddressOUT();
  ADDRESSPIN_OUT = TXbitsADDR.B4;
  clockAddressOUT();
  ADDRESSPIN_OUT = TXbitsADDR.B3;
  clockAddressOUT();
  ADDRESSPIN_OUT = TXbitsADDR.B2;
  clockAddressOUT();
  ADDRESSPIN_OUT = TXbitsADDR.B1;
  clockAddressOUT();
  ADDRESSPIN_OUT = TXbitsADDR.B0;
  clockAddressOUT();    
}



void pushAddress_SECTOR(char sector, unsigned int ADDRESS)
{
  union uADDRESS addrU;
  addrU.adINT = ADDRESS;

  addressOUT_init();
  //clearAddress();
  TXbitsADDR.CHARBITS = sector;
  clockAddressOUT();
  ADDRESSPIN_OUT = TXbitsADDR.B6;
  clockAddressOUT();
  ADDRESSPIN_OUT = TXbitsADDR.B5;
  clockAddressOUT();
  ADDRESSPIN_OUT = TXbitsADDR.B4;
  clockAddressOUT();
  ADDRESSPIN_OUT = TXbitsADDR.B3;
  clockAddressOUT();
  ADDRESSPIN_OUT = TXbitsADDR.B2;
  clockAddressOUT();
  ADDRESSPIN_OUT = TXbitsADDR.B1;
  clockAddressOUT();
  ADDRESSPIN_OUT = TXbitsADDR.B0;
  clockAddressOUT();
  
  TXbitsADDR.CHARBITS = addrU.adCHAR[1];
  ADDRESSPIN_OUT = TXbitsADDR.B3;
  clockAddressOUT();
  ADDRESSPIN_OUT = TXbitsADDR.B2;
  clockAddressOUT();
  ADDRESSPIN_OUT = TXbitsADDR.B1;
  clockAddressOUT();
  ADDRESSPIN_OUT = TXbitsADDR.B0;
  clockAddressOUT();
  
  TXbitsADDR.CHARBITS = addrU.adCHAR[0];
  ADDRESSPIN_OUT = TXbitsADDR.B7;
  clockAddressOUT();
  ADDRESSPIN_OUT = TXbitsADDR.B6;
  clockAddressOUT();
  ADDRESSPIN_OUT = TXbitsADDR.B5;
  clockAddressOUT();
  ADDRESSPIN_OUT = TXbitsADDR.B4;
  clockAddressOUT();
  ADDRESSPIN_OUT = TXbitsADDR.B3;
  clockAddressOUT();
  ADDRESSPIN_OUT = TXbitsADDR.B2;
  clockAddressOUT();
  ADDRESSPIN_OUT = TXbitsADDR.B1;
  clockAddressOUT();
  ADDRESSPIN_OUT = TXbitsADDR.B0;
  clockAddressOUT();    
}

void dataIN_init()
{
    DATAOE = 1; //inhibit output
    DATAOE_TRIS = 0; //inhibit output
    DATAPIN_IN_TRIS = 1;
    S01 = 1;
    clockDataOUT(); // parallel load
    S01 = 0;
}

void byteProgramDly()
{
  Delay10TCYx(2); // 2us delay
}

void sectorEraseDly()
{
  Delay1KTCYx(25); // 25ms delay
}

void chipEraseDly()
{
     Delay1KTCYx(100);
}


void pullData()
{ 
    dataIN_init();
    RXbits.B7 = DATAPIN_IN;
    clockDataOUT();
    RXbits.B6 = DATAPIN_IN;
    clockDataOUT();
    RXbits.B5 = DATAPIN_IN;
    clockDataOUT();
    RXbits.B4 = DATAPIN_IN;
    clockDataOUT();
    RXbits.B3 = DATAPIN_IN;
    clockDataOUT();
    RXbits.B2 = DATAPIN_IN;
    clockDataOUT();
    RXbits.B1 = DATAPIN_IN;
    clockDataOUT();
    RXbits.B0 = DATAPIN_IN;
    /*undo dataIN_init here?*/
}

void pushData(char data)
{
    dataOUT_init();
    TXbitsDATA.CHARBITS = data;
    DATAPIN_OUT = TXbitsDATA.B7;
    clockDataOUT();
    DATAPIN_OUT = TXbitsDATA.B6;
    clockDataOUT();
    DATAPIN_OUT = TXbitsDATA.B5;
    clockDataOUT();
    DATAPIN_OUT = TXbitsDATA.B4;
    clockDataOUT();
    DATAPIN_OUT = TXbitsDATA.B3;
    clockDataOUT();
    DATAPIN_OUT = TXbitsDATA.B2;
    clockDataOUT();
    DATAPIN_OUT = TXbitsDATA.B1;
    clockDataOUT();
    DATAPIN_OUT = TXbitsDATA.B0;
    clockDataOUT(); 
    dataOUT();
}

void writeDATA(char sector, unsigned int ADDRESS, char data)
{
  pushAddressCMD(0x00,writeADD[0]);
  pushData(writeDAT[0]);
  strobeWE();
  pushAddressCMD(0x00,writeADD[1]);
  pushData(writeDAT[1]);
  strobeWE();
  pushAddressCMD(0x00,writeADD[2]);
  pushData(writeDAT[2]);
  strobeWE();
  pushAddress_SECTOR(sector, ADDRESS);
  pushData(data);
  strobeWE();
  byteProgramDly();
}

void readDATA(char sector, unsigned int ADDRESS)
{
  pushAddress_SECTOR(sector, ADDRESS);
  clearOE();
  pullData();
  setOE();
}

    
volatile union uFloat HRV;
volatile union uFloat glucose;
volatile union uFloat HR;
volatile union uFloat Temp;


volatile union uFloat HRV_disp;
volatile union uFloat glucose_disp;
volatile union uFloat HR_disp;
volatile union uFloat Temp_disp;

char fives = 0x55;
char tens = 0xAA;

void chipErase()
{
    int x;
    for(x = 0;x < 6; x++){
        pushAddressCMD(0x00,eraseChipADD[x]);
        pushData(eraseChipDAT[x]);
////   Delay10KTCYx(50);
    strobeWE();
//  
//        
    }
    chipEraseDly();
}
void sectorErase(char SECTOR)
{
    int x;
    for (x = 0; x < 5; x++){
   pushAddressCMD(0x00,SectorEraseADD[x]);
   pushData(SectorEraseDAT[x]);
    strobeWE();
    }
   pushAddress_SECTOR(SECTOR,0x0000);
   pushData(SectorEraseDAT[5]);
    strobeWE();
    sectorEraseDly();
}

char softwareID()
{
    int x;
    for(x = 0;x < 3; x++){
        pushAddressCMD(0x00,softIDADD[x]);
        pushData(softIDDAT[x]);
        strobeWE();       
    }
    pushAddressCMD(0x00,0x0001);
    byteProgramDly();
    clearOE();
    pullData();
    setOE();
    
    return RXbits.CHARBITS;
}

void softwareIDExit()
{
    int x;
    for(x = 0;x < 3; x++){
        pushAddressCMD(0x00,softIDExitADD[x]);
        pushData(softIDExitDAT[x]);
        strobeWE();       
    }
    byteProgramDly();
}








