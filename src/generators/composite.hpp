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
    template <typename Output, typename Input>
    struct Generator<Output, std::tuple<Input>>
    {
        Output generate(const uint64_t& index, const uint64_t& max_index)
        {
            return generator<Output>(std::get<0>(input)).generate(index, max_index);
        }

        std::tuple<Input>& input;
    };

    template <typename Output, typename FirstInput, typename SecondInput, typename... Inputs>
    struct Generator<Output, std::tuple<FirstInput, SecondInput, Inputs...>>
    {
        Output generate(const uint64_t& index, const uint64_t& max_index) const&
        {
            std::vector<Output> results_to_mix;

            for_each(inputs, [&results_to_mix, &index, &max_index](const auto& input)
            {
                const Output& result { generator<Output>(input).generate(index, max_index) };
                results_to_mix.emplace_back(result);
            });

            return mix<Output>(results_to_mix);
        }

        std::tuple<FirstInput, SecondInput, Inputs...>& inputs;
    };
}

#endif
