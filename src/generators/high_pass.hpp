#ifndef MPP_GENERATORS_HIGH_PASS_HPP
#define MPP_GENERATORS_HIGH_PASS_HPP

#include "low_pass.hpp"

#include "generator.hpp"
#include "mixer.hpp"
#include "project_config.hpp"

namespace mpp
{
    template <typename CutoffControl, typename OrderControl, typename Input>
    struct HighPass
    {
        constexpr HighPass(const CutoffControl& cutoff, const OrderControl& order, const Input& input):
            _input { input },
            _nested { cutoff, order, input }
        {}

        constexpr HighPass(const CutoffControl& cutoff, const Input& input):
            HighPass<CutoffControl, uint64_t, Input> { cutoff, 1, input }
        {}

        constexpr Sample generate_sample(const TimePoint& time)
        {
            const Sample& output { generator<Sample>(_input).generate(time) };

            if (interpolate_control(_nested._order, time) > 0)
            {
                const Sample& low_passed_output { _nested.generate_sample(time) };
                return output - low_passed_output;
            }
            else
            {
                return output;
            }
        }

    private:
        Input _input;
        LowPass<CutoffControl, OrderControl, Input> _nested;
    };

    template <typename CutoffControl, typename Input>
    HighPass(const CutoffControl& cutoff, const Input& input) ->
        HighPass<CutoffControl, uint64_t, Input>;

    template <typename CutoffControl, typename OrderControl, typename Input>
    struct Generator<Sample, HighPass<CutoffControl, OrderControl, Input>>
    {
        Sample generate(const TimePoint& time)
        {
            return high_pass.generate_sample(time);
        }

        HighPass<CutoffControl, OrderControl, Input>& high_pass;
    };
}

#endif
