/* This is the source code of Brainduino.
 * It is licensed under GNU GPL v. 3 or later.
 * You should have received a copy of the license in this archive (see LICENSE).
 *
 * Copyright Brain, 2016.
 */

#include "Brain.h"

Brain::Brain(Print *printer, Stream *stream_in, BrainDelegate *delegate)
{
    set(printer, stream_in, delegate, 0);
}

Brain::Brain(Print *printer, Stream *stream_in, BrainDelegate *delegate,
             int seed)
{
    set(printer, stream_in, delegate, seed);
}

void Brain::set(Print *printer, Stream *stream_in, BrainDelegate *delegate,
                int seed)
{
    reset();
    _printer = printer;
    _stream_in = stream_in;
    _delegate = delegate;
    randomSeed(seed);
}

void Brain::setCode(char const *code)
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
    int index = 0;
    int action = 0;
    int jumps[STACK_SIZE];
    int index_jumps = 0;
    int iterations[STACK_SIZE];
    int index_iterations = 0;
    char err[6] = "Error";

    if (!_code) {
        write(err, true);
        return 1;
    }

    while(_code[action] != '\0') {
        char token = _code[action];
        switch(token) {
            case TT_SHIFT_LEFT: index--; break;
            case TT_SHIFT_RIGHT: index++; break;
            case TT_SHIFT_UP: index = _cells[index]; break;
            case TT_INCREMENT: _cells[index]++; break;
            case TT_DECREMENT: _cells[index]--; break;
            case TT_RANDOM: _cells[index] = random(0, BRAIN_RANDOM_MAX); break;
            case TT_MUL: _cells[index] *= _cells[index - 1]; break;
            case TT_DIV: _cells[index] /= _cells[index - 1]; break;
            case TT_REM: _cells[index] %= _cells[index - 1]; break;
            case TT_INPUT: _cells[index] = read(); break;
            case TT_OUTPUT: write(char(_cells[index])); break;
            case TT_FLOAT: write(_cells[index] / 100); break;
            case TT_DEBUG: {
                write("ptr:", false);
                write(index);
                write(" #:", false);
                write(_cells[index]);
                write('\n');
                break;
            }
            case TT_BEGIN_WHILE:
            case TT_BEGIN_FOR:
            case TT_IF_THEN:
            case TT_BREAK: {
                if (_cells[index] != 0
                    && (token == TT_BEGIN_WHILE || token == TT_IF_THEN)) {
                    jumps[index_jumps] = action;
                    index_jumps++;
                } else if (_cells[index] > 0 && token == TT_BEGIN_FOR) {
                    iterations[index_iterations] = _cells[index];
                    index_iterations++;
                    jumps[index_jumps] = action;
                    index_jumps++;
                } else if (token == TT_BREAK && !index_jumps) {
                    write(err, true);
                    return -1;
                } else {
                    int loops = 1;
                    while (loops > 0 && _code[action] != '\0' ) {
                        action++;
                        if (loops == 1
                            && _code[action] == TT_IF_ELSE
                            && token == TT_IF_THEN) {
                            loops--;
                            jumps[index_jumps] = action;
                            index_jumps++;
                        } else if (_code[action] == TT_END_WHILE
                                   || _code[action] == TT_END_FOR
                                   || _code[action] == TT_IF_END) {
                            loops--;
                            if (token == TT_BREAK) {
                                int theJump = _code[jumps[index_jumps - 1]];
                                if (theJump == TT_BEGIN_WHILE) {
                                    index_jumps--;
                                } else if (theJump == TT_BEGIN_FOR) {
                                    index_jumps--;
                                    index_iterations--;
                                } else {
                                    // it is not a real loop
                                    index_jumps--;
                                    loops++;
                                }
                            }
                        } else if (_code[action] == TT_BEGIN_WHILE
                                   || _code[action] == TT_BEGIN_FOR
                                   || _code[action] == TT_IF_THEN) {
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
                if (_code[jumps[index_jumps - 1]] == TT_IF_THEN) {
                    if (token == TT_IF_ELSE) {
                        int loops = 1;
                        while (loops > 0 && _code[action] != '\0') {
                            action++;
                            if (_code[action] == TT_END_WHILE
                                || _code[action] == TT_END_FOR
                                || _code[action] == TT_IF_END) {
                                loops--;
                            } else if (_code[action] == TT_BEGIN_WHILE
                                       || _code[action] == TT_BEGIN_FOR
                                       || _code[action] == TT_IF_THEN) {
                                loops++;
                            }
                        }
                    }

                    index_jumps--;
                } else if (_code[jumps[index_jumps - 1]] == TT_BEGIN_FOR) {
                    iterations[index_iterations - 1]--;
                    if (iterations[index_iterations - 1] > 0) {
                        action = jumps[index_jumps - 1];
                    } else {
                        index_iterations--;
                        index_jumps--;
                    }
                } else {
                    if (_code[jumps[index_jumps - 1]] == TT_IF_ELSE) {
                        index_jumps--;
                    } else if (_cells[index] != 0) {
                        action = jumps[index_jumps - 1];
                    } else {
                        index_jumps--;
                    }
                }

                break;
            }

            default: break;
        }

        if (_code[action] != '\0') {
            action++;
        }
    }

    return 0;
}

int Brain::getValue(int index)
{
    return _cells[index];
}

void Brain::setValue(int index, int value)
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
