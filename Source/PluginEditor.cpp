#include "PluginProcessor.h"
#include "PluginEditor.h"

AudioLabProAudioProcessorEditor::AudioLabProAudioProcessorEditor (AudioLabProAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Dimensioni della finestra del plugin
    setSize (1000, 600);

    // Configurazione della manopola (Slider)
    gainSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(gainSlider);

    // Collega la manopola al parametro "gain" creato nel Processor
    gainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(p.getTreeState(), "gain", gainSlider);
}

AudioLabProAudioProcessorEditor::~AudioLabProAudioProcessorEditor() {}

void AudioLabProAudioProcessorEditor::paint (juce::Graphics& g)
{
    // Sfondo scuro per ora, poi aggiungeremo la tua immagine
    g.fillAll (juce::Colours::darkgrey);
    
    g.setColour (juce::Colours::white);
    g.setFont (24.0f);
    g.drawFittedText ("AudioLabPro V2 - Volume Control", getLocalBounds(), juce::Justification::centredTop, 1);
}

void AudioLabProAudioProcessorEditor::resized()
{
    // Posiziona la manopola al centro dello schermo (x, y, larghezza, altezza)
    gainSlider.setBounds (400, 200, 200, 200);
}
