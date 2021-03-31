#include "generators/composite.hpp"
#include "generators/linear_interpolation.hpp"
#include "generators/basic.hpp"
#include "mixers/sample.hpp"

#include "generator.hpp"
#include "mixer.hpp"
#include "project_config.hpp"
#include "export.hpp"

#include <cstdint>

// NEXT TIME:
// - implement clean "partial playback"

int main()
{
    using namespace mpp;

    const auto& master_generator { generator<SINE, Sample>(std::tuple {
        generator<SINE, Sample, LinearInterpolation<Frequency>>({ 60.f, 1400.f }),
        generator<SINE, Sample, LinearInterpolation<Frequency>>({ 40.f, 1600.f }),
    })};

    Samples result {};

    for (uint64_t result_index {0}; result_index < result.size(); ++result_index)
    {
        result[result_index] = master_generator.generate(result_index, result.size());
    }

    write_samples_to("a.raw", result);

    return 0;
}
