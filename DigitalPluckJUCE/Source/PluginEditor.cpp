/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DigitalPluckAudioProcessorEditor::DigitalPluckAudioProcessorEditor 
(DigitalPluckAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 240);

    // [3] pluckVolume
    /*pluckVolume.setSliderStyle(juce::Slider::Rotary);
    pluckVolume.setRange(0.25, 5.0, 0.25);
    pluckVolume.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 90, 0);
    pluckVolume.setPopupDisplayEnabled(true, false, this);
    pluckVolume.setTextValueSuffix("Volume");
    pluckVolume.setValue(1.0);*/

    // these define the parameters of our slider object

    //juce::Slider pluckFrequency; // [1]
    //juce::Slider pluckDuration; // [2]

     // [1] pluck Frequency
    pluckFrequency.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    pluckFrequency.setRange(40, 20000, 0.01);
    pluckFrequency.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    pluckFrequency.setTextValueSuffix(" Frequency");
    pluckFrequency.setValue(440.0);
    pluckFrequency.setSkewFactorFromMidPoint(632.0); // makes it feel log
    pluckFrequency.addListener(this);
    // this function adds the slider to the editor
    addAndMakeVisible(&pluckFrequency);

    // [2] pluck Duration
    pluckDuration.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    pluckDuration.setRange(20, 2000, 10);
    pluckDuration.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    pluckDuration.setTextValueSuffix(" Duration (ms)");
    pluckDuration.setValue(1000.0);
    pluckDuration.addListener(this);
    // this function adds the slider to the editor
    addAndMakeVisible(&pluckDuration);


    // [3] pluck volume
    pluckVolume.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    pluckVolume.setRange(0.0, 1.0, 0.01);
    pluckVolume.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    pluckVolume.setTextValueSuffix(" Volume");
    pluckVolume.setValue(0.5);
    pluckVolume.addListener(this);
    // this function adds the slider to the editor
    addAndMakeVisible(&pluckVolume);

    //define our play button
    pluckTrigger.setButtonText("Play");
    pluckTrigger.addListener(this);
    // adds slider to the editor
    addAndMakeVisible(&pluckTrigger);





}

DigitalPluckAudioProcessorEditor::~DigitalPluckAudioProcessorEditor()
{
}

//==============================================================================
void DigitalPluckAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (12.0f));
   
    g.drawText("Created by Daniel Turk", getLocalBounds().reduced(10), juce::Justification::bottomRight, true);
}

void DigitalPluckAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    // set rotary pos 
    pluckFrequency.setBounds(40, 40, 80, 80);
    pluckDuration.setBounds(160, 40, 80, 80);
    pluckVolume.setBounds (280, 40, 80, 80);
    

    // set button pos
    pluckTrigger.setBounds(160, 180, 80, 20);
}

void DigitalPluckAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{

    audioProcessor.noteFrequency = pluckFrequency.getValue();
    audioProcessor.updateAngleDelta();


    audioProcessor.noteDuration = pluckDuration.getValue();
    audioProcessor.computeDecayCoeff();


    audioProcessor.noteAmplitude = pluckVolume.getValue();

}

void DigitalPluckAudioProcessorEditor::buttonClicked(juce::Button* button)
{
    if (button == &pluckTrigger) {
        audioProcessor.envelopePosition = 0;
    }
}