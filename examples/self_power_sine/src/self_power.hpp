#ifndef MPP_CONTROLS_SELF_POWER_HPP
#define MPP_CONTROLS_SELF_POWER_HPP

#include <musicpp/controls/steady.hpp>
#include <musicpp/generator.hpp>

#include <musicpp/utils/interpolate.hpp>
#include <algorithm>

namespace mpp
{
    template <typename ScaleControl>
    struct SelfPower
    {
        constexpr SelfPower(ScaleControl const& scale_control):
            _scale_control { scale_control }
        {}

        friend constexpr double interpolate_control(SelfPower const& self, TimePoint const& time)
        {
            auto const& scaled_index { interpolate_control(self._scale_control, time) };
            return std::pow(scaled_index, scaled_index);
        }

        ScaleControl _scale_control;
    };
}

#endif
