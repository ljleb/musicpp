#ifndef MPP_GENERATORS_LOW_PASS_HPP
#define MPP_GENERATORS_LOW_PASS_HPP

#include "basic.hpp"
#include "generator.hpp"
#include "mixer.hpp"

#include "controls/steady.hpp"

#include "utils/interpolate.hpp"
#include <array>

namespace mpp
{
    template <typename CutoffControl, typename OrderControl, typename Input>
    class HighPass;

    template <typename CutoffControl, typename OrderControl, typename Input>
    struct LowPass
    {
        constexpr LowPass(CutoffControl const& cutoff, OrderControl const& order, Input const& input):
            _cutoff { cutoff },
            _order { order },
            _input { input },
            _last_wet_samples {}
        {}

        constexpr LowPass(CutoffControl const& cutoff, Input const& input):
            LowPass<CutoffControl, uint64_t, Input> { cutoff, 1, input }
        {}

        constexpr Sample generate_sample(TimePoint const& time, Config const& config)
        {
            auto&& nested_generator { generator<Sample>(_input) };
            Sample output { nested_generator.generate(time, config) };

            double const& actual_cutoff { interpolate_control(_cutoff, time) * interpolate_control(_order, time) };
            double const& ratio { actual_cutoff / config.sample_rate };

            _last_wet_samples.resize(interpolate_control(_order, time));
            for (uint64_t i { 0 }; i < _last_wet_samples.size(); ++i)
            {
                Sample const& interpolated = interpolate(_last_wet_samples[i], output, ratio);
                _last_wet_samples[i] = interpolated;
                output = interpolated;
            }

            return output;
        }

        friend class HighPass<CutoffControl, OrderControl, Input>;

        CutoffControl _cutoff;
        OrderControl _order;
        Input _input;
        std::vector<Sample> _last_wet_samples;
    };

    template <typename CutoffControl, typename Input>
    LowPass(CutoffControl const&, Input const&) ->
        LowPass<CutoffControl, uint64_t, Input>;

    template <typename CutoffControl, typename OrderControl, typename Input>
    struct Generator<Sample, LowPass<CutoffControl, OrderControl, Input>>
    {
        constexpr Sample generate(TimePoint const& time, Config const& config)
        {
            return low_pass.generate_sample(time, config);
        }

        constexpr uint64_t size() const&
        {
            return generator<Sample>(low_pass._input).size();
        }

        constexpr uint64_t offset() const&
        {
            return generator<Sample>(low_pass._input).offset();
        }

        LowPass<CutoffControl, OrderControl, Input>& low_pass;
    };
}

#endif
