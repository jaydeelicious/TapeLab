#pragma once

#include <cstddef>

class TapeSaturation final
{
public:
	static constexpr float minDriveDb = 0.0f;
	static constexpr float maxDriveDb = 24.0f;

	void setDriveDb(float driveDb) noexcept;

	[[nodiscard]] float getDriveDb() const noexcept;

	[[nodiscard]] float processSample(float input) const noexcept;

	void processBuffer(float* samples, std::size_t numSamples) const noexcept;

private:
	float driveDb_ = 0.0f;
	float driveGain_ = 1.0f;
};