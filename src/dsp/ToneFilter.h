#pragma once

#include <cstddef>

class ToneFilter final
{
public:
	static constexpr float minCutoffHz = 500.0f;
	static constexpr float maxCutoffHz = 20000.0f;

	void prepare(double sampleRate) noexcept;
	void reset() noexcept;

	void setCutoffHz(float cutoffHz) noexcept;

	[[nodiscard]] float getCutoffHz() const noexcept;

	[[nodiscard]] float processSample(float input) noexcept;

	void processBuffer(float* samples, std::size_t numSamples) noexcept;

private:
	void updateCoefficient() noexcept;

	double sampleRate_ = 44100.0;

	float cutoffHz_ = maxCutoffHz;
	float coefficient_ = 0.0f;
	float previousOutput_ = 0.0f;
};