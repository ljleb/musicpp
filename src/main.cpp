#include <iostream>

#include "generators/composite.hpp"
#include "generators/section.hpp"
#include "generators/bezier.hpp"
#include "generators/low_pass.hpp"
#include "generators/high_pass.hpp"
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

    auto&& master_input = std::tuple {
        Section<Bezier<Basic<SINE>>, 1> {
            SAMPLE_RATE * 1,
            { Frequency::of_key(49), { 0 } },
        },
    };

    auto&& master_generator { generator<Sample>(master_input) };

    Samples result {};

    for (uint64_t result_index { 0 }; result_index < result.size(); ++result_index)
    {
        result[result_index] = master_generator.generate(result_index, result.size());
    }

    write_samples_to("a.raw", result);

    return 0;
}
