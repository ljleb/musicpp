#ifndef MPP_CONTROLS_BEZIER_HPP
#define MPP_CONTROLS_BEZIER_HPP

#include "steady.hpp"
#include "generator.hpp"

#include "math.hpp"
#include <algorithm>

namespace mpp
{
    template <typename MinControl, typename MaxControl>
    struct Bezier
    {
        constexpr Bezier(const MinControl& min, const MaxControl& max):
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
        MinControl _min;
        MaxControl _max;
    };
}

#endif
