#include <p18f452.h>
#include <timers.h>
#include <portb.h>
#include <delays.h>
#include "xlcd3.h"
#include "LCD_Functions.h"
#include "Keypad_Functions.h"
#include "HeartRate_Functions.h"
#include "Temperature_Functions.h"
#include "Speaker_Functions.h"
#include "SST39S_v1.h"
#include "meman_v0_4.h"


extern int keypad_input;
extern int scrolled_disp;
extern int state;
extern int interval;
extern int num_measures;
extern int interval_counter;
extern int reading_counter;
extern float store_temp;
extern int store_frac;
int store;
char temp_store[8];
//char num_disp[8];


void keypad_config(void)
{
INTCON3bits.INT1IF = 0;
INTCONbits.INT0IE = 1;
INTCON2bits.INTEDG1 = 1;

INTCON2bits.RBPU = 0;
INTCON3bits.INT1IP = 1;
INTCON3bits.INT1IE = 1;

INTCONbits.RBIE = 0;
TRISBbits.TRISB1 = 1;
TRISCbits.TRISC7 = 1;
TRISCbits.TRISC6 = 1;
TRISCbits.TRISC5 = 1;
TRISCbits.TRISC4 = 1;



RCONbits.IPEN = 1;
RCONbits.TO=1;
INTCONbits.GIEH = 1;
INTCONbits.PEIE = 1;
}



