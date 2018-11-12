#include <p18f452.h>
#include "xlcd.h"
#include <delays.h>
#include <timers.h>
#include <portb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pwm.h>
#include "adc.h"
#include "capture.h"
#include <math.h>
#include "HeartRate_Functions.h"
#include "Speaker_Functions.h"
#include "Keypad_Functions.h"
#include "ow.h"


#pragma config WDT = OFF
#pragma config OSC = HS
#pragma config LVP = OFF
#define XTAL_FREQ 4000000

extern int T0_finished;
extern int Beatcnt;
extern int ccpcnt;
extern int ccp1;
extern int ccp2;
extern int ccp3;
extern int ccpdif;
extern int NN50;
extern int NNcnt;
extern int live;


void low_isr(void); 
void high_isr(void);

#pragma code low_vector=0x18 
void interrupt_at_low_vector(void) 
{  
    _asm GOTO low_isr _endasm
} 
#pragma code /* return to the default code section */

#pragma interruptlow low_isr 
void low_isr (void) 
{  
   if(INTCONbits.TMR0IF == 1)
    {
        INTCONbits.TMR0IF = 0;
        T0_finished = 1;
    }
}

#pragma code high_vector=0x08 
void interrupt_at_high_vector(void) 
{ 
    _asm GOTO high_isr _endasm
} 
#pragma code /* return to the default code section */
#pragma interrupt high_isr 
void high_isr (void) 
{  
    if(INTCON3bits.INT1IF)
    {
         
            INTCON3bits.INT1IF = 0;
            if (live==0) keypad_lcd();
            if (live==1)
            {
                live=3;
                while(BusyXLCD());                                                      // Wait if LCD busy
                SetDDRamAddr(0x00);                                                     // Set Display data ram address to 0
                while(BusyXLCD());
                WriteCmdXLCD(0x01);
                while(BusyXLCD());
                SetDDRamAddr(0x42);                                                     
                while(BusyXLCD());
                putrsXLCD("EXITING LIVE");

            }
            if (live!=3)default_display();
        
        
       
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
     
}



void main()
{
    keypad_config();
    startup_display();
    live=0;
    while (1)
    {   
        
        default_display();
        Delay1KTCYx(255);
        
    }
}
