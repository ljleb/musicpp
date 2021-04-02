#ifndef MPP_GENERATOR_HPP
#define MPP_GENERATOR_HPP

#include <cstdint>

namespace mpp
{
    enum GeneratorShape
    {
        SINE = 0,
        SAW,
    };

    template <GeneratorShape Shape, typename Output, typename Input>
    struct Generator
    {
        Output generate(const uint64_t& index, const uint64_t& max_index);
    };

    template <GeneratorShape Shape, typename Output, typename Input>
    Generator<Shape, Output, Input> generator(const Input& input)
    {
        return Generator<Shape, Output, Input> { input };
    }
}

#endif
