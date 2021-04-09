#ifndef MPP_MAIN_HPP
#define MPP_MAIN_HPP

#include "export.hpp"
#include "config.hpp"

namespace mpp
{
    template <typename Input>
    constexpr int main(Input& input, Config const& config)
    {
        generate_and_write_samples(input, config);
        return 0;
    }
}

#endif
