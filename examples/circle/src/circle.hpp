#ifndef CIRCLE_SHAPE_HPP
#define CIRCLE_SHAPE_HPP

#include <musicpp/generators/basic.hpp>

namespace mpp {
    template <typename FrequencyControl>
    struct Generator<Sample, Basic<"circle", FrequencyControl>>
    {
        constexpr Sample generate_at(TimePoint const& time, Config const& config) const&
        {
            const double& frequency { interpolate_control(basic._frequency, time / 2) };
            const double& scaled_x { 2 * frequency * time.index / static_cast<double>(config.sample_rate) };
            const double& mod_x { std::fmod(scaled_x, 2) - 1 };
            const double& sq_mod_x { std::sqrt(1 - mod_x * mod_x) };
            return static_cast<size_t>(scaled_x / 2) % 2 == 0 ? sq_mod_x : -sq_mod_x;
        }

        constexpr uint64_t offset() const&
        {
            return 0;
        }

        Basic<"circle", FrequencyControl>& basic;
    };
}

#endif
