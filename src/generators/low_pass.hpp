#ifndef MPP_GENERATORS_LOW_PASS_HPP
#define MPP_GENERATORS_LOW_PASS_HPP

#include "controls/steady.hpp"

#include "basic.hpp"

#include "generator.hpp"
#include "mixer.hpp"
#include "project_config.hpp"

#include <array>

namespace mpp
{
    template <typename CutoffControl, typename OrderControl, typename Input>
    struct LowPass
    {
        constexpr LowPass(const CutoffControl& cutoff, const OrderControl& order, const Input& input):
            _cutoff { cutoff },
            _order { order },
            _input { input },
            _last_wet_samples {}
        {}

        constexpr LowPass(const CutoffControl& cutoff, const Input& input):
            LowPass<CutoffControl, OrderControl, Input> { cutoff, Steady<uint64_t> { 1 }, input }
        {}

        constexpr Sample generate_sample(const TimePoint& time)
        {
            auto&& nested_generator { generator<Sample>(_input) };
            Sample output { nested_generator.generate(time) };

            const double& actual_cutoff { _cutoff.interpolate_control(time) * _order.interpolate_control(time) };
            const double& ratio { actual_cutoff / SAMPLE_RATE };
            _last_wet_samples.resize(_order.interpolate_control(time));
            for (uint64_t i { 0 }; i < _last_wet_samples.size(); ++i)
            {
                const Sample& interpolated = interpolate(_last_wet_samples[i], output, ratio);
                _last_wet_samples[i] = interpolated;
                output = interpolated;
            }

            return output;
        }

    private:
        CutoffControl _cutoff;
        OrderControl _order;
        Input _input;
        std::vector<Sample> _last_wet_samples;
    };

    template <typename CutoffControl, typename Input>
    LowPass(const CutoffControl& cutoff, const Input& input) ->
        LowPass<CutoffControl, Steady<uint64_t>, Input>;

    template <typename CutoffControl, typename OrderControl, typename Input>
    struct Generator<Sample, LowPass<CutoffControl, OrderControl, Input>>
    {
        Sample generate(const TimePoint& time)
        {
            return low_pass.generate_sample(time);
        }

        LowPass<CutoffControl, OrderControl, Input>& low_pass;
    };
}

#endif
