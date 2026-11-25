# Caustic

Caustic is an original, modular polyphonic synthesizer project for VST3 and standalone use, built with C++20, JUCE, and CMake.

The long-term design target is a deep sound-design instrument with the breadth of a modern workstation synthesizer: multiple sound generators, flexible per-layer processing, modulation, sequencing, effects, preset management, and performance controls. Caustic is an independent implementation; it does not include Avenger source code, factory content, or proprietary assets.

## Current foundation

- JUCE-based VST3 and standalone plugin targets
- Polyphonic MIDI voice engine
- Four oscillator waveforms (sine, saw, square, triangle)
- Amp ADSR, resonant low-pass filter, master level
- LFO rate/depth parameters
- Resizable custom editor with a dark spectral theme and ice-blue/orange accents
- CMake build and Windows GitHub Actions build workflow

## Build

Requirements: CMake 3.22+, a C++20 compiler, and Git. JUCE is fetched by CMake from its upstream repository.

```powershell
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release --target Caustic_VST3
```

The generated VST3 bundle is placed under the build tree in the JUCE plugin output directory. Build the `Caustic_Standalone` target to run the standalone application.

## Development roadmap

1. **Core engine:** stable voice allocation, oversampling options, anti-aliased oscillators, modulation routing, and parameter smoothing.
2. **Generator expansion:** wavetable import/editing, sample and multisample playback, granular synthesis, resampling, loop instruments, and drum kits.
3. **Modulation and performance:** assignable envelopes, LFOs, step sequencer, arpeggiator, macro controls, MIDI learn, and key/velocity zones.
4. **Processing:** modular oscillator signal chains, filters, shapers, insert/send/master buses, mixer, and sidechain-capable dynamics.
5. **Workflow:** preset browser, tagging, undo/redo, preset and expansion packaging, automation polish, and scalable editor panels.

The initial commit is a buildable starting point, not a claim that every item on the roadmap is already implemented.