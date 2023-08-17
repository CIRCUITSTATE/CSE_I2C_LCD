
//=========================================================================//

#ifndef CSE_I2C_LCD_H
#define CSE_I2C_LCD_H

//=========================================================================//

#include <Wire.h>
#include "Print.h" 

//=========================================================================//

// Default values
#define LCD_DEF_ADDRESS           0x27U
#define LCD_DEF_COL_COUNT         16U
#define LCD_DEF_ROW_COUNT         2U

// Commands
#define LCD_CLEARDISPLAY          0x01
#define LCD_RETURNHOME            0x02
#define LCD_ENTRYMODESET          0x04
#define LCD_DISPLAYCONTROL        0x08
#define LCD_CURSORSHIFT           0x10
#define LCD_FUNCTIONSET           0x20
#define LCD_SETCGRAMADDR          0x40
#define LCD_SETDDRAMADDR          0x80

// Flags for display entry mode
#define LCD_ENTRYRIGHT            0x00
#define LCD_ENTRYLEFT             0x02
#define LCD_ENTRYSHIFTINCREMENT   0x01
#define LCD_ENTRYSHIFTDECREMENT   0x00

// Flags for display on/off control
#define LCD_DISPLAYON             0x04
#define LCD_DISPLAYOFF            0x00
#define LCD_CURSORON              0x02
#define LCD_CURSOROFF             0x00
#define LCD_BLINKON               0x01
#define LCD_BLINKOFF              0x00

// Flags for display/cursor shift
#define LCD_DISPLAYMOVE           0x08
#define LCD_CURSORMOVE            0x00
#define LCD_MOVERIGHT             0x04
#define LCD_MOVELEFT              0x00

// Flags for function set
#define LCD_8BITMODE              0x10
#define LCD_4BITMODE              0x00
#define LCD_2LINE                 0x08
#define LCD_1LINE                 0x00
#define LCD_5x10DOTS              0x04
#define LCD_5x8DOTS               0x00

// Flags for backlight control
#define LCD_BACKLIGHT_ON          0x08
#define LCD_BACKLIGHT_OFF         0x00

// LCD pin select bit masks
#define En 0b00000100  // Enable bit
#define Rw 0b00000010  // Read/Write bit
#define Rs 0b00000001  // Register select bit

//=========================================================================//

class CSE_I2C_LCD : public Print {
  public:
    uint8_t colCount; // Number of columns
    uint8_t rowCount; // Number of rows
    uint8_t lcdAddress; // I2C Address of the LCD

    // You may change the default values of the constructor arguments
    CSE_I2C_LCD (
      TwoWire *i2c,
      uint8_t address = LCD_DEF_ADDRESS,
      uint8_t cols = LCD_DEF_COL_COUNT,
      uint8_t rows = LCD_DEF_ROW_COUNT,
      uint8_t mode = LCD_4BITMODE,
      uint8_t dots = LCD_5x8DOTS
    );

    void begin (bool toInitWire = true); // Initializes the LCD
    void clear(); // Clears the LCD and sets the cursor to 0, 0
    void home();  // Sets the cursor to 0, 0
    void displayOff();
    void displayOn();
    void blinkOff();
    void blinkOn();
    void cursorOff();
    void cursorOn();
    void scrollDisplayLeft();
    void scrollDisplayRight();
    void leftToRight();
    void rightToLeft();
    void backlightOff();
    void backlightOn();
    void autoscrollOn();
    void autoscrollOff(); 
    void createChar (uint8_t, uint8_t []);
    void setCursor (uint8_t col, uint8_t row); 
    virtual size_t write (uint8_t);
    void writeCommand (uint8_t);

    // Compatibility function aliases
    void setBacklight (uint8_t value);

  private:
    uint8_t lcdConfig;  // LCD config flags
    uint8_t lcdControl; // LCD control state
    uint8_t lcdMode;  // The addressing mode of the LCD
    uint8_t lcdDotSize; // The dot configuration of the LCD
    uint8_t lcdBacklight; // LCD backlight control
    TwoWire *expanderI2C; // I2C bus object

    void writeLcd (uint8_t value, uint8_t mode);
    void writeLcdNibble (uint8_t value);
    void writeExpander (uint8_t data);
    void pulseEnable (uint8_t data);
};

#endif

//=========================================================================//
