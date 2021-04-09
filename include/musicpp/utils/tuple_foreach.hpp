#ifndef MPP_UTILS_TUPLE_FOREACH_HPP
#define MPP_UTILS_TUPLE_FOREACH_HPP

#include <tuple>
#include <algorithm>
#include <cstdint>

template <typename Tuple, typename Function, std::size_t... Indices>
void for_each_impl(Tuple&& tuple, Function&& function, std::index_sequence<Indices...>) {
    using swallow = int[];
    static_cast<void>(swallow
    {
        1,
        (
            function(std::get<Indices>(std::forward<Tuple>(tuple))),
            void(),
            int {}
        )...
    });
}

template <typename Tuple, typename Function>
void for_each(Tuple&& tuple, Function&& function) {
    constexpr std::size_t tuple_size = std::tuple_size<std::remove_reference_t<Tuple>>::value;
    for_each_impl(
        std::forward<Tuple>(tuple),
        std::forward<Function>(function),
        std::make_index_sequence<tuple_size> {});
}

#endif
