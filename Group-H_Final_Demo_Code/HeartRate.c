#include <p18f452.h>
#include "xlcd3.h"
#include <delays.h>
#include <timers.h>
#include <portb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "HeartRate_Functions.h"
#include "Temperature_Functions.h"
#include "Speaker_Functions.h"
#include "LCD_Functions.h"
#include "meman_v0_4.h"


////////////////////////////////////////////////////////////////////////////
char BPMMessage [8];
char HRVMessage [8];
char PulseMessage [8];

int T0_finished = 0;
int Beatcnt = 0;
int ccpcnt = 0;
int ccp1 = 0;
int ccp2 = 0;
int ccp3 = 0;
int ccpdif = 0;
int NN50 = 0;
int NNcnt  = 0;
int BPM;
unsigned int HR_V;
extern int state;
extern float store_temp;

extern int interval;
extern int num_measures;
int interval_counter;
int reading_counter;


void PORTB_con(void)
{
    OpenRB0INT(PORTB_CHANGE_INT_ON & RISING_EDGE_INT & PORTB_PULLUPS_OFF);
    //INTCON2bits.RBIP = 0;
}

void PORTB_end(void)
{
    CloseRB0INT();
}

void Heart_rate(void)
{
    

    HR_V = 0;
    Beatcnt = 0;
    BPM = 0;
    ccpcnt = 1;
    ccp1 = 0;
    ccp2 = 0;
    ccpdif = 0;
    NN50 = 0;
    NNcnt  = 0;
    

    T0_finished = 0;
    
    if (state == 1)
    {
    while(BusyXLCD());                                                      // Wait if LCD busy
    SetDDRamAddr(0x00);                                                     // Set Display data ram address to 0
    while(BusyXLCD());
    WriteCmdXLCD(0x01);
    while(BusyXLCD()); 
    
    putrsXLCD("Measuring...");
    while(BusyXLCD());
    }
    
    INTCONbits.GIEH = 1;        //enable high interrupts
    INTCONbits.GIEL = 1;        //enable low interrupts
    RCONbits.IPEN = 1;          // turn priority levels on
    INTCON2bits.TMR0IP = 0;        //timer 0 priority set to low
    
    INTCONbits.TMR0IF = 0;        //clears interrupt falg
    INTCONbits.TMR0IE = 1;        //enables tmr0 interrupt
    OpenTimer0(TIMER_INT_ON & T0_16BIT & T0_SOURCE_INT & T0_PS_1_256 & T0_EDGE_RISE );
    WriteTimer0(26472);         //writes value to overflow in 10 ms
    
    while(1)
    {
        if(T0_finished == 1)
        {
            INTCONbits.GIEH = 0;        //disable high interrupts
            INTCONbits.GIEL = 0;        //disable low interrupts
            CloseTimer0();
            
            BPM = Beatcnt * 6 ;
            itoa(BPM, BPMMessage);
            
            NNcnt = Beatcnt-1;
            HR_V = (NN50*100)/(NNcnt);
            itoa(HR_V,HRVMessage);
            break;
        }
    }  
}


void Write_HR_data(void)
{
    
    if (state == 1)
    {
    while(BusyXLCD());                                                      // Wait if LCD busy
    SetDDRamAddr(0x00);                                                     // Set Display data ram address to 0
    while(BusyXLCD());
    WriteCmdXLCD(0x01);
    while(BusyXLCD());  
    
    while(BusyXLCD());                                                      // Wait if LCD busy
    SetDDRamAddr(0x00);                                                     // Set Display data ram address to 0
    while(BusyXLCD());
    putrsXLCD("Completed");
    while(BusyXLCD());

    while(BusyXLCD());                                                      // Wait if LCD busy
    SetDDRamAddr(0x40);                                                     // Set Display data ram address to 0
    while(BusyXLCD());
    putrsXLCD("HR :");

    while(BusyXLCD());                                                      // Wait if LCD busy
    SetDDRamAddr(0x44);
    while(BusyXLCD());  
    putsXLCD(BPMMessage);
    while(BusyXLCD());
    if ( (BPM > 100) || (BPM < 60) ) speaker_alert();
    
    while(BusyXLCD());                                                      // Wait if LCD busy
    SetDDRamAddr(0x10);                                                     // Set Display data ram address to 0
    while(BusyXLCD());
    putrsXLCD("HRV:");
    while(BusyXLCD()); 

    while(BusyXLCD());                                                      // Wait if LCD busy
    SetDDRamAddr(0x14);
    while(BusyXLCD());  
    putsXLCD(HRVMessage);
    while(BusyXLCD());
    
    while(BusyXLCD());                                                      // Wait if LCD busy
    SetDDRamAddr(0x17);                                                     // Set Display data ram address to 0
    while(BusyXLCD());
    putrsXLCD("%");
    while(BusyXLCD()); 
    if (HR_V > 25) speaker_alert();
    
    }
    
    
    
    if (state == 4)
    {
    while(BusyXLCD());                                                      // Wait if LCD busy
    SetDDRamAddr(0x00);                                                     // Set Display data ram address to 0
    while(BusyXLCD());
    WriteCmdXLCD(0x01);
    while(BusyXLCD());  

    while(BusyXLCD());                                                      // Wait if LCD busy
    SetDDRamAddr(0x00);                                                     // Set Display data ram address to 0
    while(BusyXLCD());
    putrsXLCD("HR   :");

    while(BusyXLCD());                                                      // Wait if LCD busy
    SetDDRamAddr(0x06);
    while(BusyXLCD());  
    putsXLCD(BPMMessage);
    while(BusyXLCD()); 
    if ( (BPM > 100) || (BPM < 60) ) speaker_alert();
    
    while(BusyXLCD());                                                      // Wait if LCD busy
    SetDDRamAddr(0x40);                                                     // Set Display data ram address to 0
    while(BusyXLCD());
    putrsXLCD("HRV  :");
    while(BusyXLCD()); 

    while(BusyXLCD());                                                      // Wait if LCD busy
    SetDDRamAddr(0x46);
    while(BusyXLCD());  
    putsXLCD(HRVMessage);
    while(BusyXLCD());
    
    while(BusyXLCD());                                                      // Wait if LCD busy
    SetDDRamAddr(0x49);                                                     // Set Display data ram address to 0
    while(BusyXLCD());
    putrsXLCD("%");
    while(BusyXLCD());
    if (HR_V > 25) speaker_alert();
    
    while(BusyXLCD());                                                      // Wait if LCD busy
    SetDDRamAddr(0x50);                                                     // Set Display data ram address to 0
    while(BusyXLCD());
    putrsXLCD("GLUC : ---");
    while(BusyXLCD());
    }
   
}

void load_flash (void)
{
    HR.intVAL = BPM;
    MakeRecordHR(HR_STORAGE_SECTOR,HR_RCCNT,HR);
    HRV.floatVAL = HR_V;
    MakeRecordHRV(HRV_STORAGE_SECTOR,HRV_RCCNT,HRV);
    TEMP.floatVAL = store_temp;
    MakeRecordTEMP(TEMP_STORAGE_SECTOR,TEMP_RCCNT,TEMP);
    GLUC.floatVAL = 1.0;
    MakeRecordGLUC(GLUC_STORAGE_SECTOR,GLUC_RCCNT,GLUC);
}

void HR_Measure (void)
{
//    init_XLCD();
//    PORTB_con();
//    TRISDbits.RD3 = 0;
//    PORTDbits.RD3 = 1;
    
    if (state == 1)
    {
        Heart_rate();
        Write_HR_data();
        Delay10KTCYx(250);
        Delay10KTCYx(250);
    }
        
    while (state == 4)
        {
        
            Heart_rate();
            Write_HR_data();
            temp();
            if ((interval_counter == 1) && (reading_counter!= 0))
            {
                load_flash();
                reading_counter--;
                interval_counter = interval;
            }
            if (interval_counter == 2) interval_counter--;
        }
       
}



