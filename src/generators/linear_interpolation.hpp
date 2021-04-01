#ifndef MPP_GENERATORS_LINEAR_INTERPOLATION_HPP
#define MPP_GENERATORS_LINEAR_INTERPOLATION_HPP

#include "bezier_interpolation.hpp"

namespace mpp
{
    template <typename Input>
    using LinearInterpolation = mpp::BezierInterpolation<Input, 0>;
}

#endif
