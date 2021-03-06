#ifndef MPP_GENERATORS_BASIC_HPP
#define MPP_GENERATORS_BASIC_HPP

#include <musicpp/generator.hpp>
#include <musicpp/utils/string_literal.hpp>
#include <musicpp/controls/steady.hpp>

#include <cmath>

namespace mpp
{
    template <StringLiteral shape, typename FrequencyControl>
    struct Basic
    {
        constexpr Basic(FrequencyControl const& frequency):
            _frequency { frequency }
        {}

        FrequencyControl _frequency;
    };

    template <StringLiteral shape, typename FrequencyControl>
    Basic<shape, FrequencyControl> make_basic(FrequencyControl const& frequency)
    {
        return { frequency };
    }

    template <typename FrequencyControl>
    struct Generator<Sample, Basic<"sine", FrequencyControl>>
    {
        constexpr Sample generate_at(TimePoint const& time, Config const& config) const&
        {
            const double& frequency { interpolate_control(basic._frequency, time / 2) };
            return std::sin(2 * M_PI * frequency * time.index / config.sample_rate);
        }

        constexpr uint64_t offset() const&
        {
            return 0;
        }

        Basic<"sine", FrequencyControl>& basic;
    };

    template <typename FrequencyControl>
    struct Generator<Sample, Basic<"saw", FrequencyControl>>
    {
        constexpr Sample generate_at(TimePoint const& time, Config const& config) const&
        {
            const double& frequency { interpolate_control(basic._frequency, time / 2) };
            return std::fmod(2 * frequency * time.index / config.sample_rate, 2) - 1;
        }

        constexpr uint64_t offset() const&
        {
            return 0;
        }

        Basic<"saw", FrequencyControl>& basic;
    };
}

#endif
