#ifndef MPP_GENERATORS_SEQ_HPP
#define MPP_GENERATORS_SEQ_HPP

#include <musicpp/generator.hpp>
#include <functional>

namespace mpp
{
    namespace
    {
        template<typename NestedInput, typename... Functions, typename FirstFunction>
        constexpr auto seq_impl(NestedInput const& nested_input, FirstFunction const& first_function, Functions const&... functions)
        {
            if constexpr (sizeof...(Functions) == 0)
            {
                return first_function(nested_input);
            }
            else
            {
                return seq_impl(first_function(nested_input), functions...);
            }
        }

        template<typename NestedInput, typename... Functions>
        constexpr auto seq(NestedInput const& nested_input, Functions const&... functions)
        {
            return seq_impl(nested_input, functions...);
        }
    }
}

#endif
