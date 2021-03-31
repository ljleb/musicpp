#ifndef MPP_GENERATOR_HPP
#define MPP_GENERATOR_HPP

#include <cstdint>

namespace mpp
{
    enum GeneratorKind
    {
        SINE = 0,
    };

    template <GeneratorKind Kind, typename Output, typename... Args>
    struct Generator
    {
        Output generate(const uint64_t& index, const uint64_t& max_index) const&;
    };

    template <GeneratorKind Kind, typename Output, typename... Args>
    Generator<Kind, Output, Args...> generator(const Args&... args)
    {
        return Generator<Kind, Output, Args...> { args... };
    }
}

#endif
