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
                Steady { SAMPLE_RATE * 2 },
                Steady { 0 },
                Steady { 4 },
                Mix
                {
                    Section
                    {
                        Steady { note_size * 2 },
                        Steady { note_offset * 7 },
                        synth(3_G),
                    },
                    Section
                    {
                        Steady { note_size * 2 },
                        Steady { note_offset * 5 },
                        synth(3_B),
                    },
                    Section
                    {
                        Steady { note_size * 1 },
                        Steady { note_offset * 4 },
                        synth(4_C),
                    },
                    Section
                    {
                        Steady { note_size * 1 },
                        Steady { note_offset * 3 },
                        synth(4_G),
                    },
                    Section
                    {
                        Steady { note_size * 1 },
                        Steady { note_offset * 2 },
                        synth(3_A),
                    },
                    Section
                    {
                        Steady { note_size * 1 },
                        Steady { note_offset * 1 },
                        synth(3_A),
                    },
                    Section
                    {
                        Steady { note_size * 1 },
                        Steady { note_offset * 0 },
                        synth(3_A),
                    },
                }
            };
    };

    auto&& synth_a = [](const auto& frequency)
    {
        return LowPass
        {
            Bezier { frequency, 0.0_steady },
            BasicSaw { frequency },
        };
    };

    auto&& synth_b = [](const auto& frequency)
    {
        return LowPass
        {
            Bezier { frequency, Bezier { frequency, 0.0_steady } },
            BasicSine
            {
                Bezier
                {
                    frequency,
                    Bezier { 0.0_steady, frequency },
                },
            },
        };
    };

    auto&& master_input = Mix
    {
        pattern(synth_a),
        // pattern(synth_b),
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
