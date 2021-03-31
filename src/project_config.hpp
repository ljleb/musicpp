#ifndef MPP_PROJECT_CONFIG_HPP
#define MPP_PROJECT_CONFIG_HPP

#include <array>
#include <cstdint>

namespace mpp
{
    constexpr const uint64_t SAMPLE_RATE = 44100;
    using Sample = float;

    constexpr const uint64_t SAMPLES_SIZE = mpp::SAMPLE_RATE * 4;
    using Samples = std::array<mpp::Sample, SAMPLES_SIZE>;
}

#endif
