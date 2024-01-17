#pragma once

const float TWO_PI = 6.2831853071795864;

class Oscillator
{
    public :
        float amplitude;
        float inc;
        float phase;
        
        void reset()
        {
            phase = 0.0f;
        }
        
        float nextSample ()
        {
            phase += inc;
            if( phase >= 1.0f )
                phase -= 1.0f;
            float output = std::sin( TWO_PI * phase );
            
            return output;
        }
};
