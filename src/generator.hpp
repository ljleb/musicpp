#ifndef MPP_GENERATOR_HPP
#define MPP_GENERATOR_HPP

#include <cstdint>

namespace mpp
{
    template <typename Output, typename Input>
    struct Generator
    {
        Output generate(const uint64_t& index, const uint64_t& max_index);
    };

    template <typename Output, typename Input>
    Generator<Output, Input> generator(Input& input)
    {
        return Generator<Output, Input> { input };
    }
}

#endif
