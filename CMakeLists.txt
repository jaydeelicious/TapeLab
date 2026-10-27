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

# Standalone DSP library
add_library(TapeLabDSP STATIC
    src/dsp/TapeSaturation.cpp
    src/dsp/TapeSaturation.h
)

target_compile_features(TapeLabDSP
    PUBLIC
        cxx_std_20
)

target_include_directories(TapeLabDSP
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

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
        TapeLabDSP
        juce::juce_core
        juce::juce_recommended_config_flags
        juce::juce_recommended_warning_flags
)

# ----------------------------------------------------------------
# Testing 
# ----------------------------------------------------------------

include(CTest)

if(BUILD_TESTING)
    include(FetchContent)

    FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.8.1
        GIT_SHALLOW TRUE
    )

    FetchContent_MakeAvailable(Catch2)

    add_subdirectory(tests)
endif()
