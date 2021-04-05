#ifndef MPP_GENERATORS_BASIC_HPP
#define MPP_GENERATORS_BASIC_HPP

#include "controls/steady.hpp"

#include "generator.hpp"
#include "project_config.hpp"

#include "math.hpp"
#include <cmath>

namespace mpp
{
    template <typename FrequencyControl>
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

    template <typename FrequencyControl>
    struct BasicSine : public Basic<FrequencyControl>
    {
        constexpr BasicSine(const FrequencyControl& frequency):
            Basic<FrequencyControl> { frequency }
        {}
    };

    template <typename FrequencyControl>
    struct BasicSaw : public Basic<FrequencyControl>
    {
        constexpr BasicSaw(const FrequencyControl& frequency):
            Basic<FrequencyControl> { frequency }
        {}
    };

    template <typename FrequencyControl>
    struct Generator<Sample, BasicSine<FrequencyControl>>
    {
        Sample generate(const TimePoint& time)
        {
            const double& frequency { basic.frequency().interpolate_control(time / 2) };
            return std::sin(2 * M_PI * frequency * time.index / SAMPLE_RATE);
        }

        BasicSine<FrequencyControl>& basic;
    };

    template <typename FrequencyControl>
    struct Generator<Sample, BasicSaw<FrequencyControl>>
    {
        Sample generate(const TimePoint& time)
        {
            const double& frequency { basic.frequency().interpolate_control(time / 2) };
            return std::fmod(2 * frequency * time.index / SAMPLE_RATE, 2) - 1;
        }

        BasicSaw<FrequencyControl>& basic;
    };
}

#endif
