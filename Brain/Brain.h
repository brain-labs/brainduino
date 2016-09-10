/*
 * This is the source code of Brain Programming Language.
 * It is licensed under GNU GPL v. 3 or later.
 * You should have received a copy of the license in this archive (see LICENSE).
 *
 * Copyright Brain, 2016.
 */

#ifndef Brain_h
#define Brain_h

// include types & constants of Wiring core API
#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include "Stream.h"
#include "Print.h"
#include "BrainDelegate.h"

// only tested on Arduino Uno and not enough memory
// therefore, the memory is very very small
#define TAPE_SIZE 200 

#define TT_SHIFT_LEFT '<'
#define TT_SHIFT_RIGHT '>'
#define TT_SHIFT_UP '^'
#define TT_INCREMENT '+'
#define TT_DECREMENT '-'
#define TT_OUTPUT '.'
#define TT_INPUT ','
#define TT_BEGIN_WHILE '['
#define TT_END_WHILE ']'
#define TT_BEGIN_FOR '{'
#define TT_END_FOR '}'
#define TT_MUL '*'
#define TT_DIV '/'
#define TT_REM '%'
#define TT_DEBUG '#'
#define TT_BREAK '!'
#define TT_IF_THEN '?'
#define TT_IF_ELSE ':'
#define TT_IF_END ';'
#define TT_FLOAT '$'

class Brain 
{

public:
    Brain(Print *printer, Stream *streamIn, BrainDelegate *delegate);
    Brain(Print *printer, Stream *streamIn, BrainDelegate *delegate, char const *code);
    void setCode(char const *code);
    void writeCode(void);
    void begin(void);
    void runNextLoop(void);
    int getValue(int index);
    void setValue(int index, int value);

private:
    int _index;
    bool _hasBegan;
    char const *_code;
    int _cells[TAPE_SIZE];
    Print *_printer;
    Stream *_streamIn;
    BrainDelegate *_delegate;

    void set(Print *printer, Stream *streamIn, BrainDelegate *delegate, char const *code);    
};

#endif
