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
 * Adds a custom character to the "CGRAM" and resets cursor to 1,1\n
 * (Max. 8 custom characters can be added/used at a time, limited
 * by the driver chip itself!)\n
 * (Character font needs to be 5x8!)
 * @param *pattern: Binary pattern of the character\n
 * (Can be generated by googling "LCD custom character
 * generator".)
 * @param location: CGRAM address to put the character(0-7)
 */
void LCD_add_character(char *pattern, uint8_t address){
    
    /* Set the CGRAM address with the passed offset(address).
     * Multiply the offset by 8(shift left by 3), since each
     * "CGRAM location" has 8 addresses for 8 rows of character.*/  
    LCD_write_command( LCD_CMD_CGRAM_SET + (uint8_t)(address<<3) );

    /* Send the pattern */
    for(uint8_t i=0 ; i < 8 ; i++) {
        LCD_write_data(pattern[i]);
    }
    
    /* Set cursor to 1,1 to point the cursor back from "CGRAM" to "DDRAM" */
    LCD_cursor_set(1, 1);
}