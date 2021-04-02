#ifndef MPP_GENERATORS_BASIC_HPP
#define MPP_GENERATORS_BASIC_HPP

#include "generator.hpp"
#include "project_config.hpp"

#include <cmath>

namespace mpp
{
    struct Frequency
    {
        constexpr Frequency(float&& frequency):
            _frequency(std::move(frequency))
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
        constexpr Generator(Frequency& frequency):
            _frequency { frequency }
        {}

        Sample generate(const uint64_t& index, const uint64_t& max_index)
        {
            return std::sin(2 * M_PI * _frequency * index / SAMPLE_RATE);
        }

        Frequency& _frequency;
    };

    template <>
    struct Generator<SAW, Sample, Frequency>
    {
        constexpr Generator(Frequency& frequency):
            _frequency { frequency }
        {}

        Sample generate(const uint64_t& index, const uint64_t& max_index)
        {
            return std::fmod(2 * _frequency * index / SAMPLE_RATE, 2) - 1;
        }

        Frequency& _frequency;
    };
}

#endif
