#include <p18f452.h>
#include <pwm.h>
#include <timers.h>
#include <delays.h>
#include "Speaker_Functions.h"

void speaker_config(void)
{
    TRISCbits.TRISC2 = 0;
    SetDCPWM1(5);
    OpenTimer2(TIMER_INT_OFF & T2_PS_1_16 & T2_POST_1_1);
}
void speaker_alert(void)
{
//        speaker_config();
        OpenPWM1(127);
        Delay1KTCYx(255);
        ClosePWM1();
        Delay1KTCYx(255);
        
        OpenPWM1(127);
        Delay1KTCYx(255);
        ClosePWM1();
        Delay1KTCYx(255);
        
        OpenPWM1(127);
        Delay1KTCYx(255);
        ClosePWM1();
        Delay1KTCYx(255);
               
}

void speaker_startup(void)
{
        speaker_config();
        OpenPWM1(200);
        Delay1KTCYx(255);
        ClosePWM1();
         
        OpenPWM1(150);
        Delay1KTCYx(255);
        ClosePWM1();
         
        OpenPWM1(100);
        Delay1KTCYx(255);
        ClosePWM1();
         
         OpenPWM1(50);
        Delay1KTCYx(255);
        ClosePWM1();
         
//        OpenPWM1(50);
//        Delay1KTCYx(255);
//        ClosePWM1();
        Delay1KTCYx(255);
       
}


