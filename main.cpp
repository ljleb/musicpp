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
    struct SinSamples : public Samples
    {};

    template <>
    struct Generator<SinSamples, float, float, uint64_t>
    {
        std::unique_ptr<const SinSamples> generate(
            const float& start_frequency,
            const float& end_frequency,
            const uint64_t& sample_size
        ) const {
            std::unique_ptr<SinSamples> samples = std::make_unique<SinSamples>();

            for(uint64_t i = 0; i < sample_size; ++i)
            {
                samples->emplace_back(generate_sample(i, start_frequency, end_frequency, sample_size));
            }

            return samples;
        }

    private:
        float generate_sample(
            const uint64_t& index,
            const float& start_frequency,
            const float& end_frequency,
            const uint64_t& sample_size
        ) const {
            const float& current_ratio = index / float(sample_size);
            const float& current_frequency =
                (start_frequency * (1 - current_ratio)) + (end_frequency * current_ratio);
            return std::sin((index * 2 * M_PI * current_frequency) / SAMPLE_RATE);
        }
    };

    template <typename Input>
    class Mixer<Samples, Input>
    {
    public:
        std::unique_ptr<const Samples> mix(const Input& left, const Input& right) const
        {
            std::unique_ptr<Samples> result = std::make_unique<Samples>(
                std::max(left->size(), right->size()));

            for (uint64_t i = 0; i < left->size(); ++i)
            {
                (*result)[i] = (*left)[i];
            }

            for (uint64_t i = 0; i < right->size(); ++i)
            {
                (*result)[i] += (*right)[i];
            }

            return result;
        }
    };
}

void write_samples_to(const std::string& file_name, const std::unique_ptr<const Samples>& samples)
{
    std::ofstream file(file_name, std::ios::out | std::ios::binary);

    if (!file.good()) {
        std::cerr << "cannot open " << file_name << " for writing\n";
        exit(1);
    }

    file.seekp(0);
    file.write(reinterpret_cast<const char*>(samples->data()), sizeof(float) * samples->size());
}

int main()
{
    std::unique_ptr<const Samples> samples1 = mpp::generate<mpp::SinSamples>(60.f, 1200.f, SAMPLE_RATE * 2);
    std::unique_ptr<const Samples> samples2 = mpp::generate<mpp::SinSamples>(50.f, 2500.f, SAMPLE_RATE * 2);
    std::unique_ptr<const Samples> samples3 = mpp::mix<Samples>(
        samples1,
        samples2);

    write_samples_to("a.raw", samples3);
    return 0;
}
