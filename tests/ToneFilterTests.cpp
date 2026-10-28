#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include <dsp/ToneFilter.h>

#include <array>
#include <cmath>
#include <limits>
#include <numbers>

namespace
{
    double measureOutputRms(double frequencyHz, float cutoffHz)
    {
        constexpr double sampleRate = 44100.0;
        constexpr int totalSamples = 44100;
        constexpr int warmupSamples = 4096;

        ToneFilter filter;
        filter.prepare(sampleRate);
        filter.setCutoffHz(cutoffHz);

        double sumSquares = 0.0;

        for (int i = 0; i < totalSamples; ++i)
        {
            const double phase = 2.0 * std::numbers::pi * frequencyHz * i / sampleRate;

            const float input = static_cast<float>(std::sin(phase));
            const float output = filter.processSample(input);

            if (i >= warmupSamples)
                sumSquares += static_cast<double>(output) * output;
        }

        return std::sqrt(sumSquares / static_cast<double>(totalSamples - warmupSamples));
    }
}

// Test 1: Silence preservation

TEST_CASE("Tone filter preserves silence", "[dsp][tone]")
{
	ToneFilter filter;
	filter.prepare(44100.0);
	filter.setCutoffHz(5000.0f);

	for (int i = 0; i < 100; ++i)
	{
		REQUIRE(filter.processSample(0.0f) == 0.0f);
	}
}

// Test 2: Cutoff parameter boundaries

TEST_CASE("Tone filter clamps cutoff frequency", "[dsp][tone]")
{
	ToneFilter filter;
	filter.prepare(44100.0);

	filter.setCutoffHz(100.0f);
	REQUIRE(filter.getCutoffHz() == 500.0f);

	filter.setCutoffHz(25000.0f);
	REQUIRE(filter.getCutoffHz() == 20000.0f);
}

// Test 3: Invalid cutoff values

TEST_CASE("Tone filter rejects invalid cutoff values", "[dsp][tone]")
{
    ToneFilter filter;
    filter.prepare(44100.0);
    filter.setCutoffHz(5000.0f);

    filter.setCutoffHz(std::numeric_limits<float>::quiet_NaN());
    REQUIRE(filter.getCutoffHz() == 5000.0f);

    filter.setCutoffHz(std::numeric_limits<float>::infinity());
    REQUIRE(filter.getCutoffHz() == 5000.0f);

    filter.setCutoffHz(-std::numeric_limits<float>::infinity());
    REQUIRE(filter.getCutoffHz() == 5000.0f);
}

// Test 4: Step response

TEST_CASE("Tone filter responds gradually to a step", "[dsp][tone]")
{
    ToneFilter filter;
    filter.prepare(44100.0);
    filter.setCutoffHz(1000.0f);

    const float first = filter.processSample(1.0f);
    const float second = filter.processSample(1.0f);
    const float third = filter.processSample(1.0f);

    REQUIRE(first > 0.0f);
    REQUIRE(first < 1.0f);
    REQUIRE(second > first);
    REQUIRE(third > second);
    REQUIRE(third < 1.0f);
}

// Test 5: Reset clears filter state

TEST_CASE("Tone filter reset clears previous output", "[dsp][tone]")
{
    ToneFilter filter;
    filter.prepare(44100.0);
    filter.setCutoffHz(1000.0f);

    for (int i = 0; i < 100; ++i)
        static_cast<void>(filter.processSample(1.0f));

    filter.reset();

    REQUIRE(filter.processSample(0.0f) == 0.0f);
}

TEST_CASE("Tone filter buffer processing matches sample processing",
    "[dsp][tone]")
{
    ToneFilter sampleFilter;
    ToneFilter bufferFilter;

    sampleFilter.prepare(44100.0);
    bufferFilter.prepare(44100.0);

    sampleFilter.setCutoffHz(2000.0f);
    bufferFilter.setCutoffHz(2000.0f);

    std::array<float, 6> samples{
        0.0f, 0.5f, 1.0f, -0.5f, -1.0f, 0.25f
    };

    const auto original = samples;

    bufferFilter.processBuffer(samples.data(), samples.size());

    for (std::size_t i = 0; i < samples.size(); ++i)
    {
        const float expected = sampleFilter.processSample(original[i]);

        REQUIRE(samples[i] == Catch::Approx(expected).margin(1.0e-6));
    }
}

TEST_CASE("Tone filter preserves state across buffers",
    "[dsp][tone]")
{
    ToneFilter continuousFilter;
    ToneFilter blockFilter;

    continuousFilter.prepare(44100.0);
    blockFilter.prepare(44100.0);

    continuousFilter.setCutoffHz(1000.0f);
    blockFilter.setCutoffHz(1000.0f);

    std::array<float, 8> continuous{
        1.0f, 0.5f, 0.0f, -0.5f,
        -1.0f, -0.5f, 0.0f, 0.5f
    };

    auto blocks = continuous;

    continuousFilter.processBuffer(
        continuous.data(), continuous.size());

    blockFilter.processBuffer(blocks.data(), 3);
    blockFilter.processBuffer(blocks.data() + 3, 5);

    for (std::size_t i = 0; i < continuous.size(); ++i)
    {
        REQUIRE(blocks[i]
            == Catch::Approx(continuous[i]).margin(1.0e-6));
    }
}

TEST_CASE("Tone filter adapts to sample rate changes",
    "[dsp][tone]")
{
    ToneFilter filter44k;
    ToneFilter filter96k;

    filter44k.prepare(44100.0);
    filter96k.prepare(96000.0);

    filter44k.setCutoffHz(1000.0f);
    filter96k.setCutoffHz(1000.0f);

    const float output44k = filter44k.processSample(1.0f);
    const float output96k = filter96k.processSample(1.0f);

    REQUIRE(output44k > output96k);
    REQUIRE(output44k < 1.0f);
    REQUIRE(output96k > 0.0f);
}

TEST_CASE("Tone filter attenuates high frequencies more than low frequencies",
    "[dsp][tone][frequency]")
{
    constexpr float cutoffHz = 2000.0f;

    const double lowFrequencyRms =
        measureOutputRms(200.0, cutoffHz);

    const double highFrequencyRms =
        measureOutputRms(10000.0, cutoffHz);

    REQUIRE(lowFrequencyRms > highFrequencyRms);
    REQUIRE(lowFrequencyRms > 0.5);
    REQUIRE(highFrequencyRms < 0.2);
}

