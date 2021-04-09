#ifndef MPP_MIXER_HPP
#define MPP_MIXER_HPP

#include <memory>

namespace mpp
{
    template <typename A>
    class Mixer
    {
    public:
        A mix(A const& left, A const& right) const&;
    };

    template <typename A>
    inline A mix(A const& left, A const& right)
    {
        const mpp::Mixer<A> mixer {};
        return mixer.mix(left, right);
    }

    template <typename A, typename... MoreInputs>
    inline A mix(A const& first, A const& second, A const& third, MoreInputs&&... more)
    {
        const A& mixed_more { mix(second, third, std::forward<MoreInputs...>(more)...) };
        return mix<A>(first, mixed_more);
    }

    template <typename A, typename Iterable>
    A mix(Iterable const& iterable)
    {
        A result {};

        for (A const& input: iterable)
        {
            result = mix(result, input);
        }

        return result;
    }
}

#endif
