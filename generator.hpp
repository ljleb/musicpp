#ifndef MPP_GENERATOR_HPP
#define MPP_GENERATOR_HPP

#include <iostream>
#include <algorithm>
#include <memory>

namespace mpp
{
    namespace
    {
        struct StringLiteral
        {
            constexpr StringLiteral(const char*)
            {}
        };
    }

    template <StringLiteral literal, typename Output, typename... Inputs>
    struct Generator
    {
        std::unique_ptr<const Output> generate(Inputs... inputs) const;
    };

    template <StringLiteral literal, typename Output, typename... Inputs>
    std::unique_ptr<const Output> generate(Inputs&&... inputs)
    {
        Generator<literal, Output, Inputs...> generator;
        return generator.generate(std::forward<Inputs>(inputs)...);
    }
}

#endif
