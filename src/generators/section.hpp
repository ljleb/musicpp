#ifndef MPP_GENERATORS_PARTIAL_HPP
#define MPP_GENERATORS_PARTIAL_HPP

#include "controls/steady.hpp"

#include "generator.hpp"
#include <cstdint>

namespace mpp
{
    template <typename SizeControl, typename OffsetControl, typename RepeatControl, typename Input>
    struct Section
    {
        constexpr Section(
            const OffsetControl& offset,
            const SizeControl& size,
            const RepeatControl& repeat,
            const Input& input
        ):
            _offset { offset },
            _size { size },
            _repeat { repeat },
            _input { input }
        {}

        constexpr Section(
            const OffsetControl& offset,
            const SizeControl& size,
            const Input& input
        ):
            Section<SizeControl, OffsetControl, uint64_t, Input>
            { offset, size, 1, input }
        {}

        constexpr Section(
            const SizeControl& size,
            const Input& input
        ):
            Section<SizeControl, uint64_t, uint64_t, Input>
            { 0, size, input }
        {}

        constexpr bool can_generate(const TimePoint& time) const&
        {
            const auto& size { interpolate_control(_size, time) };
            const auto& offset { interpolate_control(_offset, time) };
            const auto& repeat { interpolate_control(_repeat, time) };

            return time.index >= offset && time.index < offset + size * repeat;
        }

        constexpr Input& input()
        {
            return _input;
        }

        constexpr TimePoint time_relative_to(const TimePoint& dry_time) const&
        {
            const auto& size { interpolate_control(_size, dry_time) };
            const auto& offset { interpolate_control(_offset, dry_time) };
            return { (dry_time.index - offset) % size, static_cast<uint64_t>(size) };
        }

    private:
        OffsetControl _offset;
        SizeControl _size;
        RepeatControl _repeat;
        Input _input;
    };

    template <typename SizeControl, typename OffsetControl, typename Input>
    Section(const SizeControl&, const OffsetControl&, const Input&) ->
        Section<SizeControl, OffsetControl, uint64_t, Input>;

    template <typename SizeControl, typename Input>
    Section(const SizeControl&, const Input&) ->
        Section<SizeControl, uint64_t, uint64_t, Input>;

    template <typename Output, typename SizeControl, typename OffsetControl, typename RepeatControl, typename Input>
    struct Generator<Output, Section<SizeControl, OffsetControl, RepeatControl, Input>>
    {
        constexpr Output generate(const TimePoint& time) const&
        {
            if (!section.can_generate(time))
            {
                return {};
            }

            return generator<Output>(section.input())
                .generate(section.time_relative_to(time));
        }

        Section<SizeControl, OffsetControl, RepeatControl, Input>& section;
    };
}

#endif
