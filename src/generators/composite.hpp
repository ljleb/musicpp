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
    template <typename... Inputs>
    struct Mix : public std::tuple<Inputs...>
    {
        constexpr Mix(Inputs&&... inputs):
            std::tuple<Inputs...> { std::forward<Inputs>(inputs)... }
        {}
    };

    template <typename Output, typename... Inputs>
    struct Generator<Output, Mix<Inputs...>>
    {
        Output generate(const TimePoint& time)
        {
            if constexpr (sizeof...(Inputs) == 1)
            {
                return generator<Output>(std::get<0>(inputs)).generate(time);
            }
            else
            {
                std::vector<Output> results_to_mix;

                for_each(static_cast<std::tuple<Inputs...>&>(inputs), [&results_to_mix, &time](auto& input)
                {
                    const Output& result { generator<Output>(input).generate(time) };
                    results_to_mix.emplace_back(result);
                });

                return mix<Output>(results_to_mix);
            }
        }

        Mix<Inputs...>& inputs;
    };
}

#endif
