#ifndef MPP_GENERATORS_LATENCY_HPP
#define MPP_GENERATORS_LATENCY_HPP

#include "generator.hpp"
#include "project_config.hpp"

#include <cstdint>
#include <algorithm>

namespace mpp
{
    struct Latency
    {
        constexpr Latency(const int64_t& delay):
            _delay { delay }
        {}

        constexpr bool can_delay_index(const uint64_t& index, const uint64_t& max_index) const&
        {
            const int64_t& delayed_index = index + _delay;
            return delayed_index >= 0 && delayed_index < max_index;
        }

        constexpr uint64_t delay_index(const uint64_t& index) const&
        {
            return index + _delay;
        }

    private:
        int64_t _delay;
    };
}

#endif
