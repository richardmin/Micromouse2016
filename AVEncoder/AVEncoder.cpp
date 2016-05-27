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
 
#include "AVEncoder.h"
 

AVEncoder::AVEncoder(PinName channelA, PinName channelB) : chanA(channelA), chanB(channelB) {
    m_pulses = 0;
    chanA.rise(this, &AVEncoder::incrementCount);
    chanA.fall(this, &AVEncoder::incrementCount);
    chanB.rise(this, &AVEncoder::incrementCount);
    chanB.fall(this, &AVEncoder::incrementCount);
}

void AVEncoder::reset() {
    m_pulses = 0;
}

unsigned long int AVEncoder::getPulses() {
    return m_pulses;
}

void AVEncoder::incrementCount() {
    m_pulses++;
}
