#include "TapeSaturation.h"

#include <algorithm>
#include <cmath>

void TapeSaturation::setDriveDb(float driveDb) noexcept
{
	// Reject non-finite values to preserve valid DSP state.
	if (!std::isfinite(driveDb))
		return;

	// Restrict Drive to the supported range.
	driveDb_ = std::clamp(driveDb, minDriveDb, maxDriveDb);

	// Convert decibels to a linear amplitude multiplier.
	driveGain_ = std::pow(10.0f, driveDb_ / 20.0f);
}

float TapeSaturation::getDriveDb() const noexcept
{
	return driveDb_;
}

float TapeSaturation::processSample(float input) const noexcept
{
	return std::tanh(driveGain_ * input);
}

void TapeSaturation::processBuffer(float* samples, std::size_t numSamples) const noexcept
{
	for (std::size_t i = 0; i < numSamples; ++i)
	{
		samples[i] = processSample(samples[i]);
	}
}