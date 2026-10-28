
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include <dsp/WetDryMixer.h>

#include <array>
#include <cmath>
#include <limits>

TEST_CASE("WetDryMixer defaults to fully wet", "[dsp][mix]")
{
    const WetDryMixer mixer;

    REQUIRE(mixer.getMix() == 1.0f);
    REQUIRE(mixer.processSample(0.2f, 0.8f) == 0.8f);
}

TEST_CASE("WetDryMixer supports dry wet and equal mixing", "[dsp][mix]")
{
    WetDryMixer mixer;

    constexpr float dry = 0.2f;
    constexpr float wet = 0.8f;

    mixer.setMix(0.0f);
    REQUIRE(mixer.processSample(dry, wet) == Catch::Approx(0.2f));

    mixer.setMix(0.5f);
    REQUIRE(mixer.processSample(dry, wet) == Catch::Approx(0.5f));

    mixer.setMix(1.0f);
    REQUIRE(mixer.processSample(dry, wet) == Catch::Approx(0.8f));
}

TEST_CASE("WetDryMixer clamps mix to valid range", "[dsp][mix]")
{
    WetDryMixer mixer;

    mixer.setMix(-0.5f);
    REQUIRE(mixer.getMix() == 0.0f);

    mixer.setMix(1.5f);
    REQUIRE(mixer.getMix() == 1.0f);
}

TEST_CASE("WetDryMixer rejects non-finite mix values", "[dsp][mix]")
{
    WetDryMixer mixer;
    mixer.setMix(0.5f);

    mixer.setMix(std::numeric_limits<float>::quiet_NaN());
    REQUIRE(mixer.getMix() == 0.5f);

    mixer.setMix(std::numeric_limits<float>::infinity());
    REQUIRE(mixer.getMix() == 0.5f);

    mixer.setMix(-std::numeric_limits<float>::infinity());
    REQUIRE(mixer.getMix() == 0.5f);
}

TEST_CASE("WetDryMixer preserves identical signals", "[dsp][mix]")
{
    WetDryMixer mixer;

    for (float mix : { 0.0f, 0.25f, 0.5f, 0.75f, 1.0f })
    {
        mixer.setMix(mix);

        REQUIRE(mixer.processSample(0.5f, 0.5f)
            == Catch::Approx(0.5f));
    }
}

TEST_CASE("WetDryMixer handles empty buffers", "[dsp][mix]")
{
    const WetDryMixer mixer;

    mixer.processBuffer(nullptr, nullptr, 0);

    SUCCEED();
}


TEST_CASE("WetDryMixer buffer matches sample processing", "[dsp][mix]")
{
    WetDryMixer mixer;
    mixer.setMix(0.35f);

    const std::array<float, 4> dry{
        0.1f, 0.2f, -0.3f, -0.4f
    };

    std::array<float, 4> wet{
        0.8f, -0.6f, 0.5f, -0.2f
    };

    const auto originalWet = wet;

    mixer.processBuffer(dry.data(), wet.data(), wet.size());

    for (std::size_t i = 0; i < wet.size(); ++i)
    {
        const float expected =
            mixer.processSample(dry[i], originalWet[i]);

        REQUIRE(wet[i] ==
            Catch::Approx(expected).margin(1.0e-6));
    }
}

TEST_CASE("WetDryMixer preserves dry buffer", "[dsp][mix]")
{
    WetDryMixer mixer;
    mixer.setMix(0.5f);

    const std::array<float, 4> dry{
        0.1f, 0.2f, 0.3f, 0.4f
    };

    std::array<float, 4> wet{
        0.8f, 0.7f, 0.6f, 0.5f
    };

    const auto originalDry = dry;

    mixer.processBuffer(dry.data(), wet.data(), wet.size());

    REQUIRE(dry == originalDry);
}

TEST_CASE("WetDryMixer supports fully dry and wet buffers", "[dsp][mix]")
{
    WetDryMixer mixer;

    const std::array<float, 4> dry{
        0.1f, -0.2f, 0.3f, -0.4f
    };

    const std::array<float, 4> originalWet{
        0.8f, 0.7f, -0.6f, -0.5f
    };

    auto wet = originalWet;

    mixer.setMix(0.0f);
    mixer.processBuffer(dry.data(), wet.data(), wet.size());

    for (std::size_t i = 0; i < wet.size(); ++i)
        REQUIRE(wet[i] == Catch::Approx(dry[i]));

    wet = originalWet;

    mixer.setMix(1.0f);
    mixer.processBuffer(dry.data(), wet.data(), wet.size());

    for (std::size_t i = 0; i < wet.size(); ++i)
        REQUIRE(wet[i] == Catch::Approx(originalWet[i]));
}

TEST_CASE("WetDryMixer processes consecutive buffers consistently",
    "[dsp][mix]")
{
    WetDryMixer mixer;
    mixer.setMix(0.6f);

    const std::array<float, 6> dry{
        0.1f, 0.2f, 0.3f, -0.1f, -0.2f, -0.3f
    };

    const std::array<float, 6> wet{
        0.6f, 0.5f, 0.4f, -0.4f, -0.5f, -0.6f
    };

    auto continuous = wet;
    auto blocks = wet;

    mixer.processBuffer(
        dry.data(), continuous.data(), continuous.size());

    mixer.processBuffer(dry.data(), blocks.data(), 2);

    mixer.processBuffer(
        dry.data() + 2, blocks.data() + 2, blocks.size() - 2);

    for (std::size_t i = 0; i < wet.size(); ++i)
    {
        REQUIRE(blocks[i] ==
            Catch::Approx(continuous[i]).margin(1.0e-6));
    }
}
