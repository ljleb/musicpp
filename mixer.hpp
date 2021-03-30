#ifndef MPP_MIXER_HPP
#define MPP_MIXER_HPP

#include <memory>

namespace mpp
{
    template <typename Output, typename Input>
    class Mixer
    {
    public:
        std::unique_ptr<const Output> mix(const Input& left, const Input& right) const;
    };

    template <typename Output, typename LeftInput, typename RightInput>
    inline std::unique_ptr<const Output> mix(const LeftInput& left, const RightInput& right)
    {
        const mpp::Mixer<Output, std::common_type_t<LeftInput, RightInput>> mixer;
        return mixer.mix(left, right);
    }

    template <typename Output, typename FirstInput, typename SecondInput, typename ThirdInput, typename... MoreInputs>
    inline std::unique_ptr<const Output> mix(FirstInput&& first, SecondInput&& second, ThirdInput third, MoreInputs&&... more)
    {
        const std::unique_ptr<const Output>& mixed_more = mix(
            std::forward<SecondInput>(second),
            std::forward<ThirdInput>(third),
            std::forward<MoreInputs...>(more)...);
        return mix<Output>(std::forward<FirstInput>(first), mixed_more);
    }
}

#endif
