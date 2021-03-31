#ifndef MPP_MIXER_HPP
#define MPP_MIXER_HPP

#include <memory>

namespace mpp
{
    template <typename A>
    class Mixer
    {
    public:
        A mix(const A& left, const A& right) const&;
    };

    template <typename A>
    inline A mix(const A& left, const A& right)
    {
        const mpp::Mixer<A> mixer {};
        return mixer.mix(left, right);
    }

    template <typename A, typename... MoreInputs>
    inline A mix(const A& first, const A& second, const A& third, MoreInputs&&... more)
    {
        const A& mixed_more { mix(second, third, std::forward<MoreInputs...>(more)...) };
        return mix<A>(first, mixed_more);
    }

    template <typename A, typename Iterable>
    A mix(const Iterable& iterable)
    {
        A result {};

        for (const A& input: iterable)
        {
            result = mix(result, input);
        }

        return result;
    }
}

#endif
