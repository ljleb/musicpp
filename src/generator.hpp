#ifndef MPP_GENERATOR_HPP
#define MPP_GENERATOR_HPP

#include <cstdint>

namespace mpp
{
    enum GeneratorShape
    {
        SINE = 0,
    };

    template <GeneratorShape Shape, typename Output, typename... Args>
    struct Generator
    {
        Output generate(const uint64_t& index, const uint64_t& max_index);
    };

    template <GeneratorShape Shape, typename Output, typename... Args>
    Generator<Shape, Output, Args...> generator(const Args&... args)
    {
        return Generator<Shape, Output, Args...> { args... };
    }
}

#endif
