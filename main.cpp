#include "generator.hpp"
#include "mixer.hpp"

#include <fstream>
#include <iostream>
#include <cmath>
#include <cstdint>
#include <array>
#include <vector>
#include <memory>

constexpr const uint64_t SAMPLE_RATE = 44100;
constexpr const uint64_t SAMPLES_SIZE = SAMPLE_RATE * 4;

using Sample = float;
using Samples = std::array<Sample, SAMPLES_SIZE>;

namespace mpp
{
    template <>
    struct Generator<0, Sample, float, uint64_t>
    {
        Sample generate(const float& frequency, const uint64_t& index) const {
            return std::sin((index * 2 * M_PI * frequency) / SAMPLE_RATE);
        }
    };

    template <>
    class Mixer<Sample, Sample>
    {
    public:
        Sample mix(const Sample& left, const Sample& right) const
        {
            return left + right;
        }
    };
}

void write_samples_to(const std::string& file_name, const Samples& samples)
{
    std::ofstream file(file_name, std::ios::out | std::ios::binary);

    if (!file.good()) {
        std::cerr << "cannot open " << file_name << " for writing\n";
        exit(1);
    }

    file.seekp(0);
    file.write(reinterpret_cast<const char*>(samples.data()), sizeof(float) * samples.size());
}

float interpolate(const float& min, const float& max, const float& time)
{
    return min * (1 - time) + max * time;
}

int main()
{
    Samples result;
    for (uint64_t sample_index = 0; sample_index < result.size(); ++sample_index)
    {
        const float& current_ratio = sample_index / float(result.size());

        const float& sample1_frequency = interpolate(60.f, 1200.f, current_ratio);
        const Sample& sample1 = mpp::generate<0, Sample>(sample1_frequency, sample_index);

        const float& sample2_frequency = interpolate(50.f, 2500.f, current_ratio);
        const Sample& sample2 = mpp::generate<0, Sample>(sample2_frequency, sample_index);

        result[sample_index] = mpp::mix<Sample>(sample1, sample2);
    }

    write_samples_to("a.raw", result);
    return 0;
}
