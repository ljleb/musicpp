#ifndef MPP_TIME_POINT_HPP
#define MPP_TIME_POINT_HPP

#include <algorithm>
#include <cstdint>

namespace mpp
{
    struct TimePoint
    {
        template <typename A>
        constexpr std::enable_if_t<std::is_arithmetic_v<A>, TimePoint> operator/(A const& amount) const&
        {
            return
            {
                static_cast<uint64_t>(index / amount),
                index_limit
            };
        }

        constexpr TimePoint operator-(uint64_t const& offset) const&
        {
            uint64_t const& unbound_index { index - offset };
            uint64_t const& low_bound_index { std::max<uint64_t>(unbound_index, 0ULL) };
            uint64_t const& bound_index { std::min<uint64_t>(low_bound_index, index_limit - 1) };
            return
            {
                bound_index,
                index_limit
            };
        }

        constexpr double completion() const&
        {
            return static_cast<double>(index) / static_cast<double>(index_limit);
        }

        uint64_t index;
        uint64_t index_limit;
    };
}

#endif
