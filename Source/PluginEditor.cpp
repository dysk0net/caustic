#include "PluginEditor.h"

namespace
{
    const juce::Colour background { 0xff101820 };
    const juce::Colour panel { 0xff192631 };
    const juce::Colour ice { 0xff69c9e8 };
    const juce::Colour orange { 0xffff9b52 };
    const juce::Colour text { 0xffe6f0f5 };
    const juce::Colour muted { 0xff91a5b1 };
}

CausticAudioProcessorEditor::CausticAudioProcessorEditor(CausticAudioProcessor& p)
    : AudioProcessorEditor(&p), processor(p)
{
    setSize(900, 620);
    setResizable(true, true);
    setResizeLimits(680, 480, 1500, 1100);

    styleLabel(title, "CAUSTIC", 30);
    title.setColour(juce::Label::textColourId, ice);
    addAndMakeVisible(title);
    styleLabel(subtitle, "MODULAR SYNTHESIZER  /  VERSION 0.1", 11);
    subtitle.setColour(juce::Label::textColourId, muted);
    addAndMakeVisible(subtitle);

    waveform.addItemList({ "Sine", "Saw", "Square", "Triangle" }, 1);
    addAndMakeVisible(waveform);
    waveformAttachment = std::make_unique<ComboAttachment>(processor.parameters, "waveform", waveform);
    styleLabel(waveformLabel, "OSCILLATOR", 13); addAndMakeVisible(waveformLabel);

    styleSlider(attack, "Attack"); styleSlider(decay, "Decay");
    styleSlider(sustain, "Sustain"); styleSlider(release, "Release");
    styleSlider(cutoff, "Cutoff"); styleSlider(resonance, "Resonance");
    styleSlider(lfoRate, "LFO Rate"); styleSlider(lfoDepth, "LFO Depth");
    styleSlider(master, "Master");

    attackAttachment = std::make_unique<SliderAttachment>(processor.parameters, "attack", attack);
    decayAttachment = std::make_unique<SliderAttachment>(processor.parameters, "decay", decay);
    sustainAttachment = std::make_unique<SliderAttachment>(processor.parameters, "sustain", sustain);
    releaseAttachment = std::make_unique<SliderAttachment>(processor.parameters, "release", release);
    cutoffAttachment = std::make_unique<SliderAttachment>(processor.parameters, "cutoff", cutoff);
    resonanceAttachment = std::make_unique<SliderAttachment>(processor.parameters, "resonance", resonance);
    lfoRateAttachment = std::make_unique<SliderAttachment>(processor.parameters, "lfoRate", lfoRate);
    lfoDepthAttachment = std::make_unique<SliderAttachment>(processor.parameters, "lfoDepth", lfoDepth);
    masterAttachment = std::make_unique<SliderAttachment>(processor.parameters, "master", master);

    styleLabel(ampLabel, "AMPLITUDE ENVELOPE", 13); addAndMakeVisible(ampLabel);
    styleLabel(filterLabel, "FILTER", 13); addAndMakeVisible(filterLabel);
    styleLabel(motionLabel, "MODULATION", 13); addAndMakeVisible(motionLabel);
    styleLabel(outputLabel, "OUTPUT", 13); addAndMakeVisible(outputLabel);
}

void CausticAudioProcessorEditor::styleLabel(juce::Label& label, const juce::String& name, int size)
{
    label.setText(name, juce::dontSendNotification);
    label.setFont(juce::Font((float) size, juce::Font::bold));
    label.setColour(juce::Label::textColourId, text);
}

void CausticAudioProcessorEditor::styleSlider(juce::Slider& slider, const juce::String& name)
{
    slider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 76, 20);
    slider.setName(name);
    slider.setColour(juce::Slider::rotarySliderFillColourId, ice);
    slider.setColour(juce::Slider::rotarySliderOutlineColourId, panel.brighter(0.6f));
    slider.setColour(juce::Slider::thumbColourId, orange);
    slider.setColour(juce::Slider::textBoxTextColourId, text);
    slider.setColour(juce::Slider::textBoxBackgroundColourId, panel);
    slider.setColour(juce::Slider::textBoxOutlineColourId, panel);
    addAndMakeVisible(slider);
}

void CausticAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(background);
    g.setColour(ice.withAlpha(0.8f));
    g.fillRect(0, 0, getWidth(), 3);
    g.setColour(panel);
    g.fillRoundedRectangle(24.0f, 112.0f, (float)getWidth() - 48.0f, 142.0f, 12.0f);
    g.fillRoundedRectangle(24.0f, 272.0f, (float)getWidth() - 48.0f, 160.0f, 12.0f);
    g.fillRoundedRectangle(24.0f, 450.0f, (float)getWidth() - 48.0f, 142.0f, 12.0f);
    g.setColour(orange);
    g.fillRoundedRectangle(24.0f, 112.0f, 4.0f, 142.0f, 2.0f);
    g.fillRoundedRectangle(24.0f, 272.0f, 4.0f, 160.0f, 2.0f);
    g.fillRoundedRectangle(24.0f, 450.0f, 4.0f, 142.0f, 2.0f);
}

void CausticAudioProcessorEditor::resized()
{
    const auto w = getWidth();
    title.setBounds(28, 18, 300, 42);
    subtitle.setBounds(31, 61, 360, 20);
    waveformLabel.setBounds(42, 122, 180, 24);
    waveform.setBounds(42, 160, 190, 34);

    ampLabel.setBounds(42, 280, 220, 24);
    filterLabel.setBounds(w / 2 - 60, 280, 160, 24);
    motionLabel.setBounds(42, 458, 220, 24);
    outputLabel.setBounds(w - 190, 458, 150, 24);

    const int topY = 310;
    const int knobW = juce::jmax(90, (w - 110) / 6);
    attack.setBounds(42, topY, knobW, 108);
    decay.setBounds(42 + knobW, topY, knobW, 108);
    sustain.setBounds(42 + knobW * 2, topY, knobW, 108);
    release.setBounds(42 + knobW * 3, topY, knobW, 108);
    cutoff.setBounds(42 + knobW * 4, topY, knobW, 108);
    resonance.setBounds(42 + knobW * 5, topY, knobW, 108);

    const int lowerW = (w - 110) / 4;
    lfoRate.setBounds(42, 488, lowerW, 96);
    lfoDepth.setBounds(42 + lowerW, 488, lowerW, 96);
    master.setBounds(w - lowerW - 42, 488, lowerW, 96);
}
