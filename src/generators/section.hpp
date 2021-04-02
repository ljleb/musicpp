#ifndef MPP_GENERATORS_PARTIAL_HPP
#define MPP_GENERATORS_PARTIAL_HPP

#include "generator.hpp"
#include <cstdint>

namespace mpp
{
    template <typename Input, uint64_t times = 1>
    struct Section
    {
        constexpr Section(
            uint64_t&& size,
            uint64_t&& offset,
            Input&& input
        ):
            _size { std::move(size) },
            _offset { std::move(offset) },
            _input { std::move(input) }
        {}

        constexpr Section(
            uint64_t&& size,
            Input&& input
        ):
            Section(std::move(size), 0, std::move(input))
        {}

        constexpr bool can_generate(const uint64_t& index) const&
        {
            return index >= _offset && index < _offset + _size * times;
        }

        constexpr const Input& input() const&
        {
            return _input;
        }

        constexpr const uint64_t& size() const&
        {
            return _size;
        }

        constexpr uint64_t relative_index(const uint64_t& index) const&
        {
            return (index - _offset) % _size;
        }

    private:
        uint64_t _size;
        uint64_t _offset;
        Input _input;
    };

    template <GeneratorShape Shape, typename Output, typename Input, uint64_t times>
    struct Generator<Shape, Output, Section<Input, times>>
    {
        constexpr Output generate(const uint64_t& index, const uint64_t& max_index) const&
        {
            if (!section.can_generate(index))
            {
                return {};
            }

            return generator<Shape, Output>(section.input())
                .generate(section.relative_index(index), section.size());
        }

        Section<Input, times>& section;
    };
}

#endif
