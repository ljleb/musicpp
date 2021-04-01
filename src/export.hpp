#ifndef MPP_EXPORT_HPP
#define MPP_EXPORT_HPP

#include "project_config.hpp"

#include <fstream>
#include <iostream>
#include <string>

namespace mpp
{
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
}

#endif