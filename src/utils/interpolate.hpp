#ifndef MPP_UTILS_INTERPOLATE_HPP
#define MPP_UTILS_INTERPOLATE_HPP

namespace mpp
{
    template <typename Input>
    constexpr Input interpolate(Input const& min, Input const& max, double const& ratio)
    {
        return min + (max - min) * ratio;
    }
}

#endif
