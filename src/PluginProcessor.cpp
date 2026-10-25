#include "PluginProcessor.h"

// ==============
// Construction

TapeLabAudioProcessor::TapeLabAudioProcessor()
	: AudioProcessor(
		BusesProperties()
		.withInput("Input", juce::AudioChannelSet::stereo(), true)
		.withOutput("Output", juce::AudioChannelSet::stereo(), true))
{
}

// =====================
// Audio lifecycle

void TapeLabAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
	juce::ignoreUnused(sampleRate, samplesPerBlock);

	// TODO: initialize DSP state and filters.
}

void TapeLabAudioProcessor::releaseResources()
{
	// No allocated DSP resources yet.
}

bool TapeLabAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
	const auto input = layouts.getMainInputChannelSet();
	const auto output = layouts.getMainOutputChannelSet();

	if (output != juce::AudioChannelSet::mono()
		&& output != juce::AudioChannelSet::stereo())
	{
		return false;
	}

	return input == output;
}

void TapeLabAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
	juce::ScopedNoDenormals noDenormals;

	juce::ignoreUnused(buffer, midiMessages);

	// Audio pass-through:
	// The input samples are already in the buffer.
	// Leaving the buffer unchanged preserves the signal.
}

// ==========================
// Plugin editor

bool TapeLabAudioProcessor::hasEditor() const
{
	return false;
}

juce::AudioProcessorEditor* TapeLabAudioProcessor::createEditor()
{
	return nullptr;
}

// ========================================
// Plugin information

const juce::String TapeLabAudioProcessor::getName() const
{
	return JucePlugin_Name;
}

bool TapeLabAudioProcessor::acceptsMidi() const
{
	return false;
}

bool TapeLabAudioProcessor::producesMidi() const
{
	return false;
}

bool TapeLabAudioProcessor::isMidiEffect() const
{
	return false;
}

double TapeLabAudioProcessor::getTailLengthSeconds() const
{
	return 0.0;
}

// ============================================================
// Program management

int TapeLabAudioProcessor::getNumPrograms()
{
	return 1;
}

int TapeLabAudioProcessor::getCurrentProgram()
{
	return 0;
}

void TapeLabAudioProcessor::setCurrentProgram(int index)
{
	juce::ignoreUnused(index);
}

const juce::String TapeLabAudioProcessor::getProgramName(int index)
{
	juce::ignoreUnused(index);
	return {};
}

void TapeLabAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
	juce::ignoreUnused(index, newName);
}

// ============================================================================
// Plugin state

void TapeLabAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
	juce::ignoreUnused(destData);

	// TODO: serialize plugin parameters
}

void TapeLabAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
	juce::ignoreUnused(data, sizeInBytes);

	// TODO: restore plugin parameters.
}

// ===============================================================================
// Plugin factory 

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
	return new TapeLabAudioProcessor();
}