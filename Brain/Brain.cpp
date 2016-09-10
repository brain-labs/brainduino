/* This is the source code of Brain Programming Language.
 * It is licensed under GNU GPL v. 3 or later.
 * You should have received a copy of the license in this archive (see LICENSE).
 *
 * Copyright Brain, 2016.
 */

#include "Brain.h"

Brain::Brain(Print *printer, Stream *streamIn, BrainDelegate *delegate)
{
    set(printer, streamIn, delegate, NULL);
}

Brain::Brain(Print *printer, Stream *streamIn, BrainDelegate *delegate, char const *code)
{
    set(printer, streamIn, delegate, code);
}

void Brain::set(Print *printer, Stream *streamIn, BrainDelegate *delegate, char const *code)
{
    _printer = printer;
    _streamIn = streamIn;

    _index = 0;
    _action = 0;
    for (int i = 0; i < TAPE_SIZE; i++) {
        _cells[i] = 0;
    }

    _delegate = delegate;
    _code = code;
}

void Brain::setCode(char const *code)
{
    _code = code;
}

void Brain::writeCode(void)
{
  _printer->println(_code);
}

void Brain::runNextLoop(void)
{
    char token = _code[action];
    // implement it
}

int Brain::getValue(int index)
{
    return _cells[index];
}

void Brain::setValue(int index, int value)
{
    _cells[index] = value;
}
