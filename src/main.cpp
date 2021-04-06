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

int main(int argc, char* argv[])
{
    using namespace mpp;

    auto&& pattern = [](const auto& synth)
    {
        const auto& note_size = SAMPLE_RATE / 8;
        const auto& note_offset = SAMPLE_RATE / 4;
        return 
            Section
            {
                0,
                SAMPLE_RATE * 2,
                2,
                Mix
                {
                    Section
                    {
                        note_offset * 7,
                        note_size * 2,
                        synth(4_G),
                    },
                    Section
                    {
                        note_offset * 5,
                        note_size * 2,
                        synth(4_B),
                    },
                    Section
                    {
                        note_offset * 4,
                        note_size * 1,
                        synth(5_C),
                    },
                    Section
                    {
                        note_offset * 3,
                        note_size * 1,
                        synth(5_G),
                    },
                    Section
                    {
                        note_offset * 2,
                        note_size * 1,
                        synth(4_A),
                    },
                    Section
                    {
                        note_offset * 1,
                        note_size * 1,
                        synth(4_A),
                    },
                    Section
                    {
                        note_offset * 0,
                        note_size * 1,
                        synth(4_A),
                    },
                }
            };
    };

    auto&& synth_a = [](const auto& frequency)
    {
        return LowPass
        {
            Bezier { frequency * 2, 0 },
            mpp::make_basic<SAW>(frequency * 2),
        };
    };

    auto&& synth_b = [](const auto& frequency)
    {
        return LowPass
        {
            Bezier { frequency, Bezier { frequency, 0 } },
            make_basic<SINE>
            (
                Bezier
                {
                    frequency / 2,
                    Bezier { 0, frequency / 2 },
                }
            ),
        };
    };

    auto&& master_input = Mix
    {
        pattern(synth_a),
        pattern(synth_b),
    };

    auto&& master_generator { generator<Sample>(master_input) };

    Samples result {};

    for (uint64_t result_index { 0 }; result_index < result.size(); ++result_index)
    {
        const TimePoint& time { result_index, result.size() };
        result[result_index] = master_generator.generate(time);
    }

    write_samples_to(argv[1], result);

    return 0;
}
