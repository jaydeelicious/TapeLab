cmake_minimum_required(VERSION 3.22)

project(TapeLab
    VERSION 0.1.0
    LANGUAGES C CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Include the pinned JUCE dependency
add_subdirectory(external/JUCE)

# VST3 audio effect plugin
juce_add_plugin(TapeLab
    COMPANY_NAME "TapeLab"
    BUNDLE_ID "com.tapelab.audio"
    PLUGIN_MANUFACTURER_CODE Tplb
    PLUGIN_CODE Tlab
    FORMATS VST3
    PRODUCT_NAME "TapeLab"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD FALSE
)

target_sources(TapeLab
    PRIVATE
        src/PluginProcessor.cpp
        src/PluginProcessor.h
)

target_compile_definitions(TapeLab
    PRIVATE
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
)

target_link_libraries(TapeLab
    PRIVATE
        juce::juce_core
        juce::juce_recommended_config_flags
        juce::juce_recommended_warning_flags
)