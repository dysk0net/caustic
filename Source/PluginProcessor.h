#pragma once

#include <JuceHeader.h>
#include <array>
#include <memory>

class CausticAudioProcessor final : public juce::AudioProcessor
{
public:
    CausticAudioProcessor();
    ~CausticAudioProcessor() override = default;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }
    const juce::String getName() const override { return JucePlugin_Name; }
    bool acceptsMidi() const override { return true; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double get tailLengthSeconds() const override { return 2.0; }

    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int) override {}
    const juce::String getProgramName(int) override { return "Init"; }
    void changeProgramName(int, const juce::String&) override {}

    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    juce::AudioProcessorValueTreeState parameters;
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

private:
    static constexpr int maxVoices = 32;

    struct Voice
    {
        bool active = false;
        int note = 0;
        float velocity = 0.0f;
        double phase = 0.0;
        double increment = 0.0;
        float envelope = 0.0f;
        bool releasing = false;
    };

    std::array<Voice, maxVoices> voices {};
    double currentSampleRate = 44100.0;
    float filterStateL = 0.0f;
    float filterStateR = 0.0f;
    juce::Random random;

    void noteOn(int note, float velocity);
    void noteOff(int note);
    void renderVoice(Voice& voice, int startSample, int numSamples,
                     juce::AudioBuffer<float>& buffer, float attack, float decay,
                     float sustain, float release, int waveform);
    static float oscillatorSample(double phase, int waveform);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CausticAudioProcessor)
};
