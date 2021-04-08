#ifndef MPP_GENERATOR_HPP
#define MPP_GENERATOR_HPP

#include "config.hpp"

#include <algorithm>
#include <cstdint>

namespace mpp
{
    struct TimePoint
    {
        template <typename A>
        constexpr std::enable_if_t<std::is_arithmetic_v<A>, TimePoint> operator/(A const& amount) const&
        {
            return
            {
                static_cast<uint64_t>(index / amount),
                index_limit
            };
        }

        template <typename A>
        constexpr std::enable_if_t<std::is_arithmetic_v<A>, TimePoint> operator-(A const& offset) const&
        {
            return
            {
                std::min(std::max(static_cast<uint64_t>(index - offset), 0UL), index_limit - 1),
                index_limit
            };
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
        Output generate(TimePoint const& time, Config const& config);
        uint64_t size() const&;
        uint64_t offset() const&;
    };

    template <typename Output, typename Input>
    constexpr Generator<Output, Input> generator(Input& input)
    {
        return Generator<Output, Input> { input };
    }
}

#endif
