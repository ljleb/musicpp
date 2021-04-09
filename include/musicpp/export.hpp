#ifndef MPP_EXPORT_HPP
#define MPP_EXPORT_HPP

#include <musicpp/config.hpp>
#include <musicpp/time_point.hpp>

#include <fstream>
#include <iostream>
#include <string>

namespace mpp
{
    void write_samples_to(Config const& config, mpp::Samples const& samples)
    {
        std::ofstream file { config.file_name, std::ios::out | std::ios::binary };

        if (!file.good()) {
            std::cerr << "cannot open " << config.file_name << " for writing\n";
            exit(1);
        }

        file.seekp(0);
        file.write(
            reinterpret_cast<char const*>(samples.data()),
            sizeof(Sample) * samples.size());
    }

    template <typename Input>
    constexpr void generate_and_write_samples(Input& input, Config const& config)
    {
        auto&& master_generator { generator<Sample>(input) };

        Samples result {};
        uint64_t const& result_size { master_generator.size() + master_generator.offset() };

        for (uint64_t result_index { 0 }; result_index < result_size; ++result_index)
        {
            TimePoint const& time { result_index, result_size };
            result.push_back(master_generator.generate_at(time, config));
        }

        write_samples_to(config, result);
    }
}

#endif
