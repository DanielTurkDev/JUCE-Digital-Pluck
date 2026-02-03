/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"




//==============================================================================
DigitalPluckAudioProcessor::DigitalPluckAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

DigitalPluckAudioProcessor::~DigitalPluckAudioProcessor()
{
}

//==============================================================================
const juce::String DigitalPluckAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DigitalPluckAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DigitalPluckAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DigitalPluckAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DigitalPluckAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DigitalPluckAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DigitalPluckAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DigitalPluckAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String DigitalPluckAudioProcessor::getProgramName (int index)
{
    return {};
}

void DigitalPluckAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void DigitalPluckAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    SampleRate = sampleRate;
    updateAngleDelta();
    computeDecayCoeff();
    
}

void DigitalPluckAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DigitalPluckAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void DigitalPluckAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
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
    
    
    // cleanup buffers
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

           
    // Get a pointer to the audio data
    auto* channelDataL = buffer.getWritePointer(0);
    auto* channelDataR = buffer.getWritePointer(1); // Assuming Stereo

    
    for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
    {
        if (envelopePosition != -1) {
            float envelopeValue = exp(-decayCoeff * envelopePosition);
            float currentSample = noteAmplitude * (float)std::sin(currentAngle) * envelopeValue;

           
            // Update Sine Angle
            currentAngle += angleDelta;
            // Wrap angle to maintain precision
            if (currentAngle >= juce::MathConstants<double>::twoPi)
                currentAngle -= juce::MathConstants<double>::twoPi;

            // Incremenet Position  
            envelopePosition++; 

            channelDataL[sample] = currentSample;
            channelDataR[sample] = currentSample;

            // envelope position wrapping [2]
            if (envelopeValue < endProportion) { // this prevents us from wasting computer on subnormal numbers
                envelopePosition = -1;
                currentAngle = 0.0;
            }



            

        }
        else {
            channelDataL[sample] = 0.0f;
            channelDataR[sample] = 0.0f;
        }
               
    }

}

void DigitalPluckAudioProcessor::updateAngleDelta()
{
    auto cyclesPerSample = noteFrequency / SampleRate; // [2]
    angleDelta = cyclesPerSample * 2.0 * juce::MathConstants<double>::pi; // [3]
}

void DigitalPluckAudioProcessor::computeDecayCoeff()
{
    double endPosSamples = (noteDuration / 1000.0) * getSampleRate();
    decayCoeff = -std::log(endProportion) / endPosSamples;
}



//==============================================================================
bool DigitalPluckAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* DigitalPluckAudioProcessor::createEditor()
{
    return new DigitalPluckAudioProcessorEditor (*this);
}

//==============================================================================
void DigitalPluckAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void DigitalPluckAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DigitalPluckAudioProcessor();
}
