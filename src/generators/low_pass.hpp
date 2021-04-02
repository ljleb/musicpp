#ifndef MPP_GENERATORS_LOW_PASS_HPP
#define MPP_GENERATORS_LOW_PASS_HPP

#include "bezier.hpp"
#include "frequency.hpp"

#include "generator.hpp"
#include "mixer.hpp"
#include "project_config.hpp"

namespace mpp
{
    template <typename Input, uint64_t order = 3>
    struct LowPass
    {
        constexpr LowPass(Frequency&& frequency, Input&& input):
            _last_sample { 0 },
            _nested { std::move(frequency), std::move(input) }
        {}

        constexpr Input& input()
        {
            return _nested.input();
        }

        constexpr const Frequency& frequency() const&
        {
            return _nested.frequency();
        }

        constexpr Sample filter_sample(const Sample& new_sample, const double& ratio)
        {
            _last_sample = interpolate(_last_sample, new_sample, ratio);
            return _nested.filter_sample(_last_sample, ratio);
        }

    private:
        Sample _last_sample;
        LowPass<Input, order - 1> _nested;
    };

    template <typename Input>
    struct LowPass<Input, 0>
    {
        constexpr LowPass(Frequency&& frequency, Input&& input):
            _frequency { std::move(frequency) },
            _input { std::move(input) },
            _last_sample { 0 }
        {}

        constexpr Input& input()
        {
            return _input;
        }

        constexpr const Frequency& frequency() const&
        {
            return _frequency;
        }

        constexpr Sample filter_sample(const Sample& new_sample, const double& ratio)
        {
            _last_sample = interpolate(_last_sample, new_sample, ratio);
            return _last_sample;
        }

    private:
        Frequency _frequency;
        Input _input;
        Sample _last_sample;
    };

    template <typename Input, uint64_t order>
    struct Generator<Sample, LowPass<Input, order>>
    {
        Sample generate(const uint64_t& index, const uint64_t& max_index)
        {
            const Sample& sample = generator<Sample>(low_pass.input())
                .generate(index, max_index);

            const double& ratio { low_pass.frequency() / double(SAMPLE_RATE) };
            return low_pass.filter_sample(sample, ratio);
        }

        LowPass<Input, order>& low_pass;
    };
}

#endif
