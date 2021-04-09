#ifndef MPP_CONTROLS_BEZIER_HPP
#define MPP_CONTROLS_BEZIER_HPP

#include "steady.hpp"
#include <musicpp/generator.hpp>

#include <musicpp/utils/interpolate.hpp>
#include <algorithm>

namespace mpp
{
    template <typename MinControl, typename MaxControl>
    struct Bezier
    {
        constexpr Bezier(MinControl const& min, MaxControl const& max):
            _min { min },
            _max { max }
        {}

        template <typename A>
        constexpr std::enable_if_t<std::is_arithmetic_v<A>, Bezier<MinControl, MaxControl>> operator/(A const& that) const&
        {
            return {
                _min / 2,
                _max / 2,
            };
        }

        template <typename A>
        constexpr std::enable_if_t<std::is_arithmetic_v<A>, Bezier<MinControl, MaxControl>> operator*(A const& that) const&
        {
            return {
                _min * 2,
                _max * 2,
            };
        }

        friend constexpr double interpolate_control(Bezier<MinControl, MaxControl> const& self, TimePoint const& time)
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
