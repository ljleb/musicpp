#ifndef MPP_GENERATORS_BEZIER_INTERPOLATION_HPP
#define MPP_GENERATORS_BEZIER_INTERPOLATION_HPP

#include "generator.hpp"
#include "basic.hpp"

#include <cmath>

namespace mpp
{
    template <typename Input, uint64_t order>
    struct BezierInterpolation;

    namespace _internal
    {
        template <typename Input>
        constexpr Input interpolate(const Input& min, const Input& max, const float& ratio)
        {
            return min + (max - min) * ratio;
        }

        template <typename Input, uint64_t order>
        struct BezierInterpolationBase
        {
            using Maxim = BezierInterpolation<Input, order - 1>;

            constexpr BezierInterpolationBase(Maxim&& min, Maxim&& max):
                _min { std::move(min) },
                _max { std::move(max) }
            {}

            constexpr Input& interpolate(const uint64_t& index, const uint64_t& max_index) const&
            {
                const float& ratio { float(index) / float(max_index) };
                return _internal::interpolate(_min.interpolate(ratio), _max.interpolate(ratio), ratio);
            }

        private:
            Maxim _min;
            Maxim _max;
        };

        template <typename Input>
        struct BezierInterpolationBase<Input, 0>
        {
            constexpr BezierInterpolationBase(Input&& min, Input&& max):
                _min { std::move(min) },
                _max { std::move(max) },
                _interpolated { _min }
            {}

            constexpr Input& interpolate(const uint64_t& index, const uint64_t& max_index)
            {
                const float& ratio { float(index) / float(max_index) };
                _interpolated = _internal::interpolate(_min, _max, ratio);
                return _interpolated;
            }

        private:
            Input _min;
            Input _max;
            Input _interpolated;
        };
    }

    template <typename Input, uint64_t order>
    struct BezierInterpolation : public _internal::BezierInterpolationBase<Input, order>
    {
        using NestedInput = typename _internal::BezierInterpolationBase<Input, order>::NestedInput;

        constexpr BezierInterpolation(NestedInput&& min, NestedInput&& max):
            _internal::BezierInterpolationBase<Input, order> { min, max }
        {}
    };

    template <>
    struct BezierInterpolation<Frequency, 0> : public _internal::BezierInterpolationBase<Frequency, 0>
    {
        constexpr BezierInterpolation(Frequency&& min, Frequency&& max):
            _internal::BezierInterpolationBase<Frequency, 0> {
                std::move(min),
                { _internal::interpolate(min, max, .5f) },
            }
        {}
    };

    template <GeneratorShape Shape, typename Output, typename Input, uint64_t order>
    struct Generator<Shape, Output, BezierInterpolation<Input, order>>
    {
        Output generate(const uint64_t& index, const uint64_t& max_index) const&
        {
            Input& interpolated_input { interpolation.interpolate(index, max_index) };
            return generator<Shape, Output>(interpolated_input).generate(index, max_index);
        }

        BezierInterpolation<Input, order>& interpolation;
    };
}

#endif
