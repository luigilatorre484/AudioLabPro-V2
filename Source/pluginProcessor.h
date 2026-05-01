#pragma once
#include <JuceHeader.h>
#include <vector>

class AudioLabProAudioProcessor  : public juce::AudioProcessor
{
public:
    AudioLabProAudioProcessor();
    ~AudioLabProAudioProcessor() override;

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    const juce::String getName() const override { return "AudioLab Pro: Finalizer"; }
    
    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }
    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram (int index) override {}
    const juce::String getProgramName (int index) override { return {}; }
    void changeProgramName (int index, const juce::String& newName) override {}
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;

    // VARIABILI PUBBLICHE
    bool isAnalyzing = true;
    bool isBypassed = false;
    float loudnessAmount = 1.0f;
    float stereoWidth = 1.0f;
    float toneValue = 1.0f;
    float driveAmount = 1.0f;
    float inputLevel = 0.0f;
    float outputLevel = 0.0f;
    float dbIn = -60.0f;
    float dbOut = -60.0f;
    
    enum GenreMode { Pop, Trap, Rock, Techno, Jazz, HipHop, Metal, Acoustic, Elettronica, Ambient };
    GenreMode currentGenre = Pop;

    std::vector<float>& getFFTData() { return fftData; }

private:
    std::vector<float> fftData;
    juce::dsp::FFT forwardFFT;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioLabProAudioProcessor)
};
