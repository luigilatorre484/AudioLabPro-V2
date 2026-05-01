#include "PluginProcessor.h"
#include "PluginEditor.h"

AudioLabProAudioProcessor::AudioLabProAudioProcessor()
    : AudioProcessor (BusesProperties().withInput("Input", juce::AudioChannelSet::stereo(), true)
                                       .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      treeState (*this, nullptr, "PARAMETERS", createParameterLayout())
{}

AudioLabProAudioProcessor::~AudioLabProAudioProcessor() {}

juce::AudioProcessorValueTreeState::ParameterLayout AudioLabProAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    params.push_back (std::make_unique<juce::AudioParameterFloat> ("gain", "Volume", 0.0f, 1.0f, 0.5f));
    return { params.begin(), params.end() };
}

void AudioLabProAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock) {}
void AudioLabProAudioProcessor::releaseResources() {}

void AudioLabProAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    auto gain = treeState.getRawParameterValue ("gain")->load();
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        buffer.applyGain (channel, 0, buffer.getNumSamples(), gain);
    }
}

juce::AudioProcessorEditor* AudioLabProAudioProcessor::createEditor() {
    return new AudioLabProAudioProcessorEditor (*this);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() {
    return new AudioLabProAudioProcessor();
}
