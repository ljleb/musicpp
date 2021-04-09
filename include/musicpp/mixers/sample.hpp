#ifndef MPP_MIXERS_SAMPLE_HPP
#define MPP_MIXERS_SAMPLE_HPP

#include <musicpp/mixer.hpp>

namespace mpp
{
    template <>
    class Mixer<Sample>
    {
    public:
        Sample mix(Sample const& left, Sample const& right) const&
        {
            return left + right;
        }
    };
}

#endif
