#ifndef VECTOR_H

#define VECTOR_H

#include <cstddef>
#include <iterator>
#include <memory>

template <class T, class Alloc = std::allocator<T>>
class Vector {
public:
    using value_type = T;
    using allocator_type = Alloc;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using reference = T&;
    using const_reference = const reference;
    using pointer = T*;
    using const_pointer = const pointer;
    using iterator = pointer;
    using const_iterator = const iterator;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = const reverse_iterator;
    constexpr Vector() noexcept(noexcept(Alloc()));
    explicit constexpr Vector(const Alloc& alloc) noexcept;
    constexpr Vector(size_t sz, const T& val, const Alloc& alloc);
    constexpr Vector(size_t sz, const T& val);
    constexpr Vector(size_t sz, const Alloc& alloc);
    explicit constexpr Vector(size_t sz);
    template <class InputIt>
    constexpr Vector(InputIt first, InputIt last, const Alloc& alloc);
    template <class InputIt>
    constexpr Vector(InputIt first, InputIt last);
    constexpr T* begin() noexcept;
    constexpr T* end() noexcept;
    constexpr const T* cbegin() const noexcept;
    constexpr const T* cend() const noexcept;
    constexpr reverse_iterator rbegin() noexcept;
    constexpr reverse_iterator rend() noexcept;
    constexpr const reverse_iterator crbegin() const noexcept;
    constexpr const reverse_iterator crend() const noexcept;
private:
    using AllocTraits = std::allocator_traits<Alloc>;
    const size_t INITIAL_CAP = 4;
    const size_t GROWTH_RATE = 2;
    size_t sz;
    size_t cap = INITIAL_CAP;
    const Alloc& alloc;
    T* ptr = nullptr;
};

#endif
