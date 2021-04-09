#ifndef MPP_GENERATORS_LOW_PASS_HPP
#define MPP_GENERATORS_LOW_PASS_HPP

#include "basic.hpp"

#include <musicpp/generator.hpp>
#include <musicpp/mixer.hpp>

#include <musicpp/controls/steady.hpp>

#include <musicpp/utils/interpolate.hpp>
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
        constexpr Sample generate_at(TimePoint const& time, Config const& config) const&
        {
            auto&& nested_generator { generator<Sample>(low_pass._input) };
            Sample output { nested_generator.generate_at(time, config) };

            double const& actual_cutoff { interpolate_control(low_pass._cutoff, time) * interpolate_control(low_pass._order, time) };
            double const& ratio { actual_cutoff / config.sample_rate };

            low_pass._last_wet_samples.resize(interpolate_control(low_pass._order, time));
            for (uint64_t i { 0 }; i < low_pass._last_wet_samples.size(); ++i)
            {
                Sample const& interpolated = interpolate(low_pass._last_wet_samples[i], output, ratio);
                low_pass._last_wet_samples[i] = interpolated;
                output = interpolated;
            }

            return output;
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
