#ifndef MPP_UTILS_STRING_LITERAL_HPP
#define MPP_UTILS_STRING_LITERAL_HPP

#include <cstdint>

namespace mpp {
    template <std::size_t _memory_size>
    struct StringLiteral final {
        constexpr StringLiteral(char const* _value) {
            for (std::size_t i { 0 }; i < _memory_size; ++i) value[i] = _value[i];
        }

        template <std::size_t head_memory_size, std::size_t tail_memory_size>
        constexpr StringLiteral(StringLiteral<head_memory_size> const& head, StringLiteral<tail_memory_size> const& tail) {
            for (std::size_t i { 0 }; i < head.size(); ++i) value[i] = head.value[i];
            for (std::size_t i { 0 }; i < tail.size(); ++i) value[i + head.size()] = tail.value[i];
            value[_memory_size - 1] = '\0';
        }

        constexpr operator char const*() const {
            return value;
        }

        constexpr std::size_t size() const {
            return _memory_size - 1;
        }

        char value[_memory_size];
    };

    template<std::size_t memory_size>
    StringLiteral(char const(&)[memory_size])
        -> StringLiteral<memory_size>;

    template<std::size_t head_memory_size, std::size_t tail_memory_size>
    StringLiteral(StringLiteral<head_memory_size> const&, StringLiteral<tail_memory_size> const&)
        -> StringLiteral<head_memory_size + tail_memory_size - 1>;

    template <StringLiteral head, StringLiteral last>
    constexpr auto concat() {
        return StringLiteral { head, last };
    }

    template <StringLiteral head, StringLiteral... tail>
    constexpr auto concat() requires(sizeof...(tail) > 1) {
        return concat<head, concat<tail...>()>();
    }
}

#endif
