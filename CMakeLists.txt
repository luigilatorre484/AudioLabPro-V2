cmake_minimum_required(VERSION 3.15)

project(AudioLabPro-V2 VERSION 0.0.1)

# Scarica JUCE automaticamente per evitare errori
include(FetchContent)
FetchContent_Declare(juce GITHUB_REPOSITORY juce-framework/JUCE GIT_TAG t7.0.5)
FetchContent_MakeAvailable(juce)

juce_add_plugin(AudioLabPro-V2
    COMPANY_NAME "LuigiLaTorre"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    FORMATS VST3 Standalone
    PRODUCT_NAME "AudioLabPro V2")

# Qui diciamo al computer dove saranno i file della manopola e del suono
target_sources(AudioLabPro-V2 PRIVATE
    Source/PluginProcessor.cpp
    Source/PluginProcessor.h
    Source/PluginEditor.cpp
    Source/PluginEditor.h)

target_link_libraries(AudioLabPro-V2 PRIVATE
    juce::juce_audio_utils
    juce::juce_audio_processors
    juce::juce_gui_basics
    juce::juce_graphics)
