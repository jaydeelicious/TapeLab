#include "WetDryMixer.h"

#include <algorithm>
#include <cmath>

void WetDryMixer::setMix(float mix) noexcept
{
	// Reject non-finite parameter values.
	if (!std::isfinite(mix))
		return;

	// Restrict Mix to the supported range.
	mix_ = std::clamp(mix, 0.0f, 1.0f);
}

float WetDryMixer::getMix() const noexcept
{
	return mix_;
}

float WetDryMixer::processSample(float drySample, float wetSample) const noexcept
{
	return (1.0f - mix_) * drySample + mix_ * wetSample;
}

void WetDryMixer::processBuffer(const float* drySamples, float* wetSamples, std::size_t numSamples) const noexcept
{
	for (std::size_t i = 0; i < numSamples; ++i)
	{
		wetSamples[i] = processSample(drySamples[i], wetSamples[i]);
	}
}