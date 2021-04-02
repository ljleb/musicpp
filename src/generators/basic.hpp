#ifndef MPP_GENERATORS_BASIC_HPP
#define MPP_GENERATORS_BASIC_HPP

#include "frequency.hpp"
#include "generator.hpp"
#include "project_config.hpp"

#include "math.hpp"
#include <cmath>

namespace mpp
{
    enum BasicShape
    {
        SINE = 0,
        SAW,
    };

    template <BasicShape basic_shape>
    struct Basic
    {
        constexpr Basic(Frequency&& frequency):
            _frequency { std::move(frequency) }
        {}

        constexpr const Frequency& frequency() const&
        {
            return _frequency;
        }

    private:
        Frequency _frequency;
    };

    template <BasicShape shape>
    constexpr Basic<shape> interpolate(const Basic<shape>& min, const Basic<shape>& max, const double& ratio)
    {
        return { interpolate(min.frequency(), max.frequency(), ratio) };
    }

    template <>
    struct Generator<Sample, Basic<SINE>>
    {
        Sample generate(const uint64_t& index, const uint64_t& max_index)
        {
            return std::sin(2 * M_PI * sine_shape.frequency() * index / SAMPLE_RATE);
        }

        Basic<SINE>& sine_shape;
    };

    template <>
    struct Generator<Sample, Basic<SAW>>
    {
        Sample generate(const uint64_t& index, const uint64_t& max_index)
        {
            return std::fmod(2 * saw_shape.frequency() * index / SAMPLE_RATE, 2) - 1;
        }

        Basic<SAW>& saw_shape;
    };
}

#endif
