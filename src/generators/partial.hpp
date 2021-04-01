#ifndef MPP_GENERATORS_PARTIAL_HPP
#define MPP_GENERATORS_PARTIAL_HPP

#include "generator.hpp"
#include "mixer.hpp"
#include "project_config.hpp"

#include "tuple_foreach.hpp"
#include <vector>
#include <tuple>

namespace mpp
{
    template <typename NestedGenerator>
    struct Partial
    {
        constexpr Partial(
            const NestedGenerator& generator,
            const uint64_t& offset,
            const uint64_t& length
        ):
            generator { generator },
            offset { offset },
            length { length }
        {}

        NestedGenerator generator;
        uint64_t offset;
        uint64_t length;
    };

    template <GeneratorShape Shape, typename Output, typename NestedGenerator>
    struct Generator<Shape, Output, Partial<NestedGenerator>>
    {
        constexpr Output generate(const uint64_t& index, const uint64_t& max_index) const&
        {
            if (index < partial.offset || index > partial.offset + partial.length)
            {
                return {};
            }

            return partial.generator.generate(index - partial.offset, partial.offset + partial.length);
        }

        Partial<NestedGenerator> partial;
    };
}

#endif
