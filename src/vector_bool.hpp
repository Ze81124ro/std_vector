#ifndef VECTOR_BOOL_H

#define VECTOR_BOOL_H

#include <cstddef>
#include <initializer_list>
#include <iterator>
#include <memory>

template <class T, class Alloc>
class Vector;

template <class Alloc>
class Vector<bool, Alloc> {
private:
    class BitReference;
public:
    using value_type = bool;
    using allocator_type = Alloc;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using reference = BitReference;
    using const_reference = bool;
    using pointer = BitReference;
    using const_pointer = const pointer;
    using iterator = BitReference;
    using const_iterator = const iterator;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = const reverse_iterator;
    constexpr Vector() noexcept(noexcept(Alloc()));
    explicit constexpr Vector(const Alloc& alloc) noexcept;
    constexpr Vector(size_t sz, const T& val, const Alloc& alloc);
    constexpr Vector(size_t sz, const T& val);
    constexpr Vector(size_t sz);
    constexpr Vector(size_t sz, const Alloc& alloc);
    template <class InputIt>
    constexpr Vector(InputIt first, InputIt last, const Alloc& alloc);
    template <class InputIt>
    constexpr Vector(InputIt first, InputIt last);
    constexpr Vector(const Vector& other);
    constexpr Vector(const Vector& other, const Alloc& alloc);
    constexpr Vector(Vector&& other) noexcept;
    constexpr Vector(Vector&& other, const Alloc& alloc);
    constexpr Vector(std::initializer_list<bool> init, const Alloc& alloc);
    constexpr Vector(std::initializer_list<bool> init);
private:
    using AllocTraits = std::allocator_traits<Alloc>::rebind_traits<uint8_t>;
    static const size_t INITIAL_CAP = 4;
    static const size_t GROWTH_RATE = 2;
    static const size_t BITS_IN_BYTE = 8;
    Alloc alloc;
    size_t sz = 0;
    size_t cap = INITIAL_CAP;
    uint8_t* ptr = nullptr;
};

#endif
