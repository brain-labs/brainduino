/*
 * This is the source code of Brainduino.
 * It is licensed under GNU GPL v. 3 or later.
 * You should have received a copy of the license in this archive (see LICENSE).
 *
 * Copyright Brain, 2016.
 */

#ifndef BRAIN_H
#define BRAIN_H

// include types & constants of Wiring core API
#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include "Stream.h"
#include "Print.h"
#include "BrainDelegate.h"

/*
 * The TAPE_SIZE depends on which kind of Arduino you are running on. Therefore
 * it needs to be small on most platforms. The library was tested on:
 *
 * Arduino Uno (2KiB of memory)
 * Arduino Nano (2KiB of memory)
 *
 * Note: you may change the macro TAPE_SIZE
 * in order to increase or decrease your tape size.
 * You can do the same for the size of the stack.
 */

#ifndef TAPE_SIZE
#define TAPE_SIZE 100
#endif //TAPE_SIZE

#ifndef STACK_SIZE
#define STACK_SIZE 10
#endif //STACK_SIZE

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

/**
 * @brief Represents the Brain interpreter and also provide functions to be
 * used in the Arduino environment.
 */
class Brain
{
public:
    /**
     * @brief Class constructor.
     * @param printer The default Output object.
     * @param stream_in The default Stream object.
     * @param delegate
     */
    Brain(Print *printer, Stream *stream_in, BrainDelegate *delegate);
    /**
     * @brief Class constructor.
     * @param printer The default Print object.
     * @param stream_in The default Stream object.
     * @param delegate
     * @param code The code string to be executed inside Brain.
     */
    Brain(Print *printer, Stream *stream_in, BrainDelegate *delegate,
          char const *code);
    /**
     * @brief Set the code string to be executed.
     * @param code A string of Brain code.
     */
    void setCode(char const *code);
    /**
     * @brief Executes Brain code on Arduino.
     */
    int run();
    /**
     * @brief Gets the value of the cell at index.
     * @param index The index which
     */
    int getValue(int index);
    /**
     * @brief Sets the value of the cell at index.
     * @param index The index to reference the cell.
     * @param value The value to be placed inside the cell.
     */
    void setValue(int index, int value);
    /**
     * @brief Reset all cells of Brain to 0.
     */
    void reset();

private:
    /// The code string.
    char const *_code;
    /// The Brain tape.
    int _cells[TAPE_SIZE];
    Print *_printer;
    Stream *_stream_in;
    /**
     * Brainduino uses a delegate to update or to execute any information at
     * each Arduino loop cycle.
     */
    BrainDelegate *_delegate;

    /**
     * @brief Sets properties of Brain
     * @param printer The default Printer object.
     * @param stream_in The default Stream object.
     * param delegate
     * @param code The default code string.
     */
    void set(Print *printer, Stream *stream_in, BrainDelegate *delegate,
             char const *code);
    /*
     * @brief Writes a string to stdout.
     * @param str A string.
     * @param new_line if set to true will print '\n'
     */
    void write(char const *str, boolean new_line);
    /**
     * @brief Writes characters to the stdout.
     * @param c A character.
     */
    void write(char c);
    /**
     * @brief Writes integers to the stdout.
     * @param i An integer.
     */
    void write(int i);
    /**
     * @brief Execute Brain code from _code string.
     * @returns A integer representing the state of its execution.
     */
    int exec();
    /**
     * @brief Reads a value available from Stream.
     * @returns The read value on successfull and 0 if nothing was read.
     */
    int read();
};

#endif // BRAIN_H
