#ifndef MPP_GENERATORS_COMPOSITE_HPP
#define MPP_GENERATORS_COMPOSITE_HPP

#include "generator.hpp"
#include "mixer.hpp"
#include "project_config.hpp"

#include "tuple_foreach.hpp"
#include <vector>
#include <tuple>

namespace mpp
{
    template <GeneratorShape Shape, typename Output, typename... Generators>
    struct Generator<Shape, Output, std::tuple<Generators...>>
    {
        Output generate(const uint64_t& index, const uint64_t& max_index) const&
        {
            std::vector<Output> results_to_mix;

            for_each(generators, [&results_to_mix, &index, &max_index](const auto& generator)
            {
                results_to_mix.emplace_back(generator.generate(index, max_index));
            });

            return mix<Sample>(results_to_mix);
        }

        std::tuple<Generators...> generators;
    };
}

#endif
