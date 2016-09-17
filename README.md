<p align="center">
	<img alt="Brain Logo" src="https://brain-labs.github.io/brain-visualizer/img/brain.png" width="100" />
</p>

# What it is

Brainduino is a Brain interpreter for Arduino. You can write programs in Brain and execute it on Arduino.

## Installation

To install this library, just place this entire folder as a subfolder in your
Arduino/lib/targets/libraries folder.

When installed, this library should look like:

* Arduino/libraries/Brain              (this library's folder)
* Arduino/libraries/Brain/Brain.cpp     (the library implementation file)
* Arduino/libraries/Brain/Brain.h       (the library description file)
* Arduino/libraries/Brain/keywords.txt (the syntax coloring file)
* Arduino/libraries/Brain/examples     (the examples in the "open" menu)

## Building

After this library is installed, you just have to start the Arduino application.
You may see a few warning messages as it's built.

To use this library in a sketch, go to the Sketch | Import Library menu and
select Brain. This will add a corresponding line to the top of your sketch:

`#include <Brain.h>`

To stop using this library, delete that line from your sketch.

### Geeky information

After a successful build of this library, a new file named "Brain.o" will appear
in `Arduino/libraries/Brain`. This file is the built/compiled library
code.

If you choose to modify the code for this library (i.e. "Brain.cpp" or "Brain.h"),
then you must first 'unbuild' this library by deleting the "Brain.o" file. The
new "Brain.o" with your code will appear after the next press of "verify".
