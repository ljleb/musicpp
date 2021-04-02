#ifndef MPP_MATH_HPP
#define MPP_MATH_HPP

namespace mpp
{
    template <typename Input>
    constexpr Input interpolate(const Input& min, const Input& max, const double& ratio)
    {
        return min + (max - min) * ratio;
    }
}

#endif
