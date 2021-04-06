#ifndef MPP_LITERALS_HPP
#define MPP_LITERALS_HPP

#include "controls/steady.hpp"

namespace mpp
{
    constexpr Steady<double> frequency_of_key(const double& note)
    {
        return 440 * std::pow(2, (note - 49) / 12);
    }

    constexpr Steady<double> operator"" _C(long double key)
    {
        return frequency_of_key(key * 12 - 9);
    }

    constexpr Steady<double> operator"" _C(unsigned long long key)
    {
        return frequency_of_key(static_cast<double>(key) * 12 - 9);
    }

    constexpr Steady<double> operator"" _D(long double key)
    {
        return frequency_of_key(key * 12 - 7);
    }

    constexpr Steady<double> operator"" _D(unsigned long long key)
    {
        return frequency_of_key(static_cast<double>(key) * 12 - 7);
    }

    constexpr Steady<double> operator"" _E(long double key)
    {
        return frequency_of_key(key * 12 - 5);
    }

    constexpr Steady<double> operator"" _E(unsigned long long key)
    {
        return frequency_of_key(static_cast<double>(key) * 12 - 5);
    }

    constexpr Steady<double> operator"" _F(long double key)
    {
        return frequency_of_key(key * 12 - 4);
    }

    constexpr Steady<double> operator"" _F(unsigned long long key)
    {
        return frequency_of_key(static_cast<double>(key) * 12 - 4);
    }

    constexpr Steady<double> operator"" _G(long double key)
    {
        return frequency_of_key(key * 12 - 2);
    }

    constexpr Steady<double> operator"" _G(unsigned long long key)
    {
        return frequency_of_key(static_cast<double>(key) * 12 - 2);
    }

    constexpr Steady<double> operator"" _A(long double key)
    {
        return frequency_of_key(key * 12);
    }

    constexpr Steady<double> operator"" _A(unsigned long long key)
    {
        return frequency_of_key(static_cast<double>(key) * 12);
    }

    constexpr Steady<double> operator"" _B(long double key)
    {
        return frequency_of_key(key * 12 + 2);
    }

    constexpr Steady<double> operator"" _B(unsigned long long key)
    {
        return frequency_of_key(static_cast<double>(key) * 12 + 2);
    }

    const Steady<double> operator"" _steady(long double value)
    {
        return value;
    }

    const Steady<uint64_t> operator"" _steady(unsigned long long value)
    {
        return value;
    }
}

#endif
