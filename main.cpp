#include "generator.hpp"
#include "mixer.hpp"
#include "project_config.hpp"
#include "tuple_foreach.hpp"

#include <fstream>
#include <iostream>
#include <vector>
#include <cmath>
#include <cstdint>

namespace mpp
{
    template <GeneratorKind Kind, typename Output, typename... Generators>
    struct Generator<Kind, Output, std::tuple<Generators...>>
    {
        Output generate(const uint64_t& index, const uint64_t& max_index) const&
        {
            std::vector<Output> results_to_mix;

            for_each(generators, [&results_to_mix, &index, &max_index](const auto& generator)
            {
                results_to_mix.emplace_back(generator.generate(index, max_index));
            });

            return mix<Sample>(results_to_mix);
        }

        std::tuple<Generators...> generators;
    };

    template <GeneratorKind Kind, typename Output>
    struct Generator<Kind, Output, float, float>
    {
        Output generate(const uint64_t& index, const uint64_t& max_index) const&
        {
            const float& time = index / float(max_index);
            const float& frequency = min_frequency * (1 - time) + max_frequency * time;
            Generator<Kind, Output, float> generator { frequency };
            return generator.generate(index, max_index);
        }

        float min_frequency;
        float max_frequency;
    };

    template <>
    struct Generator<SINE, Sample, float>
    {
        Sample generate(const uint64_t& index, const uint64_t& max_index) const&
        {
            return std::sin((index * 2 * M_PI * frequency) / SAMPLE_RATE);
        }

        float frequency;
    };

    template <>
    class Mixer<Sample>
    {
    public:
        Sample mix(const Sample& left, const Sample& right) const&
        {
            return left + right;
        }
    };
}

void write_samples_to(const std::string& file_name, const mpp::Samples& samples)
{
    std::ofstream file { file_name, std::ios::out | std::ios::binary };

    if (!file.good()) {
        std::cerr << "cannot open " << file_name << " for writing\n";
        exit(1);
    }

    file.seekp(0);
    file.write(reinterpret_cast<const char*>(samples.data()), sizeof(float) * samples.size());
}

int main()
{
    mpp::Samples result {};

    const auto& master_generator { mpp::generator<mpp::SINE, mpp::Sample>(std::tuple {
        mpp::generator<mpp::SINE, mpp::Sample>(60.f, 1400.f),
        mpp::generator<mpp::SINE, mpp::Sample>(40.f, 1600.f),
    })};

    for (uint64_t result_index {0}; result_index < result.size(); ++result_index)
    {
        result[result_index] = master_generator.generate(result_index, result.size());
    }

    write_samples_to("a.raw", result);
    return 0;
}
