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
 * If you want to navigate this file easier use the shortcuts (netbeans default).
 * "Ctrl + Shift + +" -> Expand all code blocks
 * "Ctrl + Shift + -" -> Collapse all code blocks
 */

#include "LCD.h"
#include <math.h>

#define LCD_BUSY_DELAY           60                    //Minimum delay required by short commands[uS]
#define LCD_BUSY_DELAY_EXTENDED (2300-LCD_BUSY_DELAY)  //Extra delay required by the longer commands[uS]

#define LCD_CMD_FUNCTION_SET     0x28  //Function Set(4-bit, 2 line, 5x8 dots)
#define LCD_CMD_DISPLAY_CONTROL  0x0C  //Display On/Off Control(Display on, cursor off, blinking off)
#define LCD_CMD_ENTRY_MODE       0x06  //Entry Mode Set(Increment, shift off)
#define LCD_CMD_CLEAR            0x01  //Clear Display
#define LCD_CMD_RETURN_HOME      0x02  //Return Home(or shift reset)
#define LCD_CMD_SHIFT_LEFT       0x18  //Shift Left
#define LCD_CMD_SHIFT_RIGHT      0x1C  //Shift Right
#define LCD_CMD_CURSOR_INCREMENT 0x14  //Increment Cursor
#define LCD_CMD_CURSOR_DECREMENT 0x10  //Decrement Cursor
#define LCD_CMD_CGRAM_SET        0x40  //Set CGRAM Address

#define LCD_CMD_DDRAM_LINE_1 (0x80-1)   //DDRAM address for line-1(-1 from row starting from 1 instead of 0)
#define LCD_CMD_DDRAM_LINE_2 (0xC0-1)   //DDRAM address for line-2(-1 from row starting from 1 instead of 0)
#define LCD_CMD_DDRAM_LINE_3 (0x80+19)  //DDRAM address for line-3(+19 from 3rd line being the continuation of line-1)
#define LCD_CMD_DDRAM_LINE_4 (0xC0+19)  //DDRAM address for line-4(+19 from 4th line being the continuation of line-2)


/* Initialize the global variables */
static uint8_t LCD_display_control = LCD_CMD_DISPLAY_CONTROL;
static uint8_t LCD_entry_mode      = LCD_CMD_ENTRY_MODE;


/**
 * Pulses the clock line of LCD\n
 * <STRONG>(!!!Meant for internal use only!!!)</STRONG>
 */
void LCD_pulse_clock(){
  
  LCD_EN_LAT = 1;              //Set the "EN" pin
  __delay_us(LCD_BUSY_DELAY);  //Give the required delay
  LCD_EN_LAT = 0;              //Clear the "EN" pin
}

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

/**
 * Sends an 8-bit data (in 4-bit mode)\n
 * <STRONG>(!!!Meant for internal use only!!!)</STRONG>s
 * @param data: 8-bit data to be sent
 */
void LCD_write_data(char data){

    LCD_RS_LAT = 1; //Enter data mode
    
    /*---------- Load higher order 4-bits ----------*/
    LCD_D7_LAT = (data & 0b10000000) >> 7;
    LCD_D6_LAT = (data & 0b01000000) >> 6;
    LCD_D5_LAT = (data & 0b00100000) >> 5;
    LCD_D4_LAT = (data & 0b00010000) >> 4;
    
    /*---------- Send a clock pulse ----------*/
    LCD_pulse_clock();
    
    /*---------- Load lower order 4-bits ----------*/
    LCD_D7_LAT = (data & 0b00001000) >> 3;
    LCD_D6_LAT = (data & 0b00000100) >> 2;
    LCD_D5_LAT = (data & 0b00000010) >> 1;
    LCD_D4_LAT = (data & 0b00000001)     ;
    
    /*---------- Send a clock pulse ----------*/
    LCD_pulse_clock();
}

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

/**
 * Sets the cursor location
 * @param row: Row position(1-4)
 * @param column: Column position(1-20)
 */
void LCD_cursor_set(uint8_t row, uint8_t column){

   /* Offset the column address with the corresponding row */
    if(row == 1) column += LCD_CMD_DDRAM_LINE_1;
    if(row == 2) column += LCD_CMD_DDRAM_LINE_2;
    if(row == 3) column += LCD_CMD_DDRAM_LINE_3;
    if(row == 4) column += LCD_CMD_DDRAM_LINE_4;
   
   /* Set the cursor location with the new address */
   LCD_write_command(column);
}

