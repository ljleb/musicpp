#ifndef MPP_CONTROLS_STEADY_HPP
#define MPP_CONTROLS_STEADY_HPP

#include "generator.hpp"

#include "math.hpp"
#include <cmath>

namespace mpp
{
    template <typename Value>
    struct Steady
    {
        constexpr Steady(const Value& initial_value):
            _value { initial_value }
        {}

        constexpr Value interpolate_control(const TimePoint& time) const&
        {
            return _value;
        }

    private:
        Value _value;
    };
}

#endif
