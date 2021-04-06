#ifndef MPP_EXPORT_HPP
#define MPP_EXPORT_HPP

#include "project_config.hpp"

#include <fstream>
#include <iostream>
#include <string>

namespace mpp
{
    struct WavData
    {
        uint32_t chunk_id = 0x52494646;
        uint32_t chunk_size;
        uint32_t format = 0x57415645;
        
        uint32_t sub_chunk1_id = 0x666d7420;

    };

    void write_samples_to(const std::string& file_name, const mpp::Samples& samples)
    {
        const uint32_t& samples_byte_size = sizeof(Sample) * samples.size();

        std::ofstream file { file_name, std::ios::out | std::ios::binary };

        if (!file.good()) {
            std::cerr << "cannot open " << file_name << " for writing\n";
            exit(1);
        }

        file.seekp(0, std::ios::beg);

        {
            file.write("RIFF", 4);

            const uint32_t& chunk_size { 42 + samples_byte_size + ((samples_byte_size % 2) == 1) };
            file.write(reinterpret_cast<const char*>(&chunk_size), 4);

            file.write("WAVE", 4);
        }

        {
            file.write("fmt ", 4);

            const uint32_t sub_chunk_size { 18 };
            file.write(reinterpret_cast<const char*>(&sub_chunk_size), 4);

            const uint16_t audio_format { 3 };
            file.write(reinterpret_cast<const char*>(&audio_format), 2);

            const uint16_t num_channels { 1 };
            file.write(reinterpret_cast<const char*>(&num_channels), 2);

            const uint32_t sample_rate { SAMPLE_RATE };
            file.write(reinterpret_cast<const char*>(&sample_rate), 4);

            const uint32_t byte_rate { sample_rate * num_channels * sizeof(Sample) };
            file.write(reinterpret_cast<const char*>(&byte_rate), 4);

            const uint16_t block_align { num_channels * sizeof(Sample) };
            file.write(reinterpret_cast<const char*>(&block_align), 2);

            const uint16_t sample_size { 8 * sizeof(Sample) };
            file.write(reinterpret_cast<const char*>(&sample_size), 2);

            const uint16_t cb_size { 0 };
            file.write(reinterpret_cast<const char*>(&cb_size), 2);
        }

        {
            file.write("data", 4);

            const uint32_t sub_chunk_size { samples_byte_size };
            file.write(reinterpret_cast<const char*>(&sub_chunk_size), 4);

            file.write(reinterpret_cast<const char*>(samples.data()), samples_byte_size);

            const bool padding { (samples_byte_size % 2) == 1 };
            file.write("\0", padding);
        }
    }
}

#endif
