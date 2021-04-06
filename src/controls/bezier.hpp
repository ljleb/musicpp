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

    friend constexpr auto interpolate_control(const Bezier<MinControl, MaxControl>& self, const TimePoint& time)
    {
        return interpolate(
            static_cast<double>(interpolate_control(self._min, time)),
            static_cast<double>(interpolate_control(self._max, time)),
            time.completion());
    }

    private:
        MinControl _min;
        MaxControl _max;
    };
}

#endif
