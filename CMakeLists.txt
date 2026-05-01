cmake_minimum_required(VERSION 3.15)

project(AudioLabPro_Finalizer VERSION 2.0.0 LANGUAGES CXX)

# Include JUCE - Assicurati che la cartella si chiami JUCE
add_subdirectory(JUCE)

juce_add_plugin(AudioLabPro-V2
    COMPANY_NAME "AudioLab"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS TRUE
    COPY_PLUGIN_AFTER_BUILD TRUE
    PLUGIN_MANUFACTURER_CODE "ALab"
    PLUGIN_CODE "Finz"
    FORMATS VST3
    PRODUCT_NAME "AudioLab Pro Finalizer")

target_sources(AudioLabPro-V2 PRIVATE
    Source/PluginProcessor.cpp
    Source/PluginProcessor.h
    Source/PluginEditor.cpp
    Source/PluginEditor.h)

# Questo comando gestisce l'immagine di sfondo
juce_add_binary_data(AudioData SOURCES
    Resources/background.png)

target_link_libraries(AudioLabPro-V2 PRIVATE
    AudioData
    juce::juce_audio_utils
    juce::juce_audio_processors
    juce::juce_gui_basics
    juce::juce_graphics
    juce::juce_dsp
    juce::juce_data_structures
    juce::juce_events
    juce::juce_audio_basics)

target_compile_definitions(AudioLabPro-V2 PUBLIC
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0)

target_compile_features(AudioLabPro-V2 PRIVATE cxx_std_17)
