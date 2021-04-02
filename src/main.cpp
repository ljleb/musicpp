#include <iostream>

#include "generators/composite.hpp"
#include "generators/section.hpp"
#include "generators/linear_interpolation.hpp"
#include "generators/low_pass.hpp"
#include "generators/high_pass.hpp"
#include "generators/basic.hpp"
#include "mixers/sample.hpp"

#include "generator.hpp"
#include "mixer.hpp"
#include "project_config.hpp"
#include "export.hpp"

#include <cstdint>

float frequency_of_key(const float& note)
{
    return 440 * std::pow(2, (note - 49) / 12);
}

int main()
{
    using namespace mpp;

    auto&& master_input = std::tuple {
        // Section<Section<LinearInterpolation<Frequency>>, 3>
        // {
        //     {
        //         { 200, 0 },
        //         SAMPLE_RATE / 2,
        //     },
        //     SAMPLE_RATE,
        //     SAMPLE_RATE,
        // },

        // LinearInterpolation<Frequency> { 300, 0 },
        HighPass<LinearInterpolation<Frequency>> {
            SAMPLE_RATE / 32,
            { 600, 0 }
        },

        // Section<Frequency> {
        //     input: frequency_of_key(44),
        //     size: SAMPLE_RATE / 4,
        //     offset: (SAMPLE_RATE / 4) * 0,
        // },
        // Section<Frequency> {
        //     input: frequency_of_key(46),
        //     size: SAMPLE_RATE / 4,
        //     offset: (SAMPLE_RATE / 4) * 1,
        // },
        // Section<Frequency> {
        //     input: frequency_of_key(48),
        //     size: SAMPLE_RATE / 4,
        //     offset: (SAMPLE_RATE / 4) * 2,
        // },
        // Section<Frequency> {
        //     input: frequency_of_key(46),
        //     size: SAMPLE_RATE / 4,
        //     offset: (SAMPLE_RATE / 4) * 3,
        // },
    };

    auto&& master_generator { generator<SINE, Sample>(master_input) };

    Samples result {};

    for (uint64_t result_index { 0 }; result_index < result.size(); ++result_index)
    {
        result[result_index] = master_generator.generate(result_index, result.size());
    }

    write_samples_to("a.raw", result);

    return 0;
}
