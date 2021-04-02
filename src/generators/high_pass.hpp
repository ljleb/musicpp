#ifndef MPP_GENERATORS_HIGH_PASS_HPP
#define MPP_GENERATORS_HIGH_PASS_HPP

#include "low_pass.hpp"

#include "generator.hpp"
#include "mixer.hpp"
#include "project_config.hpp"

namespace mpp
{
    template <typename Input>
    struct HighPass
    {
        constexpr HighPass(Frequency&& frequency, Input&& input):
            _low_pass { std::move(frequency), std::move(input) }
        {}

        constexpr Input& input()
        {
            return _low_pass.input();
        }

        constexpr Sample filter_sample(const Sample& new_sample)
        {
            const Sample& low_passed_sample = _low_pass.filter_sample(new_sample);
            return new_sample - low_passed_sample;
        }

    private:
        LowPass<Input> _low_pass;
    };

    template <GeneratorShape Shape, typename Input>
    struct Generator<Shape, Sample, HighPass<Input>>
    {
        Sample generate(const uint64_t& index, const uint64_t& max_index)
        {
            const Sample& sample = generator<Shape, Sample>(high_pass.input())
                .generate(index, max_index);

            return high_pass.filter_sample(sample);
        }

        HighPass<Input>& high_pass;
    };
}

#endif
