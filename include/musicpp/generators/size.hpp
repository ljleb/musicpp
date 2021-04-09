#ifndef MPP_GENERATORS_SIZE_HPP
#define MPP_GENERATORS_SIZE_HPP

#include <musicpp/generator.hpp>

#include <cstdint>

namespace mpp
{
    template <typename Input>
    struct Size
    {
        constexpr Size(uint64_t const& size, Input const& input):
            _size { size },
            _input { input }
        {}

        constexpr bool can_generate(TimePoint const& time) const&
        {
            return time.index < _size;
        }

        constexpr TimePoint constrained(TimePoint const& time) const&
        {
            return { time.index, _size };
        }

        uint64_t _size;
        Input _input;
    };

    template <typename Output, typename Input>
    struct Generator<Output, Size<Input>>
    {
        constexpr Output generate_at(TimePoint const& time, Config const& config) const&
        {
            if (!_size.can_generate(time))
            {
                return {};
            }

            return generator<Output>(_size._input).generate_at(_size.constrained(time), config);
        }

        constexpr uint64_t size() const&
        {
            return _size._size;
        }

        constexpr uint64_t offset() const&
        {
            return generator<Output>(_size._input).offset();
        }

        Size<Input>& _size;
    };
}

#endif
