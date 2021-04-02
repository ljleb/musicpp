#ifndef MPP_GENERATORS_HIGH_PASS_HPP
#define MPP_GENERATORS_HIGH_PASS_HPP

#include "low_pass.hpp"

#include "generator.hpp"
#include "mixer.hpp"
#include "project_config.hpp"

namespace mpp
{
    template <typename Input, uint64_t order = 3>
    struct HighPass
    {
        constexpr HighPass(Frequency&& frequency, Input&& input):
            _low_pass { std::move(frequency), std::move(input) }
        {}

        constexpr Input& input()
        {
            return _low_pass.input();
        }

        constexpr const Frequency& frequency() const&
        {
            return _low_pass.frequency();
        }

        constexpr Sample filter_sample(const Sample& new_sample, const double& ratio)
        {
            const Sample& low_passed_sample = _low_pass.filter_sample(new_sample, ratio);
            return new_sample - low_passed_sample;
        }

    private:
        LowPass<Input, order> _low_pass;
    };

    template <typename Input, uint64_t order>
    struct Generator<Sample, HighPass<Input, order>>
    {
        Sample generate(const uint64_t& index, const uint64_t& max_index)
        {
            const Sample& sample = generator<Sample>(high_pass.input())
                .generate(index, max_index);

            const double& ratio { high_pass.frequency() / double(SAMPLE_RATE) };
            return high_pass.filter_sample(sample, ratio);
        }

        HighPass<Input, order>& high_pass;
    };
}

#endif