/**
 * Increments the current cursor position by 1
 */
void LCD_cursor_increment(){
   
    /* Send the cursor increment command */
    LCD_write_command(LCD_CMD_CURSOR_INCREMENT);
}

/**
 * Decrements the current cursor position by 1
 */
void LCD_cursor_decrement(){
   
    /* Send the cursor decrement command */
    LCD_write_command(LCD_CMD_CURSOR_DECREMENT);
}

/**
 * Enables/disables character display on screen\n
 * (The LCD will still work and you can still do write operations,
 * only the characters will/won't be displayed)
 * @param state: 1 to enable, 0 to disable
 */
void LCD_configure_lcd_display(uint8_t state){
    
    /* Change the gobal display control variable accordingly */
    if(state){
        LCD_display_control |= 0b00000100;
    }
    else{
        LCD_display_control &= 0b11111011;
    }
    
    /* Send the new display control command */
    LCD_write_command(LCD_display_control);
}

/**
 * Turns on/off the line displayed below characters on cursor location  
 * @param state: 1 to turn on, 0 to turn off
 */
void LCD_configure_cursor_display(uint8_t state){
    
    /* Change the gobal display control variable accordingly */
    if(state){
        LCD_display_control |= 0b00000010;
    }
    else{
        LCD_display_control &= 0b11111101;
    }
    
    /* Send the new display control command */
    LCD_write_command(LCD_display_control);
}

/** 
 * Turns on/off the character blinking on cursor location 
 * @param state: 1 to turn on, 0 to turn off
 */
void LCD_configure_cursor_blink(uint8_t state){
    
    /* Change the gobal display control variable accordingly */
    if(state){
        LCD_display_control |= 0b00000001;
    }
    else{
        LCD_display_control &= 0b11111110;
    }
    
    /* Send the new display control command */
    LCD_write_command(LCD_display_control);
}

/**
 * Clears the whole screen and resets cursor to 1,1
 */
void LCD_clear(){
    
    /* Send the clear command(automatically sets cursor to 1,1) */
    LCD_write_command(LCD_CMD_CLEAR);
    
    /* Give the extended delay needed by the longer command */
    __delay_us(LCD_BUSY_DELAY_EXTENDED);
    
    /* Re-send the entry mode, otherwise clear command resets the id to increment */
    LCD_write_command(LCD_entry_mode);
}

/**
 * Resets the shifting and resets cursor to 1,1
 */
void LCD_shift_reset(){
    
    /* Send the return home command(automatically sets cursor to 1,1) */
    LCD_write_command(LCD_CMD_RETURN_HOME);
    
    /* Give the extended delay needed by the longer command */
    __delay_us(LCD_BUSY_DELAY_EXTENDED);
}

/**
 * Shifts the whole screen left by 1\n
 * (Shifting the screen also shifts the cursor locations!)\n
 * <STRONG>Note: Characters off-screen will be displayed on a different line!</STRONG>
 */
void LCD_shift_left(){
    
    /* Send the shift left command */
    LCD_write_command(LCD_CMD_SHIFT_LEFT);
}

/**
 * Shifts the whole screen right by 1\n
 * (Shifting the screen also shifts the cursor locations!)\n
 * <STRONG>Note: Characters off-screen will be displayed on a different line!</STRONG>
 */
void LCD_shift_right(){
    
    /* Send the shift right command */
    LCD_write_command(LCD_CMD_SHIFT_RIGHT);
}

/**
 * Configures if the cursor increments or decrements on a write command
 * @param state: 1 to increment, 0 to decrement
 */
void LCD_configure_entry_id(uint8_t state){
    
    /* Change the gobal entry mode variable accordingly */
    if(state){
        LCD_entry_mode |= 0b00000010;
    }
    else{
        LCD_entry_mode &= 0b11111101;
    }
    
    /* Send the new entry mode command */
    LCD_write_command(LCD_entry_mode);
}

/**
 * Enables/disables screen shift on a write command\n
 * (Screen will be shifted left when cursor is incrementing, shifted right otherwise)
 * @param state: 1 to enable, 0 to disable
 */
void LCD_configure_entry_shift(uint8_t state){
    
    /* Change the gobal entry mode variable accordingly */
    if(state){
        LCD_entry_mode |= 0b00000001;
    }
    else{
        LCD_entry_mode &= 0b11111110;
    }
    
    /* Send the new entry mode command */
    LCD_write_command(LCD_entry_mode);
}

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

