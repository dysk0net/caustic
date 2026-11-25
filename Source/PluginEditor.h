#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class CausticAudioProcessorEditor final : public juce::AudioProcessorEditor
{
public:
    explicit CausticAudioProcessorEditor(CausticAudioProcessor&);
    ~CausticAudioProcessorEditor() override = default;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    using ComboAttachment = juce::AudioProcessorValueTreeState::ComboBoxAttachment;

    CausticAudioProcessor& processor;
    juce::ComboBox waveform;
    juce::Slider attack, decay, sustain, release, cutoff, resonance, lfoRate, lfoDepth, master;
    juce::Label title, subtitle;
    juce::Label waveformLabel, ampLabel, filterLabel, motionLabel, outputLabel;
    std::unique_ptr<ComboAttachment> waveformAttachment;
    std::unique_ptr<SliderAttachment> attackAttachment, decayAttachment, sustainAttachment,
        releaseAttachment, cutoffAttachment, resonanceAttachment, lfoRateAttachment,
        lfoDepthAttachment, masterAttachment;

    void styleSlider(juce::Slider&, const juce::String&);
    void styleLabel(juce::Label&, const juce::String&, int size = 12);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CausticAudioProcessorEditor)
};
