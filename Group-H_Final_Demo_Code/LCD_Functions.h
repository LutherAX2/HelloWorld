/* 
 * File:   LCD_Functions.h
 * Author: zafee
 *
 * Created on November 25, 2018, 4:04 PM
 */

#ifndef LCD_FUNCTIONS_H
#define	LCD_FUNCTIONS_H

void DelayFor18TCY (void);
void DelayPORXLCD (void);
void DelayXLCD (void);
void init_XLCD(void);
void startup_display(void);
void display_options (void);
void settings_display(void);
void storage_interval(void);
void storage_number (void);
void clear_records (void);
void live_mode(void);
void instructions (void);
void display_records (void);
void RefreshLCD(void);
void clear_everything(void);

#endif	/* LCD_FUNCTIONS_H */

