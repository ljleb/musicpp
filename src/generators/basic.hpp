#ifndef MPP_GENERATORS_BASIC_HPP
#define MPP_GENERATORS_BASIC_HPP

#include "generator.hpp"
#include "project_config.hpp"

#include <cmath>

namespace mpp
{
    struct Frequency
    {
        constexpr Frequency(const float& frequency):
            _frequency(frequency)
        {}

        constexpr operator float() const&
        {
            return _frequency;
        }

    private:
        float _frequency;
    };

    template <>
    struct Generator<SINE, Sample, Frequency>
    {
        Sample generate(const uint64_t& index, const uint64_t& max_index) const&
        {
            return std::sin(2 * M_PI * frequency * index / SAMPLE_RATE);
        }

        Frequency frequency;
    };

    template <>
    struct Generator<SAW, Sample, Frequency>
    {
        Sample generate(const uint64_t& index, const uint64_t& max_index) const&
        {
            return std::fmod(2 * frequency * index / SAMPLE_RATE, 2) - 1;
        }

        Frequency frequency;
    };
}

#endif
