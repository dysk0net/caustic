#include "PluginProcessor.h"
#include "PluginEditor.h"

CausticAudioProcessor::CausticAudioProcessor()
    : AudioProcessor(BusesProperties().withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      parameters(*this, nullptr, "CAUSTIC_STATE", createParameterLayout())
{
}

juce::AudioProcessorValueTreeState::ParameterLayout CausticAudioProcessor::createParameterLayout()
{
    using namespace juce;
    AudioProcessorValueTreeState::ParameterLayout layout;
    layout.add(std::make_unique<AudioParameterChoice>("waveform", "Waveform",
        StringArray { "Sine", "Saw", "Square", "Triangle" }, 1));
    layout.add(std::make_unique<AudioParameterFloat>("attack", "Attack",
        NormalisableRange<float>(0.001f, 5.0f, 0.001f, 0.35f), 0.015f));
    layout.add(std::make_unique<AudioParameterFloat>("decay", "Decay",
        NormalisableRange<float>(0.005f, 5.0f, 0.001f, 0.35f), 0.25f));
    layout.add(std::make_unique<AudioParameterFloat>("sustain", "Sustain",
        NormalisableRange<float>(0.0f, 1.0f, 0.001f), 0.75f));
    layout.add(std::make_unique<AudioParameterFloat>("release", "Release",
        NormalisableRange<float>(0.005f, 8.0f, 0.001f, 0.35f), 0.35f));
    layout.add(std::make_unique<AudioParameterFloat>("cutoff", "Cutoff",
        NormalisableRange<float>(30.0f, 18000.0f, 1.0f, 0.35f), 12000.0f));
    layout.add(std::make_unique<AudioParameterFloat>("resonance", "Resonance",
        NormalisableRange<float>(0.1f, 1.5f, 0.001f), 0.2f));
    layout.add(std::make_unique<AudioParameterFloat>("lfoRate", "LFO Rate",
        NormalisableRange<float>(0.05f, 20.0f, 0.01f, 0.5f), 2.0f));
    layout.add(std::make_unique<AudioParameterFloat>("lfoDepth", "LFO Depth",
        NormalisableRange<float>(0.0f, 1.0f, 0.001f), 0.0f));
    layout.add(std::make_unique<AudioParameterFloat>("master", "Master",
        NormalisableRange<float>(0.0f, 1.0f, 0.001f), 0.8f));
    return layout;
}

void CausticAudioProcessor::prepareToPlay(double sampleRate, int)
{
    currentSampleRate = sampleRate;
    filterStateL = filterStateR = 0.0f;
    for (auto& voice : voices)
        voice = {};
}

void CausticAudioProcessor::releaseResources()
{
    for (auto& voice : voices)
        voice = {};
}

bool CausticAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    const auto output = layouts.getMainOutputChannelSet();
    return output == juce::AudioChannelSet::mono() || output == juce::AudioChannelSet::stereo();
}

void CausticAudioProcessor::noteOn(int note, float velocity)
{
    auto* target = &voices.front();
    for (auto& voice : voices)
        if (!voice.active) { target = &voice; break; }
    if (target->active)
        for (auto& voice : voices)
            if (voice.note == note) { target = &voice; break; }

    target->active = true;
    target->note = note;
    target->velocity = velocity;
    target->phase = 0.0;
    target->increment = juce::MidiMessage::getMidiNoteInHertz(note) / currentSampleRate;
    target->envelope = 0.0f;
    target->releasing = false;
}

void CausticAudioProcessor::noteOff(int note)
{
    for (auto& voice : voices)
        if (voice.active && voice.note == note)
            voice.releasing = true;
}

float CausticAudioProcessor::oscillatorSample(double phase, int waveform)
{
    const auto p = static_cast<float>(phase);
    switch (waveform)
    {
        case 0: return std::sin(juce::MathConstants<float>::twoPi * p);
        case 1: return 2.0f * p - 1.0f;
        case 2: return p < 0.5f ? 1.0f : -1.0f;
        case 3: return 1.0f - 4.0f * std::abs(p - 0.5f);
        default: return 0.0f;
    }
}