void keypad_lcd(void)
{     
    {
        keypad_input = PORTC & 0xF0;
        if (keypad_input == 0xF0)
        {
            if (state == 0)
            {
            while( BusyXLCD() );
            WriteCmdXLCD(0b00000001);
//            while( BusyXLCD() );
//            SetDDRamAddr(0x43);
//            putrsXLCD( "D Pressed");
//            Delay10KTCYx(255);
            scrolled_disp = 1;
            }
            
            if (state==5)
            {
               pageScrollLite(SCROLL_DOWN); 
            }

        }
        if (keypad_input == 0xE0)
        {
            if (state==0)
            {
            while( BusyXLCD() );
            WriteCmdXLCD(0b00000001);
            while( BusyXLCD() );
            SetDDRamAddr(0x42);
            putrsXLCD( "INVALID KEY");
            Delay10KTCYx(255);
 //           speaker_alert();
            }

        }
        if (keypad_input == 0xD0)
        {
            if (state==0)
            {
            //itoa(store_frac, temp_store);
            //store = store_temp * 100;
            //itoa(store, temp_store);
            //itoa(interval,interval_disp);
            while( BusyXLCD() );
            WriteCmdXLCD(0b00000001);
            while( BusyXLCD() );
            SetDDRamAddr(0x42);
            putrsXLCD( "INVALID KEY");
//            SetDDRamAddr(0x13);
//            putsXLCD(temp_store);
            Delay10KTCYx(255);
            }
            
            

        }
        if (keypad_input == 0xC0)
        {
            
            while( BusyXLCD() );
            WriteCmdXLCD(0b00000001);
            while( BusyXLCD() );
            if (state != 4)
            {
                SetDDRamAddr(0x43);
                putrsXLCD( "MAIN MENU");
            }
            if (state == 4)
            {
                SetDDRamAddr(0x42);
                putrsXLCD( "EXITING LIVE");
            }
            state = 0;
            Delay1KTCYx(255);
            
            

            
        }
        if (keypad_input == 0xB0)
        {
            if (state == 9)
            {
            while( BusyXLCD() );
            WriteCmdXLCD(0b00000001);
            while( BusyXLCD() );
            SetDDRamAddr(0x40);
            putrsXLCD( "CLEARING RECORDS");
            Delay10KTCYx(255);
            clear_everything();
            state = 0;
            }

        }
        if (keypad_input == 0xA0)
        {
            state=9;
            while( BusyXLCD() );
            WriteCmdXLCD(0b00000001);
            while( BusyXLCD() );
            SetDDRamAddr(0x02);
            putrsXLCD( "CLEAR RECORDS");
            clear_records();

        }
        if (keypad_input == 0x90)
        {
            state=8;
            while( BusyXLCD() );
            WriteCmdXLCD(0b00000001);
            while( BusyXLCD() );
            SetDDRamAddr(0x02);
            putrsXLCD( "# OF READINGS");
            storage_number();

        }
        if (keypad_input == 0x80)
        {
            state = 7;
            while( BusyXLCD() );
            WriteCmdXLCD(0b00000001);
            while( BusyXLCD() );
            SetDDRamAddr(0x00);
            putrsXLCD( "STORAGE INTERVAL");
            storage_interval();

        }
        if (keypad_input == 0x70)
        {
            if (state==0)
            {
           // itoa(num_measures,num_disp);
            while( BusyXLCD() );
            WriteCmdXLCD(0b00000001);
            while( BusyXLCD() );
            SetDDRamAddr(0x42);
            putrsXLCD( "INVALID KEY");
            while( BusyXLCD() );
//            SetDDRamAddr(0x10);
//            putsXLCD( num_disp);
            Delay10KTCYx(255);
            }

           

        }
        if (keypad_input == 0x60)
        {
            if (state == 0)
            {
            state = 6;
            while( BusyXLCD() );
            WriteCmdXLCD(0b00000001);
            while( BusyXLCD() );
            SetDDRamAddr(0x04);
            putrsXLCD( "SETTINGS");
            settings_display();
            }

        }
        
        
        if (keypad_input == 0x50)
        {
            if (state==0)
            {
            if (HRreccnt == 0)
            {
                while( BusyXLCD() );
                WriteCmdXLCD(0b00000001);
                while( BusyXLCD() );
                SetDDRamAddr(0x43);
                putrsXLCD( "NO RECORDS");
                Delay10KTCYx(255);
            }
            
            if (HRreccnt != 0)
            {
            state = 5;
            while( BusyXLCD() );
            WriteCmdXLCD(0b00000001);
            while( BusyXLCD() );
            SetDDRamAddr(0x43);
            putrsXLCD( "RECORDS");
            Delay10KTCYx(255);
            display_records();
            }
            }

        }
        
    
        if (keypad_input == 0x40)
        {
            if (state==0)
            {
            state = 4;
            while( BusyXLCD() );
            WriteCmdXLCD(0b00000001);
            while( BusyXLCD() );
            SetDDRamAddr(0x03);
            putrsXLCD( "LIVE MODE");
            live_mode();
            if (state==4) instructions();
            interval_counter = interval;
            reading_counter = num_measures;
            HR_Measure();
            }
        }
        
        
        if (keypad_input == 0x30)
        {
            if (state == 0)
            {
            while( BusyXLCD() );
            WriteCmdXLCD(0b00000001);
//            while( BusyXLCD() );
//            SetDDRamAddr(0x43);
//            putrsXLCD( "A Pressed");
//            Delay10KTCYx(255);
            scrolled_disp = 0;
            }
            
            if (state==5)
            {
                pageScrollLite(SCROLL_UP);
            }
 
            

        }
        if (keypad_input == 0x20)
        {
            if (state==0)
            {
            state = 3;
            while( BusyXLCD() );
            WriteCmdXLCD(0b00000001);
            while( BusyXLCD() );
            SetDDRamAddr(0x02);
            putrsXLCD( "TEMPERATURE");
 //           Delay10KTCYx(255);
//            while( BusyXLCD() );
//            WriteCmdXLCD(0b00000001);
            temp();
            state=0;
            }
            
             if (state == 7)
            {
               interval = 2;
               state = 6; 
            } 
            
            if (state == 8)
            {
               num_measures = 10;
               state = 6; 
            }  
            
            //state = 0;

        }
        
        if (keypad_input == 0x10)
        {
            if (state==0)
            {
            state = 2;
            while( BusyXLCD() );
            WriteCmdXLCD(0b00000001);
            while( BusyXLCD() );
            SetDDRamAddr(0x43);
            putrsXLCD( "GLUCOSE");
             while( BusyXLCD() );
            SetDDRamAddr(0x11);
            putrsXLCD( "(Unavailable)");
            Delay10KTCYx(255);
            state=0;
            }
            
             if (state == 7)
            {
               interval = 1;
               state = 6; 
            }  
            
            if (state == 8)
            {
               num_measures = 5;
               state = 6; 
            }  
            //state = 0;
        }


        if (keypad_input == 0x00)
        {
            if (state == 0)
            {
            state = 1;
            while( BusyXLCD() );
            WriteCmdXLCD(0b00000001);
            while( BusyXLCD() );
            SetDDRamAddr(0x43);
            putrsXLCD( "HEART RATE");
            Delay10KTCYx(255);
            while( BusyXLCD() );
            WriteCmdXLCD(0b00000001);
            instructions();
            HR_Measure();
            state=0;
            }
            
            
            if (state == 7)
            {
               interval = 0;
               state = 6; 
            }   
            
            if (state == 8)
            {
               num_measures = 1;
               state = 6; 
            }   
            //state = 0;

        }
    }
   
    }
