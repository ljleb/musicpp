#ifndef MPP_GENERATORS_VOLUME_HPP
#define MPP_GENERATORS_VOLUME_HPP

#include <musicpp/generator.hpp>

namespace mpp
{
    template <typename VolumeControl, typename Input>
    struct Volume
    {
        constexpr Volume(VolumeControl const& volume, Input const& input):
            _volume { volume },
            _input { input }
        {}

        VolumeControl _volume;
        Input _input;
    };

    template <typename VolumeControl, typename Input>
    struct Generator<Sample, Volume<VolumeControl, Input>>
    {
        constexpr Sample generate_at(TimePoint const& time, Config const& config) const&
        {
            auto const& volume { interpolate_control(_volume._volume, time) };

            return volume * generator<Sample>(_volume._input).generate_at(time, config);
        }

        constexpr uint64_t size() const&
        {
            return generator<Sample>(_volume._input).size();
        }

        constexpr uint64_t offset() const&
        {
            return generator<Sample>(_volume._input).offset();
        }

        Volume<VolumeControl, Input>& _volume;
    };
}

#endif
