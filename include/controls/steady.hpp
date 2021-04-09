#ifndef MPP_CONTROLS_STEADY_HPP
#define MPP_CONTROLS_STEADY_HPP

#include "generator.hpp"

#include <cmath>

namespace mpp
{
    template <typename Value>
    constexpr std::enable_if_t<std::is_arithmetic_v<Value>, Value> interpolate_control(Value const& self, TimePoint const& time)
    {
        return self;
    }
}

#endif
