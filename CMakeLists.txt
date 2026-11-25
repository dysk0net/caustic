cmake_minimum_required(VERSION 3.22)

project(Caustic VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)
FetchContent_Declare(
    JUCE
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG 8.0.6
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(JUCE)

juce_add_plugin(Caustic
    COMPANY_NAME "Caustic Audio"
    BUNDLE_ID "audio.caustic.synth"
    PLUGIN_MANUFACTURER_CODE Caus
    PLUGIN_CODE Caus
    PRODUCT_NAME "Caustic"
    DESCRIPTION "Modular polyphonic synthesizer"
    FORMATS VST3 Standalone
    IS_SYNTH TRUE
    NEEDS_MIDI_INPUT TRUE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS TRUE
    COPY_PLUGIN_AFTER_BUILD FALSE
)

juce_generate_juce_header(Caustic)

target_sources(Caustic PRIVATE
    Source/PluginProcessor.cpp
    Source/PluginEditor.cpp
)

target_include_directories(Caustic PRIVATE Source)

target_compile_definitions(Caustic PUBLIC
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
)

target_link_libraries(Caustic PRIVATE
    juce::juce_audio_utils
    juce::juce_dsp
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags
)

if(MSVC)
    target_compile_options(Caustic PRIVATE /W4 /permissive-)
else()
    target_compile_options(Caustic PRIVATE -Wall -Wextra -Wpedantic)
endif()
