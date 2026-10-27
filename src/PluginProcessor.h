#pragma once

#include <dsp/TapeSaturation.h>

#include <juce_audio_processors/juce_audio_processors.h>

class TapeLabAudioProcessor final : public juce::AudioProcessor
{
public:
	TapeLabAudioProcessor();
	~TapeLabAudioProcessor() override = default;

	// Audio processing lifecycle
	void prepareToPlay(double sampleRate, int samplesPerBlock) override;
	void releaseResources() override;

	bool isBusesLayoutSupported(const BusesLayout& layouts) const override;

	void processBlock(juce::AudioBuffer<float>& buffer,
		juce::MidiBuffer& midiMessages) override;

	// Plugin editor
	bool hasEditor() const override;
	juce::AudioProcessorEditor* createEditor() override;

	// Plugin information 
	const juce::String getName() const override;

	bool acceptsMidi() const override;
	bool producesMidi() const override;
	bool isMidiEffect() const override;

	double getTailLengthSeconds() const override;

	// Program Management
	int getNumPrograms() override;
	int getCurrentProgram() override;
	void setCurrentProgram(int index) override;

	const juce::String getProgramName(int index) override;
	void changeProgramName(int index, const juce::String& newName) override;

	// Plugin state
	void getStateInformation(juce::MemoryBlock& destData) override;
	void setStateInformation(const void* data, int sizeInBytes) override;

private:
	TapeSaturation saturation_;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TapeLabAudioProcessor)
};