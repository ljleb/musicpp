#ifndef MPP_CONTROLS_STEADY_HPP
#define MPP_CONTROLS_STEADY_HPP

#include "generator.hpp"

#include "math.hpp"
#include <cmath>

namespace mpp
{
    template <typename Value>
    constexpr std::enable_if_t<std::is_arithmetic_v<Value>, Value> interpolate_control(const Value& self, const TimePoint& time)
    {
        return self;
    }
}

#endif
