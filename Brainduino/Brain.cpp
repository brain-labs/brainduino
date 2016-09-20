/* This is the source code of Brainduino.
 * It is licensed under GNU GPL v. 3 or later.
 * You should have received a copy of the license in this archive (see LICENSE).
 *
 * Copyright Brain, 2016.
 */

#include "Brain.h"

Brain::Brain(Print *printer, Stream *stream_in, BrainDelegate *delegate)
{
    set(printer, stream_in, delegate, nullptr);
}

Brain::Brain(Print *printer, Stream *stream_in, BrainDelegate *delegate,
             char const *code)
{
    set(printer, stream_in, delegate, code);
}

void Brain::set(Print *printer, Stream *stream_in, BrainDelegate *delegate,
                char const *code)
{
    reset();
    _printer = printer;
    _stream_in = stream_in;
    _delegate = delegate;
    _code = code;
}

void Brain::set_code(char const *code)
{
    _code = code;
}

void Brain::reset()
{
    for (int i = 0; i < TAPE_SIZE; i++) {
        _cells[i] = 0;
    }
}

int Brain::run()
{
    int is_err = exec();
    if (is_err) {
        return is_err;
    }

    if(_delegate) {
        _delegate->update(this);
    }

    return is_err;
}

int Brain::exec()
{
    int _index = 0;
    int _action = 0;
    int _jumps[STACK_SIZE];
    int _index_jumps = 0;
    int _iterations[STACK_SIZE];
    int _indexIterations = 0;
    char err[6] = "Error";

    if (!_code) {
        write(err, true);
        return 1;
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
                write("ptr:", false);
                write(_index);
                write(" #:", false);
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
                    _jumps[_index_jumps] = _action;
                    _index_jumps++;
                } else if (_cells[_index] > 0 && token == TT_BEGIN_FOR) {
                    _iterations[_indexIterations] = _cells[_index];
                    _indexIterations++;
                    _jumps[_index_jumps] = _action;
                    _index_jumps++;
                } else if (token == TT_BREAK && !_index_jumps) {
                    write(err, true);
                    return -1;
                } else {
                    int loops = 1;
                    while (loops > 0 && _code[_action] != '\0' ) {
                        _action++;
                        if (loops == 1
                            && _code[_action] == TT_IF_ELSE
                            && token == TT_IF_THEN) {
                            loops--;
                            _jumps[_index_jumps] = _action;
                            _index_jumps++;
                        } else if (_code[_action] == TT_END_WHILE
                                   || _code[_action] == TT_END_FOR
                                   || _code[_action] == TT_IF_END) {
                            loops--;
                            if (token == TT_BREAK) {
                                int theJump = _code[_jumps[_index_jumps - 1]];
                                if (theJump == TT_BEGIN_WHILE) {
                                    _index_jumps--;
                                } else if (theJump == TT_BEGIN_FOR) {
                                    _index_jumps--;
                                    _indexIterations--;
                                } else {
                                    // it is not a real loop
                                    _index_jumps--;
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
                if (_code[_jumps[_index_jumps - 1]] == TT_IF_THEN) {
                    if (token == TT_IF_ELSE) {
                        int loops = 1;
                        while (loops > 0 && _code[_action] != '\0') {
                            _action++;
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

                    _index_jumps--;
                } else if (_code[_jumps[_index_jumps - 1]] == TT_BEGIN_FOR) {
                    _iterations[_indexIterations - 1]--;
                    if (_iterations[_indexIterations - 1] > 0) {
                        _action = _jumps[_index_jumps - 1];
                    } else {
                        _indexIterations--;
                        _index_jumps--;
                    }
                } else {
                    if (_code[_jumps[_index_jumps - 1]] == TT_IF_ELSE) {
                        _index_jumps--;
                    } else if (_cells[_index] != 0) {
                        _action = _jumps[_index_jumps - 1];
                    } else {
                        _index_jumps--;
                    }
                }

                break;
            }

            default: break;
        }

        if (_code[_action] != '\0') {
            _action++;
        }
    }

    return 0;
}

int Brain::get_value(int index)
{
    return _cells[index];
}

void Brain::set_value(int index, int value)
{
    _cells[index] = value;
}

void Brain::write(char const *str, boolean new_line)
{
    if (_printer) {
        if(new_line) {
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

int Brain::read()
{
    if (_stream_in) {
        while (!_stream_in->available()) {
            // wait for input
        }

        return _stream_in->read();
    }

    return 0;
}
