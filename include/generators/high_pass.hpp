#ifndef MPP_GENERATORS_HIGH_PASS_HPP
#define MPP_GENERATORS_HIGH_PASS_HPP

#include "low_pass.hpp"

#include "generator.hpp"
#include "mixer.hpp"

namespace mpp
{
    template <typename CutoffControl, typename OrderControl, typename Input>
    struct HighPass
    {
        constexpr HighPass(CutoffControl const& cutoff, OrderControl const& order, Input const& input):
            _input { input },
            _nested { cutoff, order, input }
        {}

        constexpr HighPass(CutoffControl const& cutoff, Input const& input):
            HighPass<CutoffControl, uint64_t, Input> { cutoff, 1, input }
        {}

        Input _input;
        LowPass<CutoffControl, OrderControl, Input> _nested;
    };

    template <typename CutoffControl, typename Input>
    HighPass(CutoffControl const&, Input const& input) ->
        HighPass<CutoffControl, uint64_t, Input>;

    template <typename CutoffControl, typename OrderControl, typename Input>
    struct Generator<Sample, HighPass<CutoffControl, OrderControl, Input>>
    {
        constexpr Sample generate_at(TimePoint const& time, Config const& config) const&
        {
            Sample const& output { generator<Sample>(high_pass._input).generate_at(time) };

            if (interpolate_control(high_pass._nested._order, time) > 0)
            {
                const Sample& low_passed_output { high_pass._nested.generate_sample(time) };
                return output - low_passed_output;
            }
            else
            {
                return output;
            }
        }

        constexpr uint64_t size() const&
        {
            return generator<Sample>(high_pass._nested).size();
        }

        constexpr uint64_t offset() const&
        {
            return generator<Sample>(high_pass._nested).offset();
        }

        HighPass<CutoffControl, OrderControl, Input>& high_pass;
    };
}

#endif
