# Caustic development roadmap

This roadmap translates the feature families documented in the Avenger 2.4.1 manual into an independent Caustic implementation plan. It is a design target, not a statement that these features already exist.

## Generator architecture

- [x] Initial virtual-analog oscillator (sine, saw, square, triangle)
- [ ] Freeform single-cycle waveform editor and imported waveforms
- [ ] Wavetable playback, frame scanning, import and editing
- [ ] Resample/granular playback engine
- [ ] Single and multisample playback, including layered sample stacks
- [ ] Multi-loop instrument
- [ ] Drum instrument and dedicated drum pattern editor
- [ ] Generator-specific phase, gain, key tracking and micro-fade controls

## Voice and modulation

- [x] Polyphonic MIDI voice allocation
- [x] Basic amplitude envelope
- [ ] Dedicated pitch, filter and freely assignable envelopes
- [ ] Multiple LFOs and modulation matrix
- [ ] Drag/drop modulation assignment
- [ ] Velocity, key tracking, mod wheel, aftertouch and MIDI learn
- [ ] Macro knobs and switches
- [ ] Arpeggiator, chorder and step sequencer
- [ ] Key and velocity zones

## Processing and routing

- [x] Initial low-pass tone control
- [ ] Expanded filter models and serial/parallel filter routing
- [ ] Oscillator-level shaper/distortion and EQ
- [ ] Editable per-generator processing graph
- [ ] Four insert buses with up to eight serial effect slots each
- [ ] Eight parallel send effects
- [ ] Eight-slot master effects rack
- [ ] Flexible bus routing and master bypass
- [ ] Mixer, metering and per-generator levels

## Presets and workflow

- [x] Host state serialization for current parameters
- [ ] Full preset save/load and browser
- [ ] Tagging, favorites and search
- [ ] Undo/redo
- [ ] Reusable presets for effects, modulation and patterns
- [ ] Expansion packaging and sample asset management
- [ ] MIDI controller mapping persistence

## Quality

- [ ] Parameter smoothing and denormal protection
- [ ] Oversampling and aliasing tests
- [ ] Automated DSP unit tests
- [ ] CPU profiling and stress tests
- [ ] Cross-platform CI (Windows, macOS, Linux)
- [ ] Accessibility, scalable UI and host compatibility testing
