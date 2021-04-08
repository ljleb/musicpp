#ifndef MPP_MATH_HPP
#define MPP_MATH_HPP

namespace mpp
{
    template <typename Input>
    constexpr Input interpolate(Input const& min, Input const& max, double const& ratio)
    {
        return min + (max - min) * ratio;
    }
}

#endif
