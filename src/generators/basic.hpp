#ifndef MPP_GENERATORS_BASIC_HPP
#define MPP_GENERATORS_BASIC_HPP

#include "controls/steady.hpp"

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

    template <BasicShape shape, typename FrequencyControl>
    struct Basic
    {
        constexpr Basic(const FrequencyControl& frequency):
            _frequency { frequency }
        {}

        constexpr const FrequencyControl& frequency() const&
        {
            return _frequency;
        }

    private:
        FrequencyControl _frequency;
    };

    template <BasicShape shape, typename FrequencyControl>
    Basic<shape, FrequencyControl> make_basic(FrequencyControl&& frequency)
    {
        return { std::forward<FrequencyControl>(frequency) };
    }

    template <typename FrequencyControl>
    struct Generator<Sample, Basic<SINE, FrequencyControl>>
    {
        Sample generate(const TimePoint& time)
        {
            const double& frequency { interpolate_control(basic.frequency(), time / 2) };
            return std::sin(2 * M_PI * frequency * time.index / SAMPLE_RATE);
        }

        Basic<SINE, FrequencyControl>& basic;
    };

    template <typename FrequencyControl>
    struct Generator<Sample, Basic<SAW, FrequencyControl>>
    {
        Sample generate(const TimePoint& time)
        {
            const double& frequency { interpolate_control(basic.frequency(), time / 2) };
            return std::fmod(2 * frequency * time.index / SAMPLE_RATE, 2) - 1;
        }

        Basic<SAW, FrequencyControl>& basic;
    };
}

#endif
