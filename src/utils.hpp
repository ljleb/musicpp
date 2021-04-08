#ifndef MPP_TUPLE_FOREACH_HPP
#define MPP_TUPLE_FOREACH_HPP

#include <cstddef>
#include <tuple>
#include <utility>

template <typename Tuple, typename F, std::size_t ...Indices>
void for_each_impl(Tuple&& tuple, F&& f, std::index_sequence<Indices...>) {
    using swallow = int[];
    (void)swallow{1,
        (f(std::get<Indices>(std::forward<Tuple>(tuple))), void(), int{})...
    };
}

template <typename Tuple, typename F>
void for_each(Tuple&& tuple, F&& f) {
    constexpr std::size_t N = std::tuple_size<std::remove_reference_t<Tuple>>::value;
    for_each_impl(std::forward<Tuple>(tuple), std::forward<F>(f),
                  std::make_index_sequence<N>{});
}

constexpr bool is_digit(char const& c)
{
    return c <= '9' && c >= '0';
}

constexpr uint64_t stoi_rec(char const* const str, uint64_t value)
{
    if (str[0] != '\0')
    {
        if (is_digit(str[0]))
        {
            return stoi_rec(str + 1, value * 10 + (*str - '0'));
        }
        else
        {
            throw "found a non-digit in string";
        }
    }
    else
    {
        return value;
    }
}

constexpr uint64_t stoi(char const* const string)
{
    return stoi_rec(string, 0);
}

#endif
