#ifndef MPP_EXPORT_HPP
#define MPP_EXPORT_HPP

#include <fstream>
#include <iostream>
#include <string>

namespace mpp
{
    void write_samples_to(std::string const& file_name, mpp::Samples const& samples)
    {
        std::ofstream file { file_name, std::ios::out | std::ios::binary };

        if (!file.good()) {
            std::cerr << "cannot open " << file_name << " for writing\n";
            exit(1);
        }

        file.seekp(0);
        file.write(reinterpret_cast<char const*>(samples.data()), sizeof(Sample) * samples.size());
    }
}

#endif
