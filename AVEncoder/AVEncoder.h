/**
 * @author Aravind Vadali
 *
 * @section LICENSE
 *
 * Copyright (c) 2015
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * @section DESCRIPTION
 * Because the QEI library is buggy as !@#$
 * Instead of even trying to determine direction, this will just give number of 
 * pulses. A more fleshed out solution where direction is taken into account can 
 * be worked out later.
 **/
 
#ifndef QEI_H
#define QEI_H

/**
 * Includes
 */
#include "mbed.h"

class AVEncoder {
    
public:
    AVEncoder(PinName channelA, PinName channelB);
    unsigned long int getPulses();
    void reset();
private:
    unsigned long int m_pulses;
    InterruptIn chanA;
    InterruptIn chanB;
    void incrementCount();
};


#endif /* QEI_H */