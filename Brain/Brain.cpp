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
    reset();
    if (!_code) {
        write("No Code!", true);  
        return;
    }

    while(_code[_action] != '\0') {
        char token = _code[_action];
        switch(token) {
            case TT_SHIFT_LEFT: _index--; break;
            case TT_SHIFT_RIGHT: _index++; break;
            case TT_SHIFT_UP: _index = _cells[_index]; break;
            case TT_INCREMENT: _cells[_index]++; break;
            case TT_DECREMENT: _cells[_index]--; break;
            case TT_MUL: _cells[_index] *= _cells[_index - 1]; break;
            case TT_DIV: _cells[_index] /= _cells[_index - 1]; break;
            case TT_REM: _cells[_index] %= _cells[_index - 1]; break;
            case TT_INPUT: _cells[_index] = read(); break;
            case TT_OUTPUT: write(char(_cells[_index])); break;
            case TT_FLOAT: write(_cells[_index] / 100); break;
            case TT_DEBUG: {
                write("Index Pointer: ", false);
                write(_index);
                write(" Value at Index Pointer: ", false);
                write(_cells[_index]);
                write('\n');
                break;
            }
            case TT_BEGIN_WHILE:
            case TT_BEGIN_FOR:
            case TT_IF_THEN:
            case TT_BREAK: {
                if (_cells[_index] != 0
                    && (token == TT_BEGIN_WHILE || token == TT_IF_THEN)) {
                    _jumps[_indexJumps] = _action;
                    _indexJumps++;
                } else if (_cells[_index] > 0 && token == TT_BEGIN_FOR) {
                    _iterations[_indexIterations] = _cells[_index];
                    _indexIterations++;
                    _jumps[_indexJumps] = _action;
                    _indexJumps++;
                } else if (token == TT_BREAK && !_indexJumps) {
                    write("Wrong '!'");
                } else {
                    int loops = 1;
                    while (loops > 0 && _code[_action] != '\0' ) {
                        action++;
                        if (loops == 1 
                            && _code[_action] == TT_IF_ELSE 
                            && token == TT_IF_THEN) {
                            loops--;
                            _jumps[_indexJumps] = _action;
                            _indexJumps++;
                        } else if (_code[_action] == TT_END_WHILE
                                   || _code[_action] == TT_END_FOR
                                   || _code[_action] == TT_IF_END) {
                            loops--;
                            if (token == TT_BREAK) {
                                int theJump = _code[_jumps[_indexJumps - 1]];
                                if (theJump == TT_BEGIN_WHILE) {
                                    _indexJumps--;
                                } else if (theJump == TT_BEGIN_FOR) {
                                    _indexJumps--;
                                    _indexIterations--;
                                } else {
                                    // it is not a real loop
                                    _indexJumps--;
                                    loops++;
                                }
                            }
                        } else if (_code[_action] == TT_BEGIN_WHILE
                                   || _code[_action] == TT_BEGIN_FOR
                                   || _code[_action] == TT_IF_THEN) {
                            loops++;
                        }
                    }
                }

                break;
            }
            case TT_END_WHILE:
            case TT_END_FOR:
            case TT_IF_ELSE:
            case TT_IF_END: {
                if (_code[_jumps[_indexJumps - 1]] == TT_IF_THEN) {
                    if (token == TT_IF_ELSE) {
                        int loops = 1;
                        while (loops > 0 && _code[_action] != '\0') {
                            action++;
                            if (_code[_action] == TT_END_WHILE
                                || _code[_action] == TT_END_FOR
                                || _code[_action] == TT_IF_END) {
                                loops--;
                            } else if (_code[_action] == TT_BEGIN_WHILE
                                       || _code[_action] == TT_BEGIN_FOR
                                       || _code[_action] == TT_IF_THEN) {
                                loops++;
                            }
                        }
                    }
                    
                    _indexJumps--;
                } else if (_code[_jumps[_indexJumps - 1] == TT_BEGIN_FOR]) {
                    _iterations[_indexIterations - 1]--;
                    if (_iterations[_indexIterations - 1] > 0) {
                        _action = _jumps[_indexJumps - 1];
                    } else {
                        _indexIterations--;
                        _indexJumps--;
                    }
                } else {
                    if (_code[_jumps[_indexJumps - 1]] == TT_IF_ELSE) {
                        _indexJumps--;
                    } else if (_cells[_index] != 0) {
                        _action = jumps[_indexJumps - 1];
                    } else {
                        _indexJumps--;
                    }
                }

                break;
            }
    
            default: break;
        }
        
        if (_action != '\0') {
            _action++;
        }
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

    delay(10);
}

void Brain::write(char c) 
{
    if (_printer) {
        _printer->print(c);
    }

    delay(10);
}

void Brain::write(int i) 
{
    if (_printer) {
        _printer->print(i);
    }

    delay(10);
}

int Brain::read(void)
{
    if (_streamIn) {
        while (!_streamIn->available()) {
            // wait for input
        }
       
        return _streamIn->read();
    }

    return 0;
}
