/* This is the source code of Brainduino.
 * It is licensed under GNU GPL v. 3 or later.
 * You should have received a copy of the license in this archive (see LICENSE).
 *
 * Copyright Brain, 2016.
 */

#ifndef BRAIN_DELEGATE_H
#define BRAIN_DELEGATE_H

class Brain;

class BrainDelegate
{
public:
    // This delegate will probably exist forever 
    //virtual ~BrainDelegate() {}
    virtual void update(Brain *brain) = 0;
};


#endif
