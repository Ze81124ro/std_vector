#include <cstdint>
#include "bit_reference.hpp"

namespace {
    void signed_increment(uint8_t*& byte, uint8_t& bit, size_t idx, int8_t sgn) {
        static const size_t BITS_IN_BYTE = 8;
        size_t bytes_cnt = idx / BITS_IN_BYTE;
        size_t bits_cnt = idx % BITS_IN_BYTE;
        byte += (bytes_cnt + (bits_cnt + bit) > BITS_IN_BYTE) * sgn;
        bit = (bit + (bits_cnt + bit) % BITS_IN_BYTE * sgn) % BITS_IN_BYTE;
    }
}

template <class Alloc>
constexpr Vector<bool, Alloc>::BitReference::BitReference(uint8_t* byte, uint8_t bit) noexcept
: byte(byte), bit(bit) {}

template <class Alloc>
constexpr auto Vector<bool, Alloc>::BitReference::operator=(bool val) noexcept -> BitReference& {
    *byte += val << bit - *byte & (1 << bit);
    return *this;
}

template <class Alloc>
constexpr Vector<bool, Alloc>::BitReference::operator bool() const noexcept {
    return *byte & (1 << bit) >> bit;
}

template <class Alloc>
constexpr auto operator+=(size_t idx) noexcept -> BitReference& {
    signed_increment(byte, bit, idx, 1);
    return *this;
}

template <class Alloc>
constexpr auto operator++() noexcept -> BitReference& {
    return *this += 1;
}

template <class Alloc>
constexpr auto operator++(int) noexcept -> BitReference {
    BitReference copy = *this;
    ++*this;
    return copy;
}

template <class Alloc>
constexpr auto operator-=(size_t idx) noexcept -> BitReference& {
    signed_increment(byte, bit, idx, -1);
    return *this;
}
