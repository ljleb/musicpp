#ifndef MPP_CONTROLS_FREQUENCY_HPP
#define MPP_CONTROLS_FREQUENCY_HPP

#include "generator.hpp"

#include "math.hpp"
#include <cmath>

namespace mpp
{
    struct Frequency
    {
        static constexpr Frequency of_key(const double& note)
        {
            return 440 * std::pow(2, (note - 49) / 12);
        }

        constexpr Frequency(double&& frequency):
            _frequency { std::move(frequency) }
        {}

        constexpr operator double() const&
        {
            return _frequency;
        }

    private:
        double _frequency;
    };

    constexpr Frequency interpolate(const Frequency& min, const Frequency& max, const double& ratio)
    {
        return interpolate(double(min), double(max), ratio);
    }
}

#endif
