#ifndef MPP_LITERALS_HPP
#define MPP_LITERALS_HPP

#include "controls/steady.hpp"

namespace mpp
{
    constexpr double frequency_of_key(double const& note)
    {
        return 440 * std::pow(2, (note - 49) / 12);
    }

    constexpr double operator"" _C(long double const key)
    {
        return frequency_of_key(key * 12 - 9);
    }

    constexpr double operator"" _C(unsigned long long const key)
    {
        return frequency_of_key(static_cast<double const>(key) * 12 - 9);
    }

    constexpr double operator"" _D(long double const key)
    {
        return frequency_of_key(key * 12 - 7);
    }

    constexpr double operator"" _D(unsigned long long const key)
    {
        return frequency_of_key(static_cast<double const>(key) * 12 - 7);
    }

    constexpr double operator"" _E(long double const key)
    {
        return frequency_of_key(key * 12 - 5);
    }

    constexpr double operator"" _E(unsigned long long const key)
    {
        return frequency_of_key(static_cast<double const>(key) * 12 - 5);
    }

    constexpr double operator"" _F(long double const key)
    {
        return frequency_of_key(key * 12 - 4);
    }

    constexpr double operator"" _F(unsigned long long const key)
    {
        return frequency_of_key(static_cast<double const>(key) * 12 - 4);
    }

    constexpr double operator"" _G(long double const key)
    {
        return frequency_of_key(key * 12 - 2);
    }

    constexpr double operator"" _G(unsigned long long const key)
    {
        return frequency_of_key(static_cast<double const>(key) * 12 - 2);
    }

    constexpr double operator"" _A(long double const key)
    {
        return frequency_of_key(key * 12);
    }

    constexpr double operator"" _A(unsigned long long const key)
    {
        return frequency_of_key(static_cast<double const>(key) * 12);
    }

    constexpr double operator"" _B(long double const key)
    {
        return frequency_of_key(key * 12 + 2);
    }

    constexpr double operator"" _B(unsigned long long const key)
    {
        return frequency_of_key(static_cast<double const>(key) * 12 + 2);
    }
}

#endif
