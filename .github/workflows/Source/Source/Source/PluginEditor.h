#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"

class AudioLabProAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    AudioLabProAudioProcessorEditor (AudioLabProAudioProcessor&);
    ~AudioLabProAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    AudioLabProAudioProcessor& audioProcessor;

    juce::Slider gainSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioLabProAudioProcessorEditor)
};
