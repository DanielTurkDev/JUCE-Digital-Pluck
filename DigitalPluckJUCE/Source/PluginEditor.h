/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class DigitalPluckAudioProcessorEditor  : public juce::AudioProcessorEditor,
    private juce::Slider::Listener, private juce::Button::Listener
                                          
{
public:
    DigitalPluckAudioProcessorEditor (DigitalPluckAudioProcessor&);
    ~DigitalPluckAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    void sliderValueChanged(juce::Slider* slider) override;
    void buttonClicked(juce::Button* button) override;
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    DigitalPluckAudioProcessor& audioProcessor;

    // Declare custom knobs
    juce::Slider pluckFrequency; // [1]
    juce::Slider pluckDuration; // [2]
    juce::Slider pluckVolume; // [3]

    // Declare button trigger
    juce::TextButton pluckTrigger; // [4]

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DigitalPluckAudioProcessorEditor)
};