void CausticAudioProcessor::renderVoice(Voice& voice, int startSample, int numSamples,
    juce::AudioBuffer<float>& buffer, float attack, float decay, float sustain,
    float release, int waveform)
{
    auto* left = buffer.getWritePointer(0);
    auto* right = buffer.getNumChannels() > 1 ? buffer.getWritePointer(1) : nullptr;
    const auto attackStep = 1.0f / juce::jmax(1.0f, attack * static_cast<float>(currentSampleRate));
    const auto releaseStep = 1.0f / juce::jmax(1.0f, release * static_cast<float>(currentSampleRate));
    const auto decayStep = (1.0f - sustain) / juce::jmax(1.0f, decay * static_cast<float>(currentSampleRate));

    for (int i = startSample; i < startSample + numSamples && voice.active; ++i)
    {
        if (voice.releasing)
        {
            voice.envelope -= releaseStep;
            if (voice.envelope <= 0.00001f) { voice.envelope = 0.0f; voice.active = false; break; }
        }
        else if (voice.envelope < 1.0f)
            voice.envelope = juce::jmin(1.0f, voice.envelope + attackStep);
        else
            voice.envelope = juce::jmax(sustain, voice.envelope - decayStep);

        const auto sample = oscillatorSample(voice.phase, waveform) * voice.envelope * voice.velocity * 0.18f;
        left[i] += sample;
        if (right != nullptr) right[i] += sample;
        voice.phase += voice.increment;
        voice.phase -= std::floor(voice.phase);
    }
}

void CausticAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midi)
{
    juce::ScopedNoDenormals noDenormals;
    buffer.clear();
    const auto attack = parameters.getRawParameterValue("attack")->load();
    const auto decay = parameters.getRawParameterValue("decay")->load();
    const auto sustain = parameters.getRawParameterValue("sustain")->load();
    const auto release = parameters.getRawParameterValue("release")->load();
    const auto waveform = static_cast<int>(parameters.getRawParameterValue("waveform")->load());
    const auto cutoff = parameters.getRawParameterValue("cutoff")->load();
    const auto master = parameters.getRawParameterValue("master")->load();

    int cursor = 0;
    for (const auto metadata : midi)
    {
        const auto position = metadata.samplePosition;
        if (position > cursor)
            for (auto& voice : voices)
                renderVoice(voice, cursor, position - cursor, buffer, attack, decay, sustain, release, waveform);

        const auto message = metadata.getMessage();
        if (message.isNoteOn()) noteOn(message.getNoteNumber(), message.getFloatVelocity());
        else if (message.isNoteOff()) noteOff(message.getNoteNumber());
        else if (message.isAllNotesOff() || message.isAllSoundOff())
            for (auto& voice : voices) voice.releasing = true;
        cursor = position;
    }

    if (cursor < buffer.getNumSamples())
        for (auto& voice : voices)
            renderVoice(voice, cursor, buffer.getNumSamples() - cursor, buffer, attack, decay, sustain, release, waveform);

    const auto coefficient = 1.0f - std::exp(-juce::MathConstants<float>::twoPi * cutoff / static_cast<float>(currentSampleRate));
    auto* left = buffer.getWritePointer(0);
    auto* right = buffer.getNumChannels() > 1 ? buffer.getWritePointer(1) : nullptr;
    for (int i = 0; i < buffer.getNumSamples(); ++i)
    {
        filterStateL += coefficient * (left[i] - filterStateL);
        left[i] = filterStateL * master;
        if (right != nullptr)
        {
            filterStateR += coefficient * (right[i] - filterStateR);
            right[i] = filterStateR * master;
        }
    }
}

juce::AudioProcessorEditor* CausticAudioProcessor::createEditor()
{
    return new CausticAudioProcessorEditor(*this);
}

void CausticAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    const auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void CausticAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xml(getXmlFromBinary(data, sizeInBytes));
    if (xml != nullptr && xml->hasTagName(parameters.state.getType()))
        parameters.replaceState(juce::ValueTree::fromXml(*xml));
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new CausticAudioProcessor();
}
