/* This is the source code of Brain Programming Language.
 * It is licensed under GNU GPL v. 3 or later.
 * You should have received a copy of the license in this archive (see LICENSE).
 *
 * Copyright Brain, 2016.
 */

#include <Brain.h>

Brain brain = Brain(&Serial, NULL, "++++>>>>+++");

void setup()
{
  Serial.begin(57600);
}

void loop()
{
  brain.write();
  delay(500);
}

