#include "PluginProcessor.h"
#include "PluginEditor.h"

AudioLabProAudioProcessorEditor::AudioLabProAudioProcessorEditor (AudioLabProAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    backgroundImage = juce::ImageCache::getFromMemory(BinaryData::background_png, BinaryData::background_pngSize);

    auto setupKnob = [this](juce::Slider& s) {
        s.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
        s.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
        addAndMakeVisible(s);
    };

    setupKnob(toneKnob); toneKnob.setRange(0.5, 1.5, 0.01);
    toneKnob.setValue(audioProcessor.toneValue);
    toneKnob.onValueChange = [this] { audioProcessor.toneValue = (float)toneKnob.getValue(); };

    setupKnob(driveKnob); driveKnob.setRange(1.0, 3.0, 0.01);
    driveKnob.setValue(audioProcessor.driveAmount);
    driveKnob.onValueChange = [this] { audioProcessor.driveAmount = (float)driveKnob.getValue(); };

    analyzeButton.setButtonText("AUTO MIX"); analyzeButton.setClickingTogglesState(true);
    analyzeButton.setToggleState(audioProcessor.isAnalyzing, juce::dontSendNotification);
    analyzeButton.onClick = [this] { audioProcessor.isAnalyzing = analyzeButton.getToggleState(); };
    addAndMakeVisible(analyzeButton);

    bypassButton.setButtonText("BYPASS"); bypassButton.setClickingTogglesState(true);
    bypassButton.setToggleState(audioProcessor.isBypassed, juce::dontSendNotification);
    bypassButton.onClick = [this] { audioProcessor.isBypassed = bypassButton.getToggleState(); };
    addAndMakeVisible(bypassButton);

    loudnessSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    loudnessSlider.setRange(0.0, 2.0, 0.01);
    loudnessSlider.setValue(audioProcessor.loudnessAmount);
    loudnessSlider.onValueChange = [this] { audioProcessor.loudnessAmount = (float)loudnessSlider.getValue(); };
    addAndMakeVisible(loudnessSlider);

    widthSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    widthSlider.setRange(0.0, 3.0, 0.01);
    widthSlider.setValue(audioProcessor.stereoWidth);
    widthSlider.onValueChange = [this] { audioProcessor.stereoWidth = (float)widthSlider.getValue(); };
    addAndMakeVisible(widthSlider);

    genreSelector.addItemList({"Pop", "Trap", "Rock", "Techno", "Jazz", "HipHop", "Metal", "Acoustic", "Elettronica", "Ambient"}, 1);
    genreSelector.setSelectedItemIndex((int)audioProcessor.currentGenre);
    genreSelector.onChange = [this] { audioProcessor.currentGenre = (AudioLabProAudioProcessor::GenreMode)genreSelector.getSelectedItemIndex(); };
    addAndMakeVisible(genreSelector);

    setSize (800, 450);
    startTimerHz(30);
}

AudioLabProAudioProcessorEditor::~AudioLabProAudioProcessorEditor() { stopTimer(); }

void AudioLabProAudioProcessorEditor::paint (juce::Graphics& g)
{
    if (backgroundImage.isValid())
        g.drawImageWithin(backgroundImage, 0, 0, getWidth(), getHeight(), juce::RectanglePlacement::fillDestination);

    g.setColour(juce::Colours::white.withAlpha(0.9f));
    g.setFont(juce::Font("Impact", 24.0f, juce::Font::bold));
    g.drawText("AUDIOLAB PRO: FINALIZER", 0, 25, getWidth(), 35, juce::Justification::centred);
    
    g.setColour(juce::Colours::black.withAlpha(0.8f));
    g.fillRoundedRectangle(35, 115, 45, 20, 4.0f);
    g.fillRoundedRectangle(720, 115, 45, 20, 4.0f);
    g.setColour(juce::Colours::cyan);
    g.setFont(juce::Font("Consolas", 13.0f, juce::Font::bold));
    g.drawText(juce::String(audioProcessor.dbIn, 1), 35, 115, 45, 20, juce::Justification::centred);
    g.drawText(juce::String(audioProcessor.dbOut, 1), 720, 115, 45, 20, juce::Justification::centred);

    auto drawNeon = [&](juce::TextButton& b, juce::Colour col, bool active) {
        auto area = b.getBounds().toFloat();
        if (active) {
            for (int i = 1; i <= 4; ++i) { g.setColour(col.withAlpha(0.1f/i)); g.drawRoundedRectangle(area.expanded((float)i), 4.0f, 2.0f); }
            g.setColour(col); g.drawRoundedRectangle(area, 4.0f, 2.0f);
        }
    };
    drawNeon(analyzeButton, juce::Colours::cyan, audioProcessor.isAnalyzing);
    drawNeon(bypassButton, juce::Colours::red, audioProcessor.isBypassed);

    auto drawMeter = [&](int x, int y, float lvl, juce::Colour c) {
        g.setColour(juce::Colours::black.withAlpha(0.6f)); g.fillRoundedRectangle((float)x, (float)y, 12.0f, 100.0f, 2.0f);
        int h = (int)(juce::jlimit(0.0f, 1.0f, lvl * 1.5f) * 100);
        g.setColour(c); g.fillRoundedRectangle((float)x, (float)(y + 100 - h), 12.0f, (float)h, 2.0f);
    };
    drawMeter(40, 140, audioProcessor.inputLevel, juce::Colours::lime);
    drawMeter(748, 140, audioProcessor.outputLevel, juce::Colours::orangered);

    if (audioProcessor.isAnalyzing && !audioProcessor.isBypassed) {
        auto area = juce::Rectangle<int>(310, 160, 180, 85);
        auto& d = audioProcessor.getFFTData();
        for (int j = 0; j < 512; ++j) {
            float v = juce::jlimit(0.0f, 1.0f, d[j] * 12.0f);
            g.setColour(juce::Colour::fromHSV(j / 512.0f * 0.7f, 0.8f, 0.9f, 0.8f));
            g.fillRect((float)area.getX() + (j * (area.getWidth()/512.0f)), (float)area.getBottom() - (v * area.getHeight()), 1.0f, v * area.getHeight());
        }
    }
}

void AudioLabProAudioProcessorEditor::resized()
{
    toneKnob.setBounds(210, 185, 60, 60);
    driveKnob.setBounds(530, 185, 60, 60);
    analyzeButton.setBounds(350, 252, 100, 25);
    genreSelector.setBounds(340, 295, 120, 25);
    bypassButton.setBounds(360, 330, 80, 25);
    loudnessSlider.setBounds(90, 345, 160, 30);
    widthSlider.setBounds(550, 345, 160, 30);
}

void AudioLabProAudioProcessorEditor::timerCallback() { repaint(); }
