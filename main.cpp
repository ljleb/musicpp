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
using Sample = float;
using Samples = std::vector<Sample>;

namespace mpp
{
    template <>
    struct Generator<"sin", Samples, float, uint64_t>
    {
        std::unique_ptr<const Samples> generate(const float& scale, const uint64_t& sample_size) const
        {
            std::unique_ptr<Samples> samples = std::make_unique<Samples>();

            for(uint64_t i = 0; i < sample_size; ++i)
            {
                samples->emplace_back(process_sample(i, scale, sample_size));
            }

            return samples;
        }

    private:
        float process_sample(const uint64_t& index, const float& scale, const uint64_t& samplesSize) const
        {
            const auto& ratio = (1 + index / double(samplesSize)) * scale;
            return std::sin(index * 2 * M_PI / (100 + ratio * ratio));
        }
    };

    template <typename Inputs>
    class Mixer<Samples, Inputs>
    {
    public:
        std::unique_ptr<const Samples> mix(const Inputs& inputs) const
        {
            std::unique_ptr<Samples> result = new_samples_from_size_of(inputs);

            for (const Samples& input: inputs)
            {
                for (uint64_t i = 0; i < input.size(); ++i)
                {
                    (*result)[i] = ((*result)[i] + input[i]);
                }
            }

            return result;
        }

    private:
        std::unique_ptr<Samples> new_samples_from_size_of(const Inputs& inputs) const
        {
            uint64_t max_size = 0;

            for (const Samples& input: inputs)
            {
                max_size = std::max(input.size(), max_size);
            }

            return std::make_unique<Samples>(max_size);
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

int main()
{
    std::unique_ptr<const Samples> samples1 = mpp::generate<"sin", Samples>(2.5f, SAMPLE_RATE * 2);
    std::unique_ptr<const Samples> samples2 = mpp::generate<"sin", Samples>(5.f, SAMPLE_RATE * 2);
    std::unique_ptr<const Samples> samples3 = mpp::mix<Samples>(std::array<Samples, 2> {
        *samples1,
        *samples2
    });

    write_samples_to("a.raw", *samples3);
    return 0;
}
