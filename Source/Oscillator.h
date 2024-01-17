#pragma once

#include "enums.h"

const float TWO_PI = 6.2831853071795864;

class Oscillator
{
    public :
        float amplitude;
        float inc;
        float phase;
        int type;

    
        void reset()
        {
            phase = 0.0f;
        }
        
        float nextSample ()
        {
            float output = 0.0f;
            phase += inc;
            if( phase >= 1.0f )
                phase -= 1.0f;
            
            switch( type ) 
            {
                case modTypeSaw :
                    if( phase <= 0.25f )
                        output = 0.5f + 2.0f * phase;
                    else if( phase < 0.75f )
                        output = 1.0f - 2.0f * ( phase - 0.25f );
                    else
                        output = 2.0f * ( phase - 0.75f );
                    break;
                
                case modTypeSquare :
                    if( phase <= 0.5f )
                        output = 1.0f;
                    else
                        output = 0.0f;
                    
                    break;
                
                case modTypeSquareSlopedEdges:
                    if (phase < 0.48f)
                        output = 1.0f;
                    else if (phase < 0.5f)
                        output = 1.0f - 50.0f*(phase - 0.48f);
                    else if (phase < 0.98f)
                        output = 0.0f;
                    break;
                    
                case modTypePhasor :
                    output = phase;
                    break;
                    
                case modTypeSine :
                default :
                    output = std::sin( TWO_PI * phase );
                    break;
            }

            
            return output;
        }
};
