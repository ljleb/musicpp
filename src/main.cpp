#include <iostream>

#include "generators/composite.hpp"
#include "generators/partial.hpp"
#include "generators/linear_interpolation.hpp"
#include "generators/basic.hpp"
#include "mixers/sample.hpp"

#include "generator.hpp"
#include "mixer.hpp"
#include "project_config.hpp"
#include "export.hpp"

#include <cstdint>

int main()
{
    using namespace mpp;

    const auto& master_generator { generator<SINE, Sample>(std::tuple {
        // generator<SINE, Sample>(BezierInterpolation<Frequency, 2> {
        //     {
        //         { 0, 1000 },
        //         { 1000, 0 },
        //     },
        //     {
        //         { 1000, 0 },
        //         { 0, 1000 },
        //     },
        // }),

        // generator<SINE, Sample>(Partial {
        //     generator<SINE, Sample>(LinearInterpolation<Frequency> { 200, 000 }),
        //     SAMPLE_RATE*0,
        //     SAMPLE_RATE/8,
        // }),
        // generator<SINE, Sample>(Partial {
        //     generator<SINE, Sample>(LinearInterpolation<Frequency> { 200, 000 }),
        //     SAMPLE_RATE*1,
        //     SAMPLE_RATE/8,
        // }),
        // generator<SINE, Sample>(Partial {
        //     generator<SINE, Sample>(LinearInterpolation<Frequency> { 200, 000 }),
        //     SAMPLE_RATE*2,
        //     SAMPLE_RATE/8,
        // }),
        // generator<SINE, Sample>(Partial {
        //     generator<SINE, Sample>(LinearInterpolation<Frequency> { 200, 000 }),
        //     SAMPLE_RATE*3,
        //     SAMPLE_RATE/8,
        // }),

        generator<SINE, Sample>(LinearInterpolation<Frequency> { 3200, 1600 }),
        // generator<SINE, Sample>(Frequency { 440*4 }),
        // generator<SINE, Sample>(LinearInterpolation<Frequency> { 000, 800 }),
    })};

    Samples result {};

    for (uint64_t result_index {0}; result_index < result.size(); ++result_index)
    {
        result[result_index] = master_generator.generate(result_index, result.size());
    }

    write_samples_to("a.raw", result);

    return 0;
}
