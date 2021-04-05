#include <iostream>

#include "generators/composite.hpp"
#include "generators/section.hpp"
#include "generators/low_pass.hpp"
#include "generators/high_pass.hpp"
#include "generators/basic.hpp"
#include "mixers/sample.hpp"

#include "controls/bezier.hpp"
#include "controls/steady.hpp"

#include "generator.hpp"
#include "mixer.hpp"
#include "project_config.hpp"
#include "export.hpp"

#include "literals.hpp"

#include <cstdint>

int main()
{
    using namespace mpp;

    auto&& master_input = Mix
    {
        LowPass
        {
            Bezier { 0_C, 8_C },
            4_steady,
            Mix
            {
                BasicSine { 8_C },
                BasicSine { 4_C },
            },
        },
        Section
        {
            Steady { SAMPLE_RATE },
            LowPass
            {
                Bezier { 0_C, 8_C },
                4_steady,
                Mix
                {
                    BasicSine { 8_C },
                    BasicSine { 4_C },
                },
            },
        },
    };

    auto&& master_generator { generator<Sample>(master_input) };

    Samples result {};

    for (uint64_t result_index { 0 }; result_index < result.size(); ++result_index)
    {
        const TimePoint& time { result_index, result.size() };
        result[result_index] = master_generator.generate(time);
    }

    write_samples_to("a.raw", result);

    return 0;
}
