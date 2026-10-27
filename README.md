# TapeLab

TapeLab is a VST3 audio effect plugin that emulates analog tape saturation.

Built with **C++20**, **JUCE**, and **CMake**, TapeLab is designed to run in Ableton Live 12 on Windows.

## MVP Features

- Drive — Controls saturation intensity
- Tone — Adjusts tonal brightness
- Mix — Blends processed and original audio
- Output Gain — Adjusts the final output level

## Technology Stack

- C++20
- JUCE
- CMake
- VST3

## Project Status

In development — v0.1.0 MVP.

## Build Instructions

Clone the repository:

```powershell
git clone --recurse-submodules https://github.com/jaydeelicious/TapeLab.git
cd TapeLab
```

Configure the project:

```powershell
cmake -S . -B build -G "Visual Studio 17 2022" -A x64 -DBUILD_TESTING=ON
```

Build the VST3 plugin and automated tests:

```powershell
cmake --build build --config Release --target TapeLab_VST3 TapeLabTests
```

The plugin bundle is generated at:

```text
build/TapeLab_artefacts/Release/VST3/TapeLab.vst3
```

## Testing

Run automated tests using CTest:

```powershell
ctest --test-dir build -C Release --output-on-failure
```

## Architecture

See [Architecture Documentation](docs/architecture.md) for component responsibilities, audio lifecycle, testing strategy, and planned DSP design.

## License

JUCE and other dependencies are subject to their respective licenses.