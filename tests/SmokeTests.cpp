#include <catch2/catch_test_macros.hpp>

#include <array>
#include <numeric>

TEST_CASE("Basic C++20 test infrastructure works", "[smoke]")
{
	constexpr std::array<float, 4> samples {
		0.25f,
		-0.5f,
		0.75f,
		-1.0f
	};

	const auto sum = std::accumulate(samples.begin(), samples.end(), 0.0f);

	REQUIRE(sum == -0.5f);
}