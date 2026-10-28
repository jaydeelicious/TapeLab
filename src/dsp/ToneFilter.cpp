#include "ToneFilter.h"

#include <algorithm>
#include <cmath>
#include <numbers>

void ToneFilter::prepare(double sampleRate) noexcept
{
	// Preserve the previous sample rate if the new one is invalid.
	if (!std::isfinite(sampleRate) || sampleRate <= 0.0)
		return;

	sampleRate_ = sampleRate;

	updateCoefficient();
	reset();
}

void ToneFilter::reset() noexcept
{
	previousOutput_ = 0.0f;
}

void ToneFilter::setCutoffHz(float cutoffHz) noexcept
{
	// Ignore non-finite parameter values.
	if (!std::isfinite(cutoffHz))
		return;

	cutoffHz_ = std::clamp(cutoffHz, minCutoffHz, maxCutoffHz);

	updateCoefficient();
}

float ToneFilter::getCutoffHz() const noexcept
{
	return cutoffHz_;
}

float ToneFilter::processSample(float input) noexcept
{
	const float output = (1.0f - coefficient_) * input + coefficient_ * previousOutput_;

	previousOutput_ = output;

	return output;
}

void ToneFilter::processBuffer(float* samples, std::size_t numSamples) noexcept
{
	for (std::size_t i = 0; i < numSamples; ++i)
	{
		samples[i] = processSample(samples[i]);
	}
}

void ToneFilter::updateCoefficient() noexcept
{
	// Keep the effective cutoff below the Nyquist frequency.
	const double nyquist = sampleRate_ * 0.5;

	const double effectiveCutoff = std::min(static_cast<double>(cutoffHz_), nyquist * 0.99);

	const double exponent = -2.0 * std::numbers::pi * effectiveCutoff / sampleRate_;

	coefficient_ = static_cast<float>(std::exp(exponent));
}