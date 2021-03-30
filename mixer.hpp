#ifndef MPP_MIXER_HPP
#define MPP_MIXER_HPP

#include <memory>

namespace mpp
{
    template <typename Output, typename Inputs>
    class Mixer
    {
    public:
        Output mix(const Inputs& inputs) const;
    };

    template <typename Output, typename Inputs>
    inline std::unique_ptr<const Output> mix(const Inputs& inputs)
    {
        mpp::Mixer<Output, Inputs> mixer;
        return mixer.mix(inputs);
    }
}

#endif
