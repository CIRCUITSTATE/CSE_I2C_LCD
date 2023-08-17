
//=========================================================================//

#include "CSE_I2C_LCD.h"
#include "inttypes.h"
#include "Arduino.h"

//=========================================================================//

inline size_t CSE_I2C_LCD:: write (uint8_t value) {
  writeLcd (value, Rs);
  return 1;
}

//=========================================================================//
/**
 * @brief
 * Instantiate a new LCD object. All values have defaults.
 * 
 * @param i2c I2C interface to use
 * @param address I2C address of the LCD
 * @param cols Number of columns on the LCD
 * @param rows Number of rows on the LCD
 * @param mode LCD mode (4 or 8 bit)
 * @param dotsize LCD dot size (5x8 or 5x10)
 */
CSE_I2C_LCD:: CSE_I2C_LCD (TwoWire *i2c, uint8_t address, uint8_t cols, uint8_t rows, uint8_t mode, uint8_t dotsize) {
  expanderI2C = i2c;
  lcdAddress = address;
  rowCount = rows;
  colCount = cols;
  lcdMode = mode;
  lcdDotSize = dotsize;
  lcdBacklight = LCD_BACKLIGHT_OFF; // Start with no backlight
}

//=========================================================================//
/**
 * @brief
 * Initialize the LCD. This must be called before any other LCD functions.
 * 
 */
void CSE_I2C_LCD:: begin (bool toInitWire) {
  if (toInitWire) {
    expanderI2C->begin();
  }

  if (rowCount > 1) {
    lcdConfig = lcdMode | LCD_2LINE | lcdDotSize;
  } else {
    lcdConfig = lcdMode | LCD_1LINE | lcdDotSize;
  }

  // for some 1 line displays you can select a 10 pixel high font
  // if ((lcdDotSize != 0) && (rowCount == 1)) {
  //   lcdConfig |= LCD_5x10DOTS;
  // }

  // SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
  // according to datasheet, we need at least 40ms after power rises above 2.7V
  // before sending commands. Arduino can turn on way befer 4.5V so we'll wait 50
  delay (50); 
  
  // Now we pull both RS and R/W low to begin commands
  writeExpander (lcdBacklight); // reset expanderand turn backlight off (Bit 8 =1)
  delay (1000);

  //put the LCD into 4 bit mode
  // this is according to the hitachi HD44780 datasheet
  // figure 24, pg 46
  
    // we start in 8bit mode, try to set 4 bit mode
   writeLcdNibble (0x03 << 4);
   delayMicroseconds (4500); // wait min 4.1ms
   
   // second try
   writeLcdNibble (0x03 << 4);
   delayMicroseconds (4500); // wait min 4.1ms
   
   // third go!
   writeLcdNibble (0x03 << 4); 
   delayMicroseconds (150);
   
   // finally, set to 4-bit interface
   writeLcdNibble (0x02 << 4); 

  // set # lines, font size, etc.
  writeCommand (LCD_FUNCTIONSET | lcdConfig);  
  
  // turn the display on with no cursor or blinking default
  lcdControl = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
  displayOn();
  
  // clear it off
  clear();
  
  // Initialize to default text direction (for roman languages)
  lcdMode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
  
  // set the entry mode
  writeCommand (LCD_ENTRYMODESET | lcdMode);
  
  home();
}

//=========================================================================//
/**
 * @brief
 * Clears the contents of the LCD and sets the cursor to 0, 0.
 * 
 */
void CSE_I2C_LCD:: clear() {
  writeCommand (LCD_CLEARDISPLAY);
  delayMicroseconds (2000);  // wait for the process to complete
}

//=========================================================================//
/**
 * @brief Sets the cursor to 0, 0.
 * 
 */
void CSE_I2C_LCD:: home() {
  writeCommand (LCD_RETURNHOME);
  delayMicroseconds (2000);  // wait for the process to complete
}

//=========================================================================//
/**
 * @brief Sets the cursor to arbitrary position.
 * 
 * @param col Column to set the cursor to.
 * @param row Row to set the cursor to.
 */
void CSE_I2C_LCD:: setCursor (uint8_t col, uint8_t row){
  int row_offsets[] = {0x00, 0x40, 0x14, 0x54};
  if (row > rowCount) {
    row = rowCount - 1;
  }
  writeCommand (LCD_SETDDRAMADDR | (col + row_offsets[row]));
}

//=========================================================================//
/**
 * @brief Turns off the display, but does not clear it.
 * 
 */
void CSE_I2C_LCD:: displayOff() {
  lcdControl &= (~LCD_DISPLAYON);
  writeCommand (LCD_DISPLAYCONTROL | lcdControl);
}

//=========================================================================//
/**
 * @brief Turns on the display.
 * 
 */
void CSE_I2C_LCD:: displayOn() {
  lcdControl |= LCD_DISPLAYON;
  writeCommand (LCD_DISPLAYCONTROL | lcdControl);
}

//=========================================================================//
/**
 * @brief Turns off the cursor.
 * 
 */
void CSE_I2C_LCD:: cursorOff() {
  lcdControl &= ~LCD_CURSORON;
  writeCommand (LCD_DISPLAYCONTROL | lcdControl);
}

//=========================================================================//
/**
 * @brief Turns on the cursor.
 * 
 */
void CSE_I2C_LCD:: cursorOn() {
  lcdControl |= LCD_CURSORON;
  writeCommand(LCD_DISPLAYCONTROL | lcdControl);
}

