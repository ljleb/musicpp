#ifndef MPP_GENERATORS_LINEAR_INTERPOLATION_HPP
#define MPP_GENERATORS_LINEAR_INTERPOLATION_HPP

#include "generator.hpp"
#include "basic.hpp"

namespace mpp
{
    template <typename A>
    class LinearInterpolation
    {
    public:
        constexpr LinearInterpolation(
            const A& min,
            const A& max
        ):
            _min { min }, 
            _max { max }
        {}

        constexpr A interpolate(const A& ratio) const&
        {
            return _min * (1 - ratio) + _max * ratio;
        }

    private:
        A _min;
        A _max;
    };

    template <GeneratorShape Shape, typename Output>
    struct Generator<Shape, Output, LinearInterpolation<Frequency>>
    {
        Output generate(const uint64_t& index, const uint64_t& max_index) const&
        {
            const float& ratio { index / float(max_index) };
            const float& frequency { frequency_interpolation.interpolate(ratio) };

            Generator<Shape, Output, Frequency> generator { frequency };
            return generator.generate(index, max_index);
        }

        LinearInterpolation<Frequency> frequency_interpolation;
    };
}

#endif