/**
 * Writes a string to the current cursor location\n
 * <STRONG>!!!The string has to be null terminated otherwise this function will fail or loop forever!!!</STRONG> 
 * @param *string: Character array that contains the string(max. 255 characters)
 */
void LCD_write_string(char *string){
   
    /* Write all characters one by one until you reach "NULL" */
    for(uint8_t i=0 ; string[i] != '\0' ; i++){
        LCD_write_data(string[i]);
    }
}

/**
 * Writes an ASCII character to the current cursor location
 * @param character: Character to be written
 */
void LCD_write_char(char character){
   
    /* Write the character */
    LCD_write_data(character);
}

/**
 * Writes an integer variable to the current cursor location(max. 32-bit)\n
 * (Negative numbers will be treated as positive!)
 * @param variable: Variable to be written
 * @param back_offset: If the offset is bigger than the
 * total digit count, the difference amount of empty spaces
 * will be put before writing the variable\n
 * (Allows for right to left writing while keeping the first
 * digit stationary)\n
 * (Cursor will still point to the right side of the variable)\n
 * (Leave as 0 if you want the variable to be written as is)
 */
void LCD_write_variable(int32_t variable, uint8_t left_offset){
    
    int32_t temp_variable;  //Temporary variable to work on
    uint8_t digits = 0;     //Variable to hold the number of total digits
    
    /*----- Turn negative numbers into positive -----*/
    if(variable < 0) variable *= -1;
    
    /*---------- Calculate the total digits ----------*/
    temp_variable = variable;          //Copy the variable
    while(1){                          //...
        digits++;                      //Increment the digit(do first since even 0 will have 1 digit)
        if(temp_variable < 10) break;  //If the number has 1 digit left, break
        temp_variable /= 10;           //Shift the number to the right
    }
    
    /*---------- Give the offset before writing the variable ----------*/
    while(1){
        if(left_offset <= digits) break;  //If the offset is equal to digits *or less*, break
        LCD_write_data(' ');              //Put an empty space
        left_offset--;                    //Decrement the offset
    }
    
    /*---------- Write the variable on screen ----------*/
    while(1){
        temp_variable = variable;                    //Copy/re-copy the variable
        for(uint8_t i = 1 ; i < digits ; i++){       //Shift the temporary variable *digits* times
            temp_variable /= 10;                     //...
        }                                            //...
        LCD_write_data( (temp_variable%10) + '0' );  //Write the digit(with character 0 as offset)
        digits --;                                   //Decrement the *digits*(to move onto next)
        if(digits == 0) break;                       //If *digits* reaches 0, break
    }
}

/**
 * Writes a float variable to the current cursor location\n
 * (Negative numbers will be treated as positive!)
 * @param variable: Variable to be written
 * @param back_offset: If the offset is bigger than the total
 * digit count(including integer digits, float digits and 1 
 * more for the '.' character), the difference amount of 
 * empty spaces will be put before writing the variable\n
 * (Allows for right to left writing while keeping the first
 * digit stationary)\n
 * @param f_digit: How many digits of the fractional part to
 * be written\n
 * (Too high value will cause the variable to overflow!)
 */
void LCD_write_float(float variable, uint8_t left_offset, uint8_t f_digit){
    
    /*----- Calculate the left offset for the integer part.(-1 is for the '.' character) -----*/
    /*----- (Making sure that left offset doesn't wrap back to 255.) -----*/
    if( left_offset >= (f_digit+1) ){        
        left_offset = (left_offset-f_digit) - 1;
    }
    else{
        left_offset = 0;
    }
    
    /*----- Turn negative numbers into positive -----*/
    if(variable < 0) variable *= -1;

    /*----- Seperate the integer and fractional parts using "modff" from "math.h" -----*/
    float integer;                           //Define a variable for the integer part
    float fractional;                        //Define a variable for the fractional part
    fractional = modff(variable, &integer);  //Seperate the integer and fractional parts

    /*----- Write the integer part -----*/
    LCD_write_variable( (int32_t)integer, left_offset );  //Write the integer part with given offset    
    LCD_write_char('.');                                  //Place a dot before the fractional part
    
    /*----- Write the fractional part -----*/
    for(uint8_t i=0 ; i < f_digit ; i++){                           //Limit the fractional digits
        fractional *= 10;                                           //Shift fraction to left
        LCD_write_data( (uint8_t)((int32_t)fractional%10 + '0') );  //Write the digit(with character 0 as offset)
    }
}