//=========================================================================//
/**
 * @brief Turns off the blinking cursor.
 * 
 */
void CSE_I2C_LCD:: blinkOff() {
  lcdControl &= (~LCD_BLINKON);
  writeCommand (LCD_DISPLAYCONTROL | lcdControl);
}

//=========================================================================//
/**
 * @brief Turns on the blinking cursor.
 * 
 */
void CSE_I2C_LCD:: blinkOn() {
  lcdControl |= LCD_BLINKON;
  writeCommand (LCD_DISPLAYCONTROL | lcdControl);
}

//=========================================================================//
/**
 * @brief This command scrolls the display left without changing the
 * RAM contents.
 * 
 */
void CSE_I2C_LCD:: scrollDisplayLeft() {
  writeCommand (LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}

//=========================================================================//
/**
 * @brief This command scrolls the display right without changing the
 * RAM contents.
 * 
 */
void CSE_I2C_LCD:: scrollDisplayRight() {
  writeCommand (LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

//=========================================================================//
/**
 * @brief For printing text from left to right.
 * 
 */
void CSE_I2C_LCD:: leftToRight() {
  lcdMode |= LCD_ENTRYLEFT;
  writeCommand (LCD_ENTRYMODESET | lcdMode);
}

//=========================================================================//
/**
 * @brief For printing text from right to left.
 * 
 */
void CSE_I2C_LCD:: rightToLeft() {
  lcdMode &= (~LCD_ENTRYLEFT);
  writeCommand (LCD_ENTRYMODESET | lcdMode);
}

//=========================================================================//
/**
 * @brief
 * 
 */
void CSE_I2C_LCD:: autoscrollOn() {
  lcdMode |= LCD_ENTRYSHIFTINCREMENT;
  writeCommand (LCD_ENTRYMODESET | lcdMode);
}

//=========================================================================//
/**
 * @brief
 * 
 */
void CSE_I2C_LCD:: autoscrollOff() {
  lcdMode &= ~LCD_ENTRYSHIFTINCREMENT;
  writeCommand (LCD_ENTRYMODESET | lcdMode);
}

//=========================================================================//
/**
 * @brief Allows us to fill the first 8 CGRAM locations with custom characters.
 * 
 * @param location The location of the character in CGRAM.
 * @param charmap The array of characters.
 */
void CSE_I2C_LCD:: createChar (uint8_t location, uint8_t charmap[]) {
  location &= 0x7; // we only have 8 locations 0-7
  writeCommand (LCD_SETCGRAMADDR | (location << 3));
  for (int i = 0; i < 8; i++) {
    write (charmap[i]);
  }
}

//=========================================================================//
/**
 * @brief Turns off the backlight. This only works if the I2C module is
 * configured to control the LCD backlight.
 * 
 */
void CSE_I2C_LCD:: backlightOff() {
  lcdBacklight = LCD_BACKLIGHT_OFF;
  writeExpander (0);
}

//=========================================================================//
/**
 * @brief Turns on the backlight. This only works if the I2C module is
 * configured to control the LCD backlight.
 * 
 */
void CSE_I2C_LCD:: backlightOn() {
  lcdBacklight = LCD_BACKLIGHT_ON;
  writeExpander (0);
}

//=========================================================================//
/**
 * @brief Writes a single command to the LCD.
 * 
 * @param value The command to write.
 */
inline void CSE_I2C_LCD:: writeCommand (uint8_t value) {
  writeLcd (value, 0);
}

//=========================================================================//
/**
 * @brief Writes either a command or data to the LCD with specified mode.
 * The value will be sent as two nibbles.
 * 
 * @param value The value to right.
 * @param mode The LCD read/write mode.
 */
void CSE_I2C_LCD:: writeLcd (uint8_t value, uint8_t mode) {
  uint8_t highnib = value & 0xF0;
  uint8_t lownib = (value << 4) & 0xF0;
  writeLcdNibble ((highnib) | mode);
  writeLcdNibble ((lownib) | mode); 
}

//=========================================================================//
/**
 * @brief Writes a single nibble to the LCD.
 * 
 * @param value The value to write. Only the first 4 MSBs will be used.
 * 
 */
void CSE_I2C_LCD:: writeLcdNibble (uint8_t value) {
  writeExpander (value);
  pulseEnable (value);
}

//=========================================================================//
/**
 * @brief Writes a single byte to the I2C expander. The LCD backlight state
 * is also written to the expander.
 * 
 * @param data The data to send.
 */
void CSE_I2C_LCD:: writeExpander (uint8_t data){                                        
  expanderI2C->beginTransmission (lcdAddress);
  expanderI2C->write ((int)(data) | lcdBacklight);
  expanderI2C->endTransmission();   
}

//=========================================================================//
/**
 * @brief Sends a pulse through the EN pin of the LCD and writes data.
 * 
 * @param data The data to write.
 */
void CSE_I2C_LCD:: pulseEnable (uint8_t data){
  writeExpander (data | En);  // En high
  delayMicroseconds (1);   // enable pulse must be >450ns
  
  writeExpander (data & ~En); // En low
  delayMicroseconds (50);    // commands need > 37us to settle
}

//=========================================================================//
/**
 * @brief Turn the backlight on or off.
 * 
 * @param value The backlight state. Can be HIGH or LOW.
 */
void CSE_I2C_LCD:: setBacklight (uint8_t value) {
  if (value == 0) {
    backlightOff();
  } else {
    backlightOn();
  }
}

//=========================================================================//

