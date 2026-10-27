#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include <dsp/TapeSaturation.h>

#include <array>
#include <cmath>

TEST_CASE("Tape saturation preserves silence", "[dsp][saturation]")
{
	const TapeSaturation saturation;

	REQUIRE(saturation.processSample(0.0f) == 0.0f);
}

TEST_CASE("Tape saturation applies tanh waveshaping", "[dsp][saturation]")
{
	const TapeSaturation saturation;

	const float input = 0.5f;
	const float expected = std::tanh(input);

	REQUIRE(saturation.processSample(input) == Catch::Approx(expected).margin(1.0e-6));
}

TEST_CASE("Tape saturation is symmetric around zero", "[dsp][saturation]")
{
	const TapeSaturation saturation;

	const float positive = saturation.processSample(0.75f);
	const float negative = saturation.processSample(-0.75f);

	REQUIRE(positive == Catch::Approx(-negative).margin(1.0e-6));
}

TEST_CASE("Tape saturation processes buffers in place", "[dsp][saturation]")
{
	const TapeSaturation saturation;

	std::array<float, 4> samples{ 0.0f, 0.5f, -0.5f, 1.0f };

	saturation.processBuffer(samples.data(), samples.size());

	REQUIRE(samples[0] == 0.0f);
	REQUIRE(samples[1] == Catch::Approx(std::tanh(0.5f)));
	REQUIRE(samples[2] == Catch::Approx(std::tanh(-0.5f)));
	REQUIRE(samples[3] == Catch::Approx(std::tanh(1.0f)));
}