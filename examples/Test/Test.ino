
//=========================================================================//

#include <Wire.h>
#include <CSE-I2C-LCD.h>

//=========================================================================//

#define   PIN_I2C0_SCL                13
#define   PIN_I2C0_SDA                12

#define   VAL_CLCD_ROW_COUNT          4       // Character CLCD row count
#define   VAL_CLCD_COL_COUNT          20      // Character CLCD column count
#define   VAL_CLCD_ADDRESS            0x3F    // Character CLCD I2C address

#define   DEBUG_SERIAL                Serial
#define   DEBUG_SERIAL_BAUDRATE       115200

//=========================================================================//

CSE_I2C_LCD CLCD (&Wire, VAL_CLCD_ADDRESS, VAL_CLCD_COL_COUNT, VAL_CLCD_ROW_COUNT); // Character LCD

int counter = 0;

//=========================================================================//

void setup() {
  DEBUG_SERIAL.begin (DEBUG_SERIAL_BAUDRATE);
  delay (2000);
  DEBUG_SERIAL.println ("Character LCD Test");

  initWire();

  CLCD.begin(); // Initialize the CLCD
  CLCD.clear(); // Clear the CLCD (set cursor to 0, 0)
  CLCD.backlightOn(); // Turn on the CLCD backlight

  CLCD.print ("Hello World");
  CLCD.setCursor (0, 1);
  CLCD.print (String (CLCD.colCount) + " x " + String (CLCD.rowCount) + " CLCD Test");
  CLCD.setCursor (0, 2);
  CLCD.print ("CIRCUITSTATE");

  delay (1000);
  CLCD.clear();
}

//=========================================================================//

void loop() {
  testLCD();
}

//=========================================================================//

bool initWire() {
  DEBUG_SERIAL.print ("Initializing I2C.. ");

  Wire.setSCL (PIN_I2C0_SCL);
  Wire.setSDA (PIN_I2C0_SDA);
  Wire.begin();

  DEBUG_SERIAL.println ("Done.");
  return true;
}

//=========================================================================//

void testLCD() {
  CLCD.clear();
  CLCD.setCursor (0, 0);
  CLCD.print ("Text at 0, 0");
  delay (1000);

  // CLCD.clear();
  // CLCD.backlightOff();
  // CLCD.setCursor (0, 0);
  // CLCD.print ("Backlight OFF");
  // delay (1000);

  // CLCD.clear();
  // CLCD.backlightOn();
  // CLCD.setCursor (0, 0);
  // CLCD.print ("Backlight ON");
  // delay (1000);

  // CLCD.clear();
  // CLCD.setCursor (0, 0);
  // CLCD.print ("Display OFF");
  // delay (1000);
  // CLCD.displayOff();
  // delay (1000);

  // CLCD.clear();
  // CLCD.displayOn();
  // CLCD.setCursor (0, 0);
  // CLCD.print ("Display ON");
  // delay (1000);

  // CLCD.clear();
  // CLCD.blinkOn();
  // CLCD.setCursor (0, 0);
  // CLCD.print ("Blink ON");
  // delay (2000);

  // CLCD.clear();
  // CLCD.blinkOff();
  // CLCD.setCursor (0, 0);
  // CLCD.print ("Blink OFF");
  // delay (2000);

  // CLCD.clear();
  // CLCD.cursorOn();
  // CLCD.setCursor (0, 0);
  // CLCD.print ("Cursor ON");
  // delay (2000);

  // CLCD.clear();
  // CLCD.cursorOff();
  // CLCD.setCursor (0, 0);
  // CLCD.print ("Cursor OFF");
  // delay (2000);

  // CLCD.clear();
  // CLCD.setCursor (0, 0);
  // CLCD.print ("Scroll Right");

  // for (int i = 0; i < 20; i++) {
  //   CLCD.scrollDisplayRight();
  //   delay (200);
  // }
  // delay (1000);

  // CLCD.clear();
  // CLCD.setCursor (0, 0);
  // CLCD.print ("Scroll Left");

  // for (int i = 0; i < 20; i++) {
  //   CLCD.scrollDisplayLeft();
  //   delay (200);
  // }
  // delay (1000);

  // CLCD.clear();
  // CLCD.rightToLeft();
  // CLCD.setCursor (19, 0);
  // CLCD.print ("Right Text");
  // delay (2000);

  // CLCD.clear();
  // CLCD.leftToRight();
  // CLCD.setCursor (0, 0);
  // CLCD.print ("Left Text");
  // delay (2000);

  CLCD.clear();
  // CLCD.setCursor (0, 0);
  // CLCD.print ("Autoscroll ON");
  CLCD.autoscrollOn();
  CLCD.setCursor (19, 0);
  CLCD.print ("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890");
  CLCD.autoscrollOff();
  delay (2500);

  // CLCD.clear();
  // CLCD.autoscrollOff();
  // CLCD.setCursor (0, 0);
  // CLCD.print ("Autoscroll OFF");
  // CLCD.setCursor (0, 2);
  // CLCD.print ("Some long text that scrolls");
  // delay (2000);
}

//=========================================================================//