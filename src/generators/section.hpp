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
            const SizeControl& size,
            const OffsetControl& offset,
            const RepeatControl& repeat,
            const Input& input
        ):
            _size { size },
            _offset { offset },
            _repeat { repeat },
            _input { input }
        {}

        constexpr Section(
            const SizeControl& size,
            const OffsetControl& offset,
            const Input& input
        ):
            Section<SizeControl, OffsetControl, Steady<uint64_t>, Input>
            { size, offset, Steady<uint64_t> { 1 }, input }
        {}

        constexpr Section(
            const SizeControl& size,
            const Input& input
        ):
            Section<SizeControl, Steady<uint64_t>, Steady<uint64_t>, Input>
            { size, Steady<uint64_t> { 0 }, input }
        {}

        constexpr bool can_generate(const TimePoint& time) const&
        {
            const auto& size { _size.interpolate_control(time) };
            const auto& offset { _offset.interpolate_control(time) };
            const auto& repeat { _repeat.interpolate_control(time) };

            return time.index >= offset && time.index < offset + size * repeat;
        }

        constexpr Input& input()
        {
            return _input;
        }

        constexpr TimePoint time_relative_to(const TimePoint& dry_time) const&
        {
            const auto& size { _size.interpolate_control(dry_time) };
            const auto& offset { _offset.interpolate_control(dry_time) };
            return { (dry_time.index - offset) % size, size };
        }

    private:
        SizeControl _size;
        OffsetControl _offset;
        RepeatControl _repeat;
        Input _input;
    };

    template <typename SizeControl, typename OffsetControl, typename Input>
    Section(const SizeControl&, const OffsetControl&, const Input&) ->
        Section<SizeControl, OffsetControl, Steady<uint64_t>, Input>;

    template <typename SizeControl, typename Input>
    Section(const SizeControl&, const Input&) ->
        Section<SizeControl, Steady<uint64_t>, Steady<uint64_t>, Input>;

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
