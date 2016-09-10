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

class Brain 
{

public:
    Brain(Print *printer, Stream *streamIn, char const *code);
    void write(void);

private:
    Print *_printer;
    Stream *_streamIn;
    char const *_code;
};

#endif

