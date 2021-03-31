#ifndef MPP_GENERATOR_HPP
#define MPP_GENERATOR_HPP

#include <iostream>
#include <algorithm>
#include <memory>

namespace mpp
{
    template <uint64_t id, typename Output, typename... Inputs>
    struct Generator
    {
        Output generate(Inputs... inputs) const;
    };

    template <uint64_t id, typename Output, typename... Inputs>
    Output generate(const Inputs&... inputs)
    {
        const Generator<id, Output, Inputs...> generator;
        return generator.generate(inputs...);
    }
}

#endif
