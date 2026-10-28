#pragma once

#include <cstddef>

class WetDryMixer final
{
public:
	void setMix(float mix) noexcept;

	[[nodiscard]] float getMix() const noexcept;

	[[nodiscard]] float processSample(float drySample, float wetSample) const noexcept;

	void processBuffer(const float* drySamples, float* wetSamples, std::size_t numSamples) const noexcept;

private:
	float mix_ = 1.0f;
};