/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
TremoloAudioProcessor::TremoloAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::mono(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::mono(), true)
                     #endif
                       )
#endif
{
}

TremoloAudioProcessor::~TremoloAudioProcessor()
{
}

//==============================================================================
const juce::String TremoloAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool TremoloAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool TremoloAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool TremoloAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double TremoloAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int TremoloAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int TremoloAudioProcessor::getCurrentProgram()
{
    return 0;
}

void TremoloAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String TremoloAudioProcessor::getProgramName (int index)
{
    return {};
}

void TremoloAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void TremoloAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    sinusoidalOsc.reset();
    modulatingOsc.reset();
    
    sinusoidalOsc.inc = oscFreq / sampleRate;
    sinusoidalOsc.amplitude = oscAmp;
    sinusoidalOsc.type = modTypeSine;

    modulatingOsc.inc = modFreq / sampleRate;
    modulatingOsc.amplitude = modAmp;
    modulatingOsc.type = modTypePhasor;
    
    DBG( modulatingOsc.inc );
    DBG( modulatingOsc.amplitude );
    DBG( modulatingOsc.type );
}

void TremoloAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool TremoloAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void TremoloAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* in = buffer.getReadPointer( channel );
        auto* channelData = buffer.getWritePointer (channel);
        
        for( int i = 0; i < buffer.getNumSamples() ; i++ )
        {
            //*channelData++ = *in++ * modulatingOsc.nextSample();
            *channelData++ = sinusoidalOsc.nextSample() * modulatingOsc.nextSample();
            //*channelData++ = sinusoidalOsc.nextSample();
            
        }
    }
}

//==============================================================================
bool TremoloAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* TremoloAudioProcessor::createEditor()
{
    auto editor = new juce::GenericAudioProcessorEditor( *this );
    editor->setSize( 500, 300 );
    return editor;
    //return new TremoloAudioProcessorEditor (*this);
}

//==============================================================================
void TremoloAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void TremoloAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new TremoloAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout TremoloAudioProcessor::createParameterLayout()
{
    /**
     PARAMETER_ID( modFreq )
     PARAMETER_ID( modDepth )
     PARAMETER_ID( modType )
     */
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    
    
    layout.add( std::make_unique<juce::AudioParameterChoice>(
        ParameterID::mainOscType, "Input Choice",
        juce::StringArray{ "Oscillator", "Audio input" },
        0
    ));
    
    layout.add( std::make_unique<juce::AudioParameterChoice>(
        ParameterID::mainOscType, "Main Oscillator type",
        juce::StringArray{ "Sine", "Saw", "Square", "SquareRounded", "Phasor"},
        1
    ));
               
    layout.add( std::make_unique<juce::AudioParameterFloat>(
        ParameterID::mainOscFreq,
        "Main Oscillator Frequency",
        juce::NormalisableRange<float>( 20.0f, 20000.0f, 1.0f, 0.3f, false ),
        220.0f,
        juce::AudioParameterFloatAttributes().withLabel( "Hz" )
    ));

    layout.add( std::make_unique<juce::AudioParameterFloat>(
        ParameterID::mainOscDepth,
        "Main Oscillator Depth",
        juce::NormalisableRange<float>( 0.0f, 1.0f, 0.01f ),
        1.0f
        //juce::AudioParameterFloatAttributes().withLabel( "Hz" )
    ));
    
    layout.add( std::make_unique<juce::AudioParameterChoice>(
        ParameterID::modType, "Modulation Type",
        juce::StringArray{ "Sine", "Saw", "Square", "SquareRounded", "Phasor"},
        1
    ));
               
    layout.add( std::make_unique<juce::AudioParameterFloat>(
        ParameterID::modFreq,
        "Modulation Frequency",
        juce::NormalisableRange<float>( 0.001f, 20.0f, 0.001f, 0.3f, false ),
        2.0f,
        juce::AudioParameterFloatAttributes().withLabel( "Hz" )
    ));

    layout.add( std::make_unique<juce::AudioParameterFloat>(
        ParameterID::modDepth,
        "Modulation Depth",
        juce::NormalisableRange<float>( 0.0f, 1.0f, 0.01f ),
        1.0f
        //juce::AudioParameterFloatAttributes().withLabel( "Hz" )
    ));
    

    return layout;
}
