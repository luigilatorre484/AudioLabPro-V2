#include "PluginProcessor.h"
#include "PluginEditor.h"

AudioLabProAudioProcessor::AudioLabProAudioProcessor()
     : AudioProcessor (BusesProperties()
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                       ), forwardFFT (10)
{
    fftData.assign(512, 0.0f);
}

AudioLabProAudioProcessor::~AudioLabProAudioProcessor() {}

bool AudioLabProAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;

    return true;
}

void AudioLabProAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    std::fill(fftData.begin(), fftData.end(), 0.0f);
    inputLevel = 0.0f;
    outputLevel = 0.0f;
}

void AudioLabProAudioProcessor::releaseResources() {}

void AudioLabProAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    if (isBypassed) return;

    inputLevel = buffer.getMagnitude(0, buffer.getNumSamples());
    dbIn = juce::Decibels::gainToDecibels(inputLevel + 1e-5f);

    if (isAnalyzing)
    {
        auto* left = buffer.getWritePointer(0);
        auto* right = buffer.getWritePointer(1);

        for (int i = 0; i < buffer.getNumSamples(); ++i)
        {
            float l = left[i];
            float r = right[i];

            float mid = (l + r) * 0.5f;
            float side = (l - r) * 0.5f;

            side *= (stereoWidth * 1.8f);

            float driveMid = driveAmount * loudnessAmount;
            float driveSide = driveAmount * 0.7f;

            mid = std::tanh(mid * driveMid * toneValue);
            side = std::tanh(side * driveSide);

            float outGain = 1.25f;
            left[i] = (mid + side) * outGain;
            right[i] = (mid - side) * outGain;
        }

        outputLevel = buffer.getMagnitude(0, buffer.getNumSamples());
        dbOut = juce::Decibels::gainToDecibels(outputLevel + 1e-5f);

        for (int i = 0; i < 512; ++i) {
            float target = outputLevel * (juce::Random::getSystemRandom().nextFloat() * 0.5f + 0.5f) * (1.0f - (i / 512.0f));
            fftData[i] = fftData[i] * 0.75f + target * 0.25f;
        }
    }
}

juce::AudioProcessorEditor* AudioLabProAudioProcessor::createEditor() { return new AudioLabProAudioProcessorEditor (*this); }
void AudioLabProAudioProcessor::getStateInformation (juce::MemoryBlock& destData) {}
void AudioLabProAudioProcessor::setStateInformation (const void* data, int sizeInBytes) {}
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() { return new AudioLabProAudioProcessor(); }
