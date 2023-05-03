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


/**
 * Sends an 8-bit command (in 4-bit mode)\n
 * <STRONG>(!!!Meant for internal use only!!!)</STRONG>
 * @param command: 8-bit command to be sent
 */
void LCD_write_command(char command){
    
    LCD_RS_LAT = 0; //Enter command mode
    
    /*---------- Load higher order 4-bits ----------*/
    LCD_D7_LAT = (command & 0b10000000) >> 7;
    LCD_D6_LAT = (command & 0b01000000) >> 6;
    LCD_D5_LAT = (command & 0b00100000) >> 5;
    LCD_D4_LAT = (command & 0b00010000) >> 4;
    
    /*---------- Send a clock pulse ----------*/
    LCD_pulse_clock();
    
    /*---------- Load lower order 4-bits ----------*/
    LCD_D7_LAT = (command & 0b00001000) >> 3;
    LCD_D6_LAT = (command & 0b00000100) >> 2;
    LCD_D5_LAT = (command & 0b00000010) >> 1;
    LCD_D4_LAT = (command & 0b00000001)     ;
    
    /*---------- Send a clock pulse ----------*/
    LCD_pulse_clock();
}