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


/* 
 * This library is made for communicating with LCDs (Liquid Crystal Display)
 * with PIC microcontrollers.
 *
 * The library is written to communicate with LCDs that are controlled by
 * the HD44780U chip from hitachi. It's written for the XC8 compiler, and is
 * intended to be used with MPLAB X IDE.
 *
 * All library functions are blocking, so there is no need for any peripherals.
 * 
 * Works with the common 4x20 and 2x16 LCD displays (should work with any LCD
 * that uses HD44780U).
 * 
 * For a comprehensive tutorial on how to use the library visit
 * "https://youtube.com/Microesque".
 * Actual video: https://www.youtube.com/watch?v=pR942IY54oo
 */

#ifndef LCD_H
    #define	LCD_H


    /**************************************************************************/
    /*************** Update the definitions below accordingly *****************/
    /**************************************************************************/
    
    #define LCD_RS_TRIS TRISAbits.TRISA0  //TRIS bit of the pin connected to "RS"
    #define LCD_EN_TRIS TRISAbits.TRISA1  //TRIS bit of the pin connected to "EN"
    #define LCD_D4_TRIS TRISAbits.TRISA2  //TRIS bit of the pin connected to "D4"
    #define LCD_D5_TRIS TRISAbits.TRISA3  //TRIS bit of the pin connected to "D5"
    #define LCD_D6_TRIS TRISAbits.TRISA4  //TRIS bit of the pin connected to "D6"
    #define LCD_D7_TRIS TRISAbits.TRISA5  //TRIS bit of the pin connected to "D7"

    #define LCD_RS_LAT LATAbits.LATA0  //LAT bit of the pin connected to "RS"
    #define LCD_EN_LAT LATAbits.LATA1  //LAT bit of the pin connected to "EN"
    #define LCD_D4_LAT LATAbits.LATA2  //LAT bit of the pin connected to "D4"
    #define LCD_D5_LAT LATAbits.LATA3  //LAT bit of the pin connected to "D5"
    #define LCD_D6_LAT LATAbits.LATA4  //LAT bit of the pin connected to "D6"
    #define LCD_D7_LAT LATAbits.LATA5  //LAT bit of the pin connected to "D7"

    #define _XTAL_FREQ 64000000  //Frequency of the sytem clock

    /**************************************************************************/
    /*************************** Necessary includes ***************************/
    /**************************************************************************/

    #include <xc.h>
    #include <stdint.h>
    
    /**************************************************************************/
    /*************************** Function prototypes **************************/
    /**************************************************************************/

    void LCD_init(void);
    void LCD_cursor_set(uint8_t, uint8_t);
    void LCD_cursor_increment(void);
    void LCD_cursor_decrement(void);
    void LCD_configure_lcd_display(uint8_t);
    void LCD_configure_cursor_display(uint8_t);
    void LCD_configure_cursor_blink(uint8_t);
    void LCD_clear(void);
    void LCD_shift_reset(void);
    void LCD_shift_left(void);
    void LCD_shift_right(void);    
    void LCD_configure_entry_id(uint8_t);
    void LCD_configure_entry_shift(uint8_t);
    void LCD_add_character(char*, uint8_t);
    
    void LCD_write_string(char*);
    void LCD_write_char(char);
    void LCD_write_variable(int32_t, uint8_t);
    void LCD_write_float(float, uint8_t, uint8_t);
    
    
#endif	/* LCD_H */