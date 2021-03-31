#ifndef MPP_MIXERS_SAMPLE_HPP
#define MPP_MIXERS_SAMPLE_HPP

#include "mixer.hpp"
#include "project_config.hpp"

namespace mpp
{
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

#endif
