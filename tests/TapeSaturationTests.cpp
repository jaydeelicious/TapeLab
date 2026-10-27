#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include <dsp/TapeSaturation.h>

#include <array>
#include <cmath>
#include <limits>

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


TEST_CASE("Drive defaults to 0 dB", "[dsp][drive]")
{
    const TapeSaturation saturation;

    REQUIRE(saturation.getDriveDb() == 0.0f);
}

TEST_CASE("Drive applies correct gain before saturation", "[dsp][drive]")
{
    TapeSaturation saturation;

    constexpr float input = 0.25f;

    for (const float driveDb : { 0.0f, 6.0f, 12.0f, 24.0f })
    {
        saturation.setDriveDb(driveDb);

        const float gain = std::pow(10.0f, driveDb / 20.0f);
        const float expected = std::tanh(gain * input);

        REQUIRE(saturation.getDriveDb() == driveDb);
        REQUIRE(saturation.processSample(input)
            == Catch::Approx(expected).margin(1.0e-6));
    }
}

TEST_CASE("Drive is clamped to the supported range", "[dsp][drive]")
{
    TapeSaturation saturation;

    saturation.setDriveDb(-10.0f);
    REQUIRE(saturation.getDriveDb() == 0.0f);

    saturation.setDriveDb(30.0f);
    REQUIRE(saturation.getDriveDb() == 24.0f);
}

TEST_CASE("Drive rejects non-finite parameter values", "[dsp][drive]")
{
    TapeSaturation saturation;
    saturation.setDriveDb(6.0f);

    const float previousOutput = saturation.processSample(0.5f);

    saturation.setDriveDb(std::numeric_limits<float>::quiet_NaN());
    REQUIRE(saturation.getDriveDb() == 6.0f);

    saturation.setDriveDb(std::numeric_limits<float>::infinity());
    REQUIRE(saturation.getDriveDb() == 6.0f);

    saturation.setDriveDb(-std::numeric_limits<float>::infinity());
    REQUIRE(saturation.getDriveDb() == 6.0f);

    REQUIRE(saturation.processSample(0.5f)
        == Catch::Approx(previousOutput));
}

TEST_CASE("Increasing Drive increases saturation magnitude", "[dsp][drive]")
{
    TapeSaturation saturation;

    constexpr float input = 0.25f;

    saturation.setDriveDb(0.0f);
    const float lowDrive = saturation.processSample(input);

    saturation.setDriveDb(12.0f);
    const float highDrive = saturation.processSample(input);

    REQUIRE(std::abs(highDrive) > std::abs(lowDrive));
}

TEST_CASE("Buffer processing respects Drive setting", "[dsp][drive]")
{
    TapeSaturation saturation;
    saturation.setDriveDb(12.0f);

    std::array<float, 4> samples{
        0.0f, 0.25f, -0.5f, 1.0f
    };

    const auto original = samples;

    saturation.processBuffer(samples.data(), samples.size());

    for (std::size_t i = 0; i < samples.size(); ++i)
    {
        REQUIRE(samples[i]
            == Catch::Approx(
                saturation.processSample(original[i])
            ).margin(1.0e-6));
    }
}
