#ifndef MPP_GENERATORS_LOW_PASS_HPP
#define MPP_GENERATORS_LOW_PASS_HPP

#include "latency.hpp"
#include "frequency.hpp"

#include "generator.hpp"
#include "mixer.hpp"
#include "project_config.hpp"

#include <array>

namespace mpp
{
    template <typename Input, uint64_t order = 4>
    struct LowPass
    {
        constexpr LowPass(Frequency&& frequency, Input&& input):
            _latency { 0 },
            _frequency { std::move(frequency) },
            _input { std::move(input) },
            _last_wet_samples {}
        {}

        constexpr Sample generate_sample(const uint64_t& index, const uint64_t& max_index)
        {
            if (!_latency.can_delay_index(index, max_index))
            {
                return {};
            }

            auto&& nested_generator { generator<Sample>(_input) };
            Sample output { nested_generator.generate(_latency.delay_index(index), max_index) };

            const double& ratio { _frequency / SAMPLE_RATE };
            for (uint64_t i { 0 }; i < _last_wet_samples.size(); ++i)
            {
                const Sample& interpolated = interpolate(_last_wet_samples[i], output, ratio);
                _last_wet_samples[i] = interpolated;
                output = interpolated;
            }

            return output;
        }

    private:
        Latency _latency;
        Frequency _frequency;
        Input _input;
        std::array<Sample, order> _last_wet_samples;
    };

    template <typename Input, uint64_t order>
    struct Generator<Sample, LowPass<Input, order>>
    {
        Sample generate(const uint64_t& index, const uint64_t& max_index)
        {
            return low_pass.generate_sample(index, max_index);
        }

        LowPass<Input, order>& low_pass;
    };
}

#endif
