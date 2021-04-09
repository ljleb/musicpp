#ifndef MPP_UTILS_FUNCTIONAL_HPP
#define MPP_UTILS_FUNCTIONAL_HPP

#include <functional>

namespace mpp
{
    template <template <typename...> typename Constructor, typename... PartialArgs>
    constexpr auto bind_front(PartialArgs const&... partial_args)
    {
        return std::bind_front(
            [](auto const&... args){ return Constructor { args... }; },
            partial_args...
        );
    }
}

#endif
