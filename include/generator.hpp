#ifndef MPP_GENERATOR_HPP
#define MPP_GENERATOR_HPP

#include "config.hpp"
#include "time_point.hpp"

namespace mpp
{
    template <typename Output, typename Input>
    struct Generator
    {
        Output generate_at(TimePoint const& time, Config const& config) const&;
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
