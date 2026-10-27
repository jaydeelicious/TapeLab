#include "TapeSaturation.h"

#include <cmath>

float TapeSaturation::processSample(float input) const noexcept
{
	return std::tanh(input);
}

void TapeSaturation::processBuffer(float* samples, std::size_t numSamples) const noexcept
{
	for (std::size_t i = 0; i < numSamples; ++i)
	{
		samples[i] = processSample(samples[i]);
	}
}