#ifndef MPP_GENERATORS_BEZIER_HPP
#define MPP_GENERATORS_BEZIER_HPP

#include "generator.hpp"

#include "math.hpp"
#include <algorithm>

namespace mpp
{
    template <typename Input, uint64_t order>
    struct Bezier;

    namespace _internal
    {
        template <typename Input, uint64_t order>
        struct BezierBase
        {
            using Maxim = Bezier<Input, order - 1>;

            constexpr BezierBase(Maxim&& min, Maxim&& max):
                _min { std::move(min) },
                _max { std::move(max) },
                _interpolated { 0 }
            {}

            constexpr Input& interpolate_input(const uint64_t& index, const uint64_t& max_index)
            {
                const double& ratio { double(index) / double(max_index) };
                _interpolated = interpolate(
                    _min.interpolate_input(index, max_index),
                    _max.interpolate_input(index, max_index),
                    ratio);
                return _interpolated;
            }

        private:
            Maxim _min;
            Maxim _max;
            Input _interpolated;
        };

        template <typename Input>
        struct BezierBase<Input, 0>
        {
            constexpr BezierBase(Input&& min, Input&& max):
                _min { std::move(min) },
                _max { std::move(max) },
                _interpolated { _min }
            {}

            constexpr Input& interpolate_input(const uint64_t& index, const uint64_t& max_index)
            {
                const double& ratio { double(index) / double(max_index) };
                _interpolated = interpolate(_min, _max, ratio);
                return _interpolated;
            }

        private:
            Input _min;
            Input _max;
            Input _interpolated;
        };
    }

    template <typename Input, uint64_t order = 0>
    struct Bezier : public _internal::BezierBase<Input, order>
    {
        using NestedInput = Bezier<Input, order - 1>;

        constexpr Bezier(NestedInput&& min, NestedInput&& max):
            _internal::BezierBase<Input, order> { std::move(min), std::move(max) }
        {}
    };

    template <typename Input>
    struct Bezier<Input, 0> : public _internal::BezierBase<Input, 0>
    {
        constexpr Bezier(Input&& min, Input&& max):
            _internal::BezierBase<Input, 0> {
                std::move(min),
                { interpolate(min, max, .5f) },
            }
        {}
    };

    template <typename Output, typename Input, uint64_t order>
    struct Generator<Output, Bezier<Input, order>>
    {
        Output generate(const uint64_t& index, const uint64_t& max_index) const&
        {
            Input& input { interpolation.interpolate_input(index, max_index) };
            return generator<Output>(input).generate(index, max_index);
        }

        Bezier<Input, order>& interpolation;
    };
}

#endif
