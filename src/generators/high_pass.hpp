#ifndef MPP_GENERATORS_HIGH_PASS_HPP
#define MPP_GENERATORS_HIGH_PASS_HPP

#include "low_pass.hpp"

#include "generator.hpp"
#include "mixer.hpp"
#include "project_config.hpp"

namespace mpp
{
    template <typename Input, uint64_t order = 4>
    struct HighPass
    {
        constexpr HighPass(Frequency&& frequency, Input&& input):
            _input { std::move(input) },
            _nested { std::move(frequency), std::move(input) }
        {}

        constexpr Sample generate_sample(const uint64_t& index, const uint64_t& max_index)
        {
            const Sample& output { generator<Sample>(_input).generate(index, max_index) };

            if constexpr (order > 0)
            {
                const Sample& low_passed_output { _nested.generate_sample(index, max_index) };
                return output - low_passed_output;
            }
            else
            {
                return output;
            }
        }

    private:
        Input _input;
        LowPass<Input, order> _nested;
    };

    template <typename Input, uint64_t order>
    struct Generator<Sample, HighPass<Input, order>>
    {
        Sample generate(const uint64_t& index, const uint64_t& max_index)
        {
            return high_pass.generate_sample(index, max_index);
        }

        HighPass<Input, order>& high_pass;
    };
}

#endif
