#ifndef MPP_GENERATORS_LINEAR_INTERPOLATION_HPP
#define MPP_GENERATORS_LINEAR_INTERPOLATION_HPP

#include "generator.hpp"
#include "basic.hpp"

#include <cmath>

namespace mpp
{
    template <typename Input, uint64_t order>
    struct BezierInterpolation;

    template <typename Input, uint64_t order>
    class BezierInterpolationBase
    {
    public:
        using NestedInput = std::conditional_t<order == 0,
            Input,
            BezierInterpolation<Input, order - 1>>;

        constexpr BezierInterpolationBase(const NestedInput& min, const NestedInput& max):
            _min { min },
            _max { max }
        {}

        constexpr Input interpolate(const uint64_t& index, const uint64_t& max_index) const&
        {
            Input min {0};
            Input max {0};

            if constexpr (order == 0)
            {
                min = _min;
                max = _max;
            }
            else
            {
                min = _min.interpolate(index, max_index);
                max = _max.interpolate(index, max_index);
            }

            const float& ratio { float(index) / float(max_index) };
            return (min * (1 - ratio)) + (max * ratio);
        }

    protected:
        NestedInput _min;
        NestedInput _max;
    };

    template <typename Input, uint64_t order>
    struct BezierInterpolation : public BezierInterpolationBase<Input, order>
    {
        using NestedInput = typename BezierInterpolationBase<Input, order>::NestedInput;

        constexpr BezierInterpolation(const NestedInput& min, const NestedInput& max):
            BezierInterpolationBase<Input, order> { min, max }
        {}
    };

    template <>
    struct BezierInterpolation<Frequency, 0> : public BezierInterpolationBase<Frequency, 0>
    {
        using NestedInput = typename BezierInterpolationBase<Frequency, 0>::NestedInput;

        constexpr BezierInterpolation(const Frequency& min, const Frequency& max):
            BezierInterpolationBase<Frequency, 0> { min, std::abs(max - min) / 2 }
        {}
    };

    template <typename Input>
    using LinearInterpolation = BezierInterpolation<Input, 0>;

    template <GeneratorShape Shape, typename Output, typename Input, uint64_t order>
    struct Generator<Shape, Output, BezierInterpolation<Input, order>>
    {
        Output generate(const uint64_t& index, const uint64_t& max_index) const&
        {
            const Input& interpolated_input { interpolation.interpolate(index, max_index) };
            std::cout << interpolated_input << '\n';

            Generator<Shape, Output, Input> generator { interpolated_input };
            return generator.generate(index, max_index);
        }

        BezierInterpolation<Input, order> interpolation;
    };
}

#endif
