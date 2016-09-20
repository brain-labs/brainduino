/* This is the source code of Brain Programming Language.
 * It is licensed under GNU GPL v. 3 or later.
 * You should have received a copy of the license in this archive (see LICENSE).
 *
 * Copyright Brain, 2016.
 */

#include <SPI.h>
#include <EEPROM.h>
#include <boards.h>
#include <RBL_nRF8001.h>
#include <Brain.h>
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

Brain brain = Brain(&lcd, &Serial, NULL);

void setup()
{    
  // Set your BLE Shield name here, max. length 10
  //ble_set_name("My Name");
  
  ble_begin();
  
  // Enable serial debug
  Serial.begin(57600);

  lcd.begin(16, 2);
  lcd.noAutoscroll();
  brain.set_code("++>+++++>++++++++++>+**<[-]+*>*<<%>>$>++++++++++>+++*++.<{>>+<<}>>>+**+++++.++++++++++.<<.<<<<?>>>>>>----.-----------..:>>>>>>>+*<<++++{>>-<<}>>.<+++.>.<--------.;");

}

void loop()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  if ( ble_available() )
  {
    while ( ble_available() )
      lcd.write(ble_read());
  }
  else
  {
    brain.reset();
    brain.run();
  }

  delay(1000);
  if ( Serial.available() )
  {
    delay(5);
    
    while ( Serial.available() )
        ble_write( Serial.read() );
  }
  
  ble_do_events();
}

