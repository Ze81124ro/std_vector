#ifndef BIT_REFERENCE_H

#define BIT_REFERENCE_H

#include <cstdint>
#include <compare>
#include <iterator>
#include "vector_bool.hpp"

template <class Alloc>
class Vector<bool, Alloc>::BitReference {
public:
    using difference_type = ptrdiff_t;
    using value_type = bool;
    using pointer = BitReference;
    using reference = BitReference;
    using iterator_category = std::random_access_iterator_tag;
    constexpr BitReference(uint8_t* byte, uint8_t bit) noexcept;
    constexpr BitReference& operator=(bool val) noexcept;
    constexpr operator bool() const noexcept;
    constexpr BitReference& operator+=(size_t idx) noexcept;
    constexpr BitReference& operator++() noexcept;
    constexpr BitReference operator++(int) noexcept;
    constexpr BitReference& operator-=(size_t idx) noexcept;
    constexpr BitReference& operator--() noexcept;
    constexpr BitReference operator--(int) noexcept;
    constexpr BitReference operator-(size_t idx) noexcept;
    constexpr bool operator[](size_t idx) const noexcept;
    constexpr BitReference operator[](size_t idx) noexcept;
    constexpr std::weak_ordering operator<=>(const BitReference& other) const noexcept = default;
private:
    static const size_t BITS_IN_BYTE = 8;
    uint8_t* byte;
    uint8_t bit;
};

namespace {
    template <class Alloc>
    using bit_reference = typename Vector<bool, Alloc>::reference;
}

template <class Alloc>
constexpr bit_reference<Alloc> operator+(const bit_reference<Alloc>& ref, size_t idx) noexcept;

template <class Alloc>
constexpr bit_reference<Alloc> operator+(size_t idx, const bit_reference<Alloc>& ref) noexcept;

template <class Alloc>
constexpr ptrdiff_t operator-(
    const bit_reference<Alloc>& lhs, 
    const bit_reference<Alloc>& rhs
) noexcept;

#endif
