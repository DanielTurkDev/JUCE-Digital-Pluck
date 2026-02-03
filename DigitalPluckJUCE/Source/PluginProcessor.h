/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class DigitalPluckAudioProcessor  : public juce::AudioProcessor
{
public:
    float noteAmplitude = 0.5; 
    float noteFrequency = 440;
    float noteDuration = 1000;
   
    double decayCoeff;
    double envelopePosition = -1;
    float endProportion = 0.001; // desired proportion of original amplitude desired at end of decay
    

    //==============================================================================
    DigitalPluckAudioProcessor();
    ~DigitalPluckAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    // custom stuff 
    void updateAngleDelta(); // updates delta for sine
    void computeDecayCoeff(); // computes coefficent at which decay is achieved


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

private:
    double SampleRate = 0.0, currentAngle = 0.0, angleDelta = 0.0; // [1]
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DigitalPluckAudioProcessor)
};
