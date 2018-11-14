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

int globalval;
int globalval2;



void main (void)
{

    TRISB = 0x00;
    ram_init();
    sectorErase(0x34);
	writeDATA(0x34,0x466,0x79);
    readDATA(0x34,0x466);
    PORTB = RXbits.CHARBITS;
    while(1);
}








