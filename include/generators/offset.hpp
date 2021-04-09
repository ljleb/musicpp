#ifndef MPP_GENERATORS_OFFSET_HPP
#define MPP_GENERATORS_OFFSET_HPP

#include "generator.hpp"

#include <cstdint>

namespace mpp
{
    template <typename Input>
    struct Offset
    {
        constexpr Offset(uint64_t const& offset, Input const& input):
            _offset { offset },
            _input { input }
        {}

        constexpr bool can_generate(TimePoint const& time) const&
        {
            return _offset <= time.index;
        }

        constexpr TimePoint time_relative_to(TimePoint const& time) const&
        {
            return time - _offset;
        }

        uint64_t _offset;
        Input _input;
    };

    template <typename Output, typename Input>
    struct Generator<Output, Offset<Input>>
    {
        constexpr Output generate_at(TimePoint const& time, Config const& config) const&
        {
            if (!_offset.can_generate(time))
            {
                return {};
            }

            return generator<Output>(_offset._input).generate_at(_offset.time_relative_to(time), config);
        }

        constexpr uint64_t size() const&
        {
            return generator<Output>(_offset._input).size();
        }

        constexpr uint64_t offset() const&
        {
            return _offset._offset;
        }

        Offset<Input>& _offset;
    };
}

#endif
