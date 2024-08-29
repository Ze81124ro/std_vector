#include "bit_reference.hpp"
#include <cstddef>
#include <cstdint>

namespace {
    static const size_t BITS_IN_BYTE = 8;

    template <class Alloc>
    using bit_reference = typename Vector<bool, Alloc>::reference;

    uint8_t signed_rem(uint8_t val, int8_t sgn) {
        return sgn > 0 ? BITS_IN_BYTE - val : val;
    }

    void signed_increment(uint8_t*& byte, uint8_t& bit, size_t idx, int8_t sgn) {
        uint8_t rem = idx % BITS_IN_BYTE;
        bool additional_byte = rem > signed_rem(bit, sgn);
        byte += sgn * (idx / BITS_IN_BYTE + additional_byte);
        bit = (bit + signed_rem(rem, sgn)) % BITS_IN_BYTE;
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
constexpr auto Vector<bool, Alloc>::BitReference::operator+=(size_t idx) noexcept -> BitReference& {
    signed_increment(byte, bit, idx, 1);
    return *this;
}

template <class Alloc>
constexpr auto Vector<bool, Alloc>::BitReference::operator++() noexcept -> BitReference& {
    return *this += 1;
}

template <class Alloc>
constexpr auto Vector<bool, Alloc>::BitReference::operator++(int) noexcept -> BitReference {
    BitReference copy = *this;
    ++*this;
    return copy;
}

template <class Alloc>
constexpr auto Vector<bool, Alloc>::BitReference::operator-=(size_t idx) noexcept -> BitReference& {
    signed_increment(byte, bit, idx, -1);
    return *this;
}

template <class Alloc>
constexpr auto Vector<bool, Alloc>::BitReference::operator--() noexcept -> BitReference& {
    return *this -= 1;
}

template <class Alloc>
constexpr auto Vector<bool, Alloc>::BitReference::operator--(int) noexcept -> BitReference {
    BitReference copy = *this;
    ++*this;
    return copy;
}

template <class Alloc>
constexpr auto Vector<bool, Alloc>::BitReference::operator-(size_t idx) noexcept -> BitReference {
    BitReference copy = *this;
    copy -= idx;
    return copy;
}

template <class Alloc>
constexpr auto Vector<bool, Alloc>::BitReference::operator[](size_t idx) const noexcept -> bool {
    return *this + idx;
}

template <class Alloc>
constexpr auto Vector<bool, Alloc>::BitReference::operator[](size_t idx) noexcept -> BitReference {
    return *this + idx;
}

template <class Alloc>
constexpr auto operator+(const bit_reference<Alloc>& ref, size_t idx) noexcept 
-> bit_reference<Alloc> {
    bit_reference<Alloc> copy = ref;
    copy += idx;
    return copy;
}

template <class Alloc>
constexpr auto operator+(size_t idx, const bit_reference<Alloc>& ref) noexcept
-> bit_reference<Alloc> {
    return ref + idx;
}

template <class Alloc>
constexpr auto operator-(
    const bit_reference<Alloc>& lhs,
    const bit_reference<Alloc>& rhs
) noexcept -> ptrdiff_t {
    return (static_cast<ptrdiff_t>(lhs.byte) - rhs.byte) * BITS_IN_BYTE
    + lhs.bit - rhs.bit;
}
