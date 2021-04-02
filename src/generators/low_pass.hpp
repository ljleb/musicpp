#ifndef MPP_GENERATORS_LOW_PASS_HPP
#define MPP_GENERATORS_LOW_PASS_HPP

#include "basic.hpp"
#include "bezier_interpolation.hpp"

#include "generator.hpp"
#include "mixer.hpp"
#include "project_config.hpp"

namespace mpp
{
    template <typename Input>
    struct LowPass
    {
        constexpr LowPass(Frequency&& frequency, Input&& input):
            _frequency { std::move(frequency) },
            _last_sample { 0 },
            _input { std::move(input) }
        {}

        constexpr Input& input()
        {
            return _input;
        }

        constexpr Sample filter_sample(const Sample& new_sample)
        {
            const float& ratio { float(_frequency) / float(SAMPLE_RATE) };
            _last_sample = _internal::interpolate(_last_sample, new_sample, ratio);
            return _last_sample;
        }

    private:
        Frequency _frequency;
        Sample _last_sample;
        Input _input;
    };

    template <GeneratorShape Shape, typename Input>
    struct Generator<Shape, Sample, LowPass<Input>>
    {
        Sample generate(const uint64_t& index, const uint64_t& max_index)
        {
            const Sample& sample = generator<Shape, Sample>(low_pass.input())
                .generate(index, max_index);

            return low_pass.filter_sample(sample);
        }

        LowPass<Input>& low_pass;
    };
}

#endif
