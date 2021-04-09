#include <iostream>

#include "generators/mix.hpp"
#include "generators/size.hpp"
#include "generators/offset.hpp"
#include "generators/low_pass.hpp"
#include "generators/high_pass.hpp"
#include "generators/basic.hpp"
#include "mixers/sample.hpp"

#include "controls/bezier.hpp"
#include "controls/steady.hpp"

#include "generator.hpp"
#include "mixer.hpp"
#include "export.hpp"
#include "config.hpp"

#include "literals.hpp"

#include <cstdint>

int main(int const argc, char** const argv)
{
    using namespace mpp;

    Config const& config { argc, argv };

    auto const& pattern = [&](auto const& synth)
    {
        auto const& note_size = config.sample_rate / 8;
        auto const& note_offset = config.sample_rate / 4;
        return Mix
        {
            Offset { note_offset * 0, Size { note_size * 2, synth(4_A), }, },
            Offset { note_offset * 1, Size { note_size * 2, synth(4_A), }, },
            Offset { note_offset * 2, Size { note_size * 2, synth(4_A), }, },
            Offset { note_offset * 3, Size { note_size * 2, synth(5_G), }, },
            Offset { note_offset * 4, Size { note_size * 2, synth(5_C), }, },
            Offset { note_offset * 5, Size { note_size * 3, synth(4_B), }, },
            Offset { note_offset * 7, Size { note_size * 3, synth(4_G), }, },
        };
    };

    auto const& synth_a
    {
        [](auto const& frequency)
        {
            return LowPass
            {
                Bezier { frequency * 4, 0 },
                mpp::make_basic<SAW>(frequency),
            };
            // return mpp::make_basic<SAW>(frequency * 2);
        }
    };

    auto const& synth_b
    {
        [](auto const& frequency)
        {
            return LowPass
            {
                Bezier { 0, frequency * 2 },
                make_basic<SINE>
                (
                    Bezier
                    {
                        frequency / 2,
                        Bezier { 0, frequency / 2 },
                    }
                ),
            };
        }
    };

    auto&& input
    {
        Mix
        {
            pattern(synth_a),
            // pattern(synth_b),
        }
    };

    generate_and_write_samples(input, config);
    return 0;
}
