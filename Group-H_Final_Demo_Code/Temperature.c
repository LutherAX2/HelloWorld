#include <p18f452.h>
#include <delays.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "xlcd3.h"
#include "ow.h"
#include "Temperature_Functions.h"

extern int state;
unsigned char TempMSB;
unsigned char TempLSB;
unsigned int MSBTemporary = 0;
unsigned int LSBTemporary =0;
unsigned int TempInt = 0;
unsigned char degree = 0xDF;
float fracFloat = 0.0000;
int sign = 0;
int fracInt =0;
float store_fracint;
int x = 0;
float store_temp;
char Temperature[20];
int store_frac = 0;
 
 void temp(void)
{    
    // TRISBbits.RB3=0;                //set RC7 as output to transistor
  
        ow_reset();             //reset 1822P
        
        ow_write_byte(0xCC);    // Skip ROM Check
        ow_write_byte(0x44);    //Temperature conversion 
        
        // PORTBbits.RB3 = 1;      //Strong pullup to provide current that parasitic capacitance can't provide
        
         for(x = 1; x<=8;x++)
         {  
           //800ms (750ms is recommended conversion time))
           Delay1KTCYx(100);
        }
        
        // PORTBbits.RB3 = 0;      //Turn off strong pullup
        
        //Read Dallas 1822P
        ow_reset();                 //reset device
        ow_write_byte(0xCC);        //skip ROM check
        ow_write_byte(0xBE);        //Send read scratchpad on 1822P
        
        TempLSB = ow_read_byte();   //Read first byte, LS and store in TempLSB
        TempMSB = ow_read_byte();   //Read first byte, MS and store in TempMSB
        
        //Really don't care about the other bytes, stop reading and prep data for LCD
        
        //Acquire Integer
        LSBTemporary = TempLSB >> 4; 
        MSBTemporary = TempMSB << 4;
        TempInt = MSBTemporary | LSBTemporary;
        
        //Acquire Fraction
        if(TempLSB & 0x01){fracFloat += 0.0625;}
        if(TempLSB & 0x02){fracFloat += 0.125;}
        if(TempLSB & 0x04){fracFloat += 0.25;}
        if(TempLSB & 0x08){fracFloat += 0.5;}
        fracInt =fracFloat*1000;
        
        fracInt = fracInt/100;
        store_frac = fracInt;
        

        store_temp = TempInt + ((float)fracInt)/10;
        
        
        //fracInt = fracInt/100;
        //Sign check
        sign = ((TempMSB >> 3 )& 0x3F);
        if(sign == 0){
            //sprintf(Temperature,"+%d.%03d%cC",TempInt,fracInt,degree);
            sprintf(Temperature,"+%d.%01d%cC",TempInt,fracInt,degree);
            
            TempInt = 0;
            fracInt= 0;
            fracFloat =0.0;
        }
        else{
            //sprintf(Temperature,"-%d.%03d%cC",TempInt,fracInt);
            sprintf(Temperature,"-%d.%01d%cC",TempInt,fracInt);
            
            TempInt = 0;
            fracInt= 0;
            fracFloat =0.0;

        }
        
        
        
        if (state == 3)
        {
        while(BusyXLCD());
        SetDDRamAddr(0x13); //Set cursor to top line
        while(BusyXLCD());
        //Delay1KTCYx(110); //Give time for LCD to refresh, writing to it too quickly causes flicker issues
        //while(BusyXLCD());
        putsXLCD(Temperature);//Write top line value
        sprintf(Temperature,"                   ");
        Delay10KTCYx(255); 
        }
        
        
        if (state == 4)
        {
        while(BusyXLCD());
        SetDDRamAddr(0x10); //Set cursor to top line
        while(BusyXLCD());
        putrsXLCD("TEMP :");
        //Delay1KTCYx(110); //Give time for LCD to refresh, writing to it too quickly causes flicker issues
        //while(BusyXLCD());
        while(BusyXLCD());
        SetDDRamAddr(0x16); //Set cursor to top line
        putsXLCD(Temperature);//Write top line value
        sprintf(Temperature,"                   ");
        Delay10KTCYx(255); 
        
        }
}




