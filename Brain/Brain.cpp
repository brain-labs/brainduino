/* This is the source code of Brain Programming Language.
 * It is licensed under GNU GPL v. 3 or later.
 * You should have received a copy of the license in this archive (see LICENSE).
 *
 * Copyright Brain, 2016.
 */

#include "Brain.h"

Brain::Brain(Print *printer, Stream *streamIn, char const *code)
{
    _printer = printer;
    _streamIn = streamIn;
    _code = code;
}

void Brain::write(void)
{
  _printer->println(_code);
}

