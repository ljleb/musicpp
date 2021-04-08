#ifndef MPP_CONFIG_HPP
#define MPP_CONFIG_HPP

#include "utils.hpp"

#include <vector>
#include <string>
#include <cstdint>

namespace mpp
{
    using Sample = float;
    using Samples = std::vector<mpp::Sample>;

    struct Config
    {
        constexpr Config(int const& argc, char** const& argv):
            file_name { argv[1] },
            sample_rate { stoi(argv[2]) }
        {}

        char* const& file_name;
        uint64_t const sample_rate;
    };
}

#endif
