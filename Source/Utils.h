/*
  ==============================================================================

    Utils.h
    Created: 17 Jan 2024 11:13:12am
    Author:  Loïc

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

enum modulationTypes
{
    modTypeSine = 0,
    modTypeSaw,
    modTypeSquare,
    modTypePhasor,
    modTypeSquareSlopedEdges,
    
};

template<typename T>
inline static void castParameter( juce::AudioProcessorValueTreeState& aptvs, const juce::ParameterID& id, T& destination )
{
    destination = dynamic_cast<T>( aptvs.getParameter( id.getParamID() ) );
    jassert( destination );
}
