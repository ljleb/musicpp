#ifndef MPP_GENERATOR_HPP
#define MPP_GENERATOR_HPP

#include <iostream>
#include <algorithm>
#include <memory>

namespace mpp
{
    template <typename Output, typename... Inputs>
    struct Generator
    {
        std::unique_ptr<const Output> generate(Inputs... inputs) const;
    };

    template <typename Output, typename... Inputs>
    inline std::unique_ptr<const Output> generate(Inputs&&... inputs)
    {
        const Generator<Output, Inputs...> generator;
        return generator.generate(std::forward<Inputs>(inputs)...);
    }
}

#endif
