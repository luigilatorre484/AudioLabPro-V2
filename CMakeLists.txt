cmake_minimum_required(VERSION 3.15)

project(AudioLabPro-V2 VERSION 0.0.1)

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

# Questa è la riga magica per saltare il controllo VST2 senza errori
set_target_properties(AudioLabPro-V2 PROPERTIES JUCE_COPY_PLUGIN_AFTER_BUILD FALSE)
target_compile_definitions(AudioLabPro-V2 PUBLIC JUCE_VST3_CAN_REPLACE_VST2=0)

juce_generate_juce_header(AudioLabPro-V2)

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
