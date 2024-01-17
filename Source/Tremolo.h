//
//  Tremolo.h
//  tremolo
//
//  Created by Loïc on 16/01/2024.
//

#pragma once

#include "Utils.h"

class Tremolo
{
    public :
        Oscillator lfo;
        float modulationDepth;
        float modulationFreq;
        int modType;
    
        void initLFO () {
            lfo.amplitude = 1.0;
//            lfo.inc = modulationFreq / sampleRate;
            lfo.type = modType;
        }
        
        void reset ()
        {
            lfo.reset();
        }
        
        float nextSample()
        {
            return modulationDepth * lfo.nextSample();
        }
};

