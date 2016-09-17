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
    reset();
    _printer = printer;
    _streamIn = streamIn;
    _delegate = delegate;
    _code = code;
}

void Brain::setCode(char const *code)
{
    _code = code;
}

void Brain::reset(void)
{
    _index = 0;
    _indexJumps = 0;
    _indexIterations = 0;
    _action = 0;
    for (int i = 0; i < TAPE_SIZE; i++) {
        _cells[i] = 0;
    }
}

void Brain::run(void)
{
    if (!_code) {
        write("No Code!", true);  
        return;
    }

    for(int sizeCode = sizeof(_code); _action < sizeCode; /* no increment*/) {
        char token = _code[action];
        switch(token) {
            case '<': _index--; break;
            case '>': _index++; break;
            case '^': _index = _cells[_index]; break;
            case '+': _cells[_index]++; break;
            case '-': _cells[_index]--; break;
            case '*': _cells[_index] *= _cells[_index - 1]; break;
            case '/': _cells[_index] /= _cells[_index - 1]; break;
            case '%': _cells[_index] %= _cells[_index - 1]; break;
            case ',': _cells[_index] = read(); break;
            case '.': write(char(_cells[_index])); break;
            case '$': write(_cells[_index] / 100); break;
            case '#': {
                write("Index Pointer: ", true);
                write(_index);
                write(" Value at Index Pointer: ", true);
                write(_cells[_index]);
                break;
            }
            default: break;
        }
        
        action++;
    }
}

int Brain::getValue(int index)
{
    return _cells[index];
}

void Brain::setValue(int index, int value)
{
    _cells[index] = value;
}

void Brain::write(char const *str, boolean newLine)
{
    if (_printer) {
        if(newLine) {
            _printer->println(str);
        } else {
            _printer->print(str);
        }
    }
}

void write(char c) 
{
    if (_printer) {
        _printer->print(c);
    }
}

void write(int i) 
{
    if (_printer) {
        _printer->print(i);
    }
}

int Brain::read(void)
{
    if (_streamIn) {
        while (!_streamIn->available()) {
            // wait for input
        }
       
        return streamIn->read();
    }

    return 0;
}
