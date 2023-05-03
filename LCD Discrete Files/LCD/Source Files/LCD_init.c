/*
 * MIT License
 * 
 * Copyright (c) 2022 Ahmet Burak Irmak (https://youtube.com/Microesque)
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */


#include "LCD.h"


/* Initialize the global variables */
uint8_t LCD_display_control = LCD_CMD_DISPLAY_CONTROL;
uint8_t LCD_entry_mode      = LCD_CMD_ENTRY_MODE;


/**
 * Initializes the LCD\n
 * <u>LCD configured as:</u>\n
 * - Function set: 4-bit, 2-line, 5x8 dots\n
 * - Display: On\n
 * - Cursor display: Off\n
 * - Cursor blink: Off\n
 * - Cursor on write: Increment, no shift\n
 */
void LCD_init(){
    
    //---------- Pin configurations ----------
    LCD_RS_TRIS = 0; LCD_RS_LAT = 0;
    LCD_EN_TRIS = 0; LCD_EN_LAT = 0;
    LCD_D4_TRIS = 0; LCD_D4_LAT = 0;
    LCD_D5_TRIS = 0; LCD_D5_LAT = 0;
    LCD_D6_TRIS = 0; LCD_D6_LAT = 0;
    LCD_D7_TRIS = 0; LCD_D7_LAT = 0;
    
    //******************************************************************//
    //*** Sequence for "Initializing 4-bit interface" from datasheet ***//
    //******************************************************************//
    
    //---------- Wait for more than 40ms ----------
    __delay_ms(60);
    
    //---------- Load 0b0011 in command mode ----------
    LCD_D7_LAT = 0; LCD_D6_LAT = 0; LCD_D5_LAT = 1; LCD_D4_LAT = 1;
    
    //---------- Send a clock pulse ----------
    LCD_pulse_clock();
    
    //---------- Wait for more than 4.1ms ----------
    __delay_ms(6);
    
    //---------- Send a clock pulse ----------
    LCD_pulse_clock();
    
    //---------- Wait for more than 100us ----------
    __delay_us(150);
    
    //---------- Send a clock pulse ----------
    LCD_pulse_clock();
    
    //---------- Load 0b0010 in command mode(4-bit mode start) ----------
    LCD_D7_LAT = 0; LCD_D6_LAT = 0; LCD_D5_LAT = 1; LCD_D4_LAT = 0;
    
    //---------- Send a clock pulse ----------
    LCD_pulse_clock();
    
    //---------- Configure important settings ----------
    LCD_write_command(LCD_CMD_FUNCTION_SET);  //Function Set(4-bit, 2 line, 5x8 dots)
    LCD_write_command(LCD_display_control);   //Display On/Off Control(Display on, cursor off, blinking off)
    LCD_write_command(LCD_entry_mode);        //Entry Mode Set(Increment, shift off)
    LCD_clear();                              //Clear the display
 }