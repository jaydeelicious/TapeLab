#pragma once

#include <cstddef>

class TapeSaturation final
{
public:
	[[nodiscard]] float processSample(float input) const noexcept;

	void processBuffer(float* samples, std::size_t numSamples) const noexcept;
};