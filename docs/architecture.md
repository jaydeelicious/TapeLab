# TapeLab — Software Architecture

## 1. Overview

TapeLab is a VST3 audio effect plugin for analog-inspired tape saturation built using JUCE and CMake.

## 2. Current Architecture

The current implementation consists of a minimal JUCE plugin and a separate automated testing executable.

```text
                 Ableton Live 12
                        |
                        v
                JUCE VST3 Wrapper
                        |
                        v
              TapeLabAudioProcessor
                        |
                        v
                   processBlock()
                        |
                        v
                Audio Pass-Through
```

### PluginProcessor

**Files:**
- `src/PluginProcessor.h`
- `src/PluginProcessor.cpp`

`TapeLabAudioProcessor` inherits from `juce::AudioProcessor`.

Its responsibilities are:

- Communicating with the plugin host through JUCE.
- Configuring mono and stereo input/output layouts.
- Preparing and releasing audio-processing resources.
- Processing audio buffers.
- Exposing plugin metadata and lifecycle methods.
- Providing plugin state serialization hooks.

Currently, `processBlock()` leaves the incoming buffer unchanged, resulting in audio pass-through.

The processor currently has no custom editor or user-adjustable parameters. State serialization is not yet implemented.

### Audio Lifecycle

The JUCE audio processor exposes three important lifecycle methods:

**`prepareToPlay()`**

Called when the host prepares the processor for playback. Future implementations will initialize sample-rate-dependent DSP state and reserve necessary resources.

**`processBlock()`**

Receives audio and MIDI buffers from the host.

This method runs on the audio processing path and must avoid blocking operations, dynamic memory allocation, and other potentially unbounded work.

The current implementation performs no signal modification.

**`releaseResources()`**

Provides an opportunity to release resources when audio processing is stopped or reconfigured.

## 3. Planned DSP Architecture

The planned audio-processing pipeline is:

```text
Input
  |
  +------------------------------+
  |                              |
  v                              |
Drive                            |
  |                              |
  v                              |
Tape Saturation                  | Dry Path
  |                              |
  v                              |
Tone Filter                      |
  |                              |
  +-------------+----------------+
                |
                v
            Wet/Dry Mix
                |
                v
            Output Gain
                |
                v
              Output
```

The DSP layer will contain three principal components.

### TapeSaturation

Implements the nonlinear transfer function responsible for introducing harmonic distortion.

The first version will use a simple nonlinear waveshaping model.

Advanced saturation, hysteresis, and oversampling are outside the initial MVP.

### ToneFilter

Implements the tonal shaping stage.

The initial design calls for a configurable low-pass filter with smoothing to prevent audible artifacts during parameter changes.

### TapeProcessor

Coordinates the DSP chain, including input drive, saturation, filtering, wet/dry mixing, and output gain.

It will expose methods for initialization, parameter updates, and audio processing.

The actual DSP implementation will be independent of the graphical interface and, where practical, independent of JUCE.

## 4. Planned Parameter Management

The MVP will expose four host-automatable parameters:

| Parameter | Responsibility |
|---|---|
| Drive | Input gain and saturation intensity |
| Tone | Brightness of the processed signal |
| Mix | Wet/dry blending |
| Output Gain | Final level adjustment |

JUCE's `AudioProcessorValueTreeState` is the planned mechanism for parameter storage, UI attachments, and host state integration.

### Initial Saturation Model

TapeLab's first saturation algorithm uses hyperbolic tangent waveshaping:

\[
y[n] = \tanh(g \cdot x[n])
\]

where \(g = 10^{D_{\mathrm{dB}}/20}\) and Drive ranges from 0 to 24 dB.

The algorithm produces smooth soft clipping and introduces predominantly odd-order harmonic distortion for zero-centered sinusoidal signals.

**Limitations:** The initial model does not include oversampling, hysteresis, frequency-dependent saturation, or automatic output gain compensation. It is a tape-inspired waveshaper rather than a physical tape simulation.



### Saturation Validation

The initial tanh() saturation model was validated in Ableton Live 12 using sine-wave analysis. Harmonic generation was observed, with prominent odd harmonics consistent with the symmetric nonlinear transfer function. Mono and stereo processing were also verified. Oversampling remains outside the MVP scope.