/* This is the source code of Brain Programming Language.
 * It is licensed under GNU GPL v. 3 or later.
 * You should have received a copy of the license in this archive (see LICENSE).
 *
 * Copyright Brain, 2016.
 */

#include <Brain.h>
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

Brain brain = Brain(&lcd, &Serial, NULL);

void setup()
{
  Serial.begin(57600);
  lcd.begin(16, 2);
  lcd.noAutoscroll();
  brain.set_code("++>+++++>++++++++++>+**<[-]+*>*<<%>>$>++++++++++>+++*++.<{>>+<<}>>>+**+++++.++++++++++.<<.<<<<?>>>>>>----.-----------..:>>>>>>>+*<<++++{>>-<<}>>.<+++.>.<--------.;");
}

void loop()
{
  lcd.setCursor(0, 0);
  brain.reset();
  brain.run();
  delay(2000);
}
