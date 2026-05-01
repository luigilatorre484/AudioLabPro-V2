cmake_minimum_required(VERSION 3.15)

project(AudioLabPro-V2 VERSION 0.0.1)

# Metodo più sicuro per scaricare JUCE
include(FetchContent)
FetchContent_Declare(
    juce
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG        7.0.5
)
FetchContent_MakeAvailable(juce)

juce_add_plugin(AudioLabPro-V2
    COMPANY_NAME "LuigiLaTorre"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    FORMATS VST3 Standalone
    PRODUCT_NAME "AudioLabPro V2")

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

# Importante per plugin moderni
juce_set_vst3_install_path(AudioLabPro-V2)
