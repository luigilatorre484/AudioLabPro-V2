#include "PluginProcessor.h"
#include "PluginEditor.h"

AudioLabProAudioProcessor::AudioLabProAudioProcessor()
    : AudioProcessor (BusesProperties().withInput("Input", juce::AudioChannelSet::stereo(), true)
                                       .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      treeState (*this, nullptr, "PARAMS", createParameterLayout())
{}

AudioLabProAudioProcessor::~AudioLabProAudioProcessor() {}

juce::AudioProcessorValueTreeState::ParameterLayout AudioLabProAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    params.push_back (std::make_unique<juce::AudioParameterFloat> ("gain", "Gain", 0.0f, 1.0f, 0.5f));
    return { params.begin(), params.end() };
}

void AudioLabProAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    auto gain = treeState.getRawParameterValue ("gain")->load();
    buffer.applyGain (gain);
}

juce::AudioProcessorEditor* AudioLabProAudioProcessor::createEditor() {
    return new AudioLabProAudioProcessorEditor (*this);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() {
    return new AudioLabProAudioProcessor();
}
