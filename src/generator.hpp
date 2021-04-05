#ifndef MPP_GENERATOR_HPP
#define MPP_GENERATOR_HPP

#include <cstdint>

namespace mpp
{
    struct TimePoint
    {
        constexpr TimePoint operator/(const double& amount) const&
        {
            return { static_cast<uint64_t>(index / amount), index_limit };
        }

        constexpr double completion() const&
        {
            return static_cast<double>(index) / static_cast<double>(index_limit);
        }

        uint64_t index;
        uint64_t index_limit;
    };

    template <typename Output, typename Input>
    struct Generator
    {
        Output generate(const TimePoint& time);
    };

    template <typename Output, typename Input>
    Generator<Output, Input> generator(Input& input)
    {
        return Generator<Output, Input> { input };
    }
}

#endif
