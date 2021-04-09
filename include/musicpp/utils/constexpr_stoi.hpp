#ifndef MPP_UTILS_CONSTEXPR_STOI_HPP
#define MPP_UTILS_CONSTEXPR_STOI_HPP

#include <cstdint>

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
