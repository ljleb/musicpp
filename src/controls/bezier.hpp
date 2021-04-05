#ifndef MPP_CONTROLS_BEZIER_HPP
#define MPP_CONTROLS_BEZIER_HPP

#include "steady.hpp"
#include "generator.hpp"

#include "math.hpp"
#include <algorithm>

namespace mpp
{
    template <typename Control>
    struct Bezier
    {
        constexpr Bezier(const Control& min, const Control& max):
            _min { min },
            _max { max }
        {}

        constexpr auto interpolate_control(const TimePoint& time) const&
        {
            return interpolate(
                _min.interpolate_control(time),
                _max.interpolate_control(time),
                time.completion());
        }

    private:
        Control _min;
        Control _max;
    };
}

#endif
