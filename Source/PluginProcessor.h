/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Oscillator.h"
#include "Tremolo.h"
#include "Utils.h"

namespace ParameterID
{
    #define PARAMETER_ID(str) const juce::ParameterID str(#str, 1);
    
    PARAMETER_ID( inputChoice )
    PARAMETER_ID( mainOscFreq )
    PARAMETER_ID( mainOscAmp )
    PARAMETER_ID( mainOscType )
    PARAMETER_ID( modFreq )
    PARAMETER_ID( modDepth )
    PARAMETER_ID( modType )
    
    #undef PARAMETER_ID
}

//==============================================================================
/**
*/
class TremoloAudioProcessor  : public juce::AudioProcessor,
                               private juce::ValueTree::Listener
{
public:
    //==============================================================================
    TremoloAudioProcessor();
    ~TremoloAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    juce::AudioProcessorValueTreeState aptvs { *this, nullptr, "Parameters", createParameterLayout() };
    
private:
    Oscillator sinusoidalOsc, modulatingOsc;
    //float oscFreq = 180.0f;
    //float oscAmp = 0.5f;
    //int oscType = 1;
    //float modFreq = 5.0f;
    //float modAmp = 1.0f;
    //int modType = 0;
    
    int sampleRate = 0.0f;
    
    juce::AudioParameterChoice* inputParam;
    juce::AudioParameterFloat*  mainOscFreqParam;
    juce::AudioParameterFloat*  mainOscAmpParam;
    juce::AudioParameterChoice* mainOscTypeParam;
    juce::AudioParameterFloat*  modFreqParam;
    juce::AudioParameterFloat*  modDepthParam;
    juce::AudioParameterChoice* modTypeParam;
    
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    
    void update();
    
    void valueTreePropertyChanged( juce::ValueTree&, const juce::Identifier& ) override
    {
        parametersChanged.store( true );
    }
    
    std::atomic<bool> parametersChanged { false };
    

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TremoloAudioProcessor)
};
