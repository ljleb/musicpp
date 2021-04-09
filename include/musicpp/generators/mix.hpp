#ifndef MPP_GENERATORS_MIX_HPP
#define MPP_GENERATORS_MIX_HPP

#include <musicpp/generator.hpp>
#include <musicpp/mixer.hpp>

#include <musicpp/utils/tuple_foreach.hpp>
#include <vector>
#include <tuple>

namespace mpp
{
    template <typename... Inputs>
    struct Mix : public std::tuple<Inputs...>
    {
        constexpr Mix(Inputs const&... inputs):
            std::tuple<Inputs...> { inputs... }
        {}
    };

    template <typename Output, typename... Inputs>
    struct Generator<Output, Mix<Inputs...>>
    {
        constexpr Output generate_at(TimePoint const& time, Config const& config) const&
        {
            if constexpr (sizeof...(Inputs) == 1)
            {
                return generator<Output>(std::get<0>(inputs)).generate_at(time, config);
            }
            else
            {
                std::vector<Output> results_to_mix;

                for_each(static_cast<std::tuple<Inputs...>&>(inputs), [&results_to_mix, &time, &config](auto& input)
                {
                    Output const& result { generator<Output>(input).generate_at(time, config) };
                    results_to_mix.emplace_back(result);
                });

                return mix<Output>(results_to_mix);
            }
        }

        constexpr uint64_t size() const&
        {
            if constexpr (sizeof...(Inputs) == 1)
            {
                return generator<Output>(std::get<0>(inputs)).size();
            }
            else
            {
                uint64_t max_size_with_offset { 0 };

                for_each(static_cast<std::tuple<Inputs...>&>(inputs), [&max_size_with_offset](auto& input)
                {
                    auto const& current_generator { generator<Output>(input) };
                    uint64_t const& current_size { current_generator.offset() + current_generator.size() };
                    max_size_with_offset = std::max(max_size_with_offset, current_size);
                });

                return max_size_with_offset - offset();
            }
        }

        constexpr uint64_t offset() const&
        {
            if constexpr (sizeof...(Inputs) == 1)
            {
                return generator<Output>(std::get<0>(inputs)).offset();
            }
            else
            {
                uint64_t min_offset { std::numeric_limits<uint64_t>::max() };

                for_each(static_cast<std::tuple<Inputs...>&>(inputs), [&min_offset](auto& input)
                {
                    auto const& current_generator { generator<Output>(input) };
                    uint64_t const& current_offset { current_generator.offset() };
                    min_offset = std::min<uint64_t>(min_offset, current_offset);
                });

                return min_offset;
            }
        }

        Mix<Inputs...>& inputs;
    };
}

#endif
