#include "vector.hpp"
#include <cstddef>
#include <cstdio>
#include <initializer_list>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <utility>

static const char* OUT_OF_RANGE_MSG = "Index %zu is greater or equal to Vector size %zu in Vector::at.";

template <class T, class Alloc>
constexpr auto copy_allocator(const Vector<T, Alloc>& vec) -> Alloc {
    using AllocTraits = std::allocator_traits<Alloc>;
    return AllocTraits::select_on_container_copy_construction(vec.get_allocator());
}

template <class T, class Alloc>
constexpr auto destroy_before(Alloc& alloc, T* buf, size_t cap, size_t i) -> void {
    while (i) {
        --i;
        try {
            AllocTraits::destroy(alloc, ptr + i);
        } catch(...) {
            break;
        }
    }
    try {
        AllocTraits::deallocate(alloc, buf, cap);
    } catch(...) {}
}

template <class T, class Alloc>
constexpr auto reserve_strict(Alloc& alloc, T*& ptr, size_t& cap, size_t new_cap) -> void {
    using AllocTraits = std::allocator_traits<Alloc>;
    T* buf = AllocTraits::allocate(alloc, new_cap);
    for (size_t i = 0; i < sz; ++i) {
        try {
            AllocTraits::construct(alloc, buf + i, ptr[i]);
        } catch(...) {
            destroy_before(alloc, buf, new_cap, i);
            throw;
        }
    }
    try {
        AllocTraits::deallocate(alloc, ptr, cap);
    } catch(...) {
        destroy_before(alloc, buf, new_cap, sz);
        throw;
    }
    ptr = buf;
    cap = new_cap;
}

template <class InputIt, class Alloc>
Vector(InputIt first, InputIt last, const Alloc& alloc) 
-> Vector<typename std::iterator_traits<InputIt>::value_type, Alloc>;

template <class T, class Alloc> 
constexpr Vector<T, Alloc>::Vector(const Alloc& alloc) noexcept
: sz(0), cap(0), alloc(alloc) {}

template <class T, class Alloc>
constexpr Vector<T, Alloc>::Vector() noexcept(noexcept(Alloc()))
: Vector(Alloc()) {}

template <class T, class Alloc>
constexpr Vector<T, Alloc>::Vector(size_t sz, const T& val, const Alloc& alloc)
: sz(sz), alloc(alloc) {
    while (sz > cap)
        cap *= GROWTH_RATE;
    ptr = AllocTraits::allocate(alloc, cap);
    for (size_t i = 0; i < sz; ++i) {
        try {
            AllocTraits::construct(alloc, ptr + i, val);
        } catch(...) {
            destroy_before(alloc, ptr, cap, i);
            throw;
        }
    }
}

template <class T, class Alloc>
constexpr Vector<T, Alloc>::Vector(size_t sz, const T& val)
: Vector(sz, val, Alloc()) {}

template <class T, class Alloc>
constexpr Vector<T, Alloc>::Vector(size_t sz, const Alloc& alloc)
: Vector(sz, T(), alloc) {}

template <class T, class Alloc>
constexpr Vector<T, Alloc>::Vector(size_t sz)
: Vector(sz, T(), Alloc()) {}

template <class T, class Alloc>
template <class InputIt>
constexpr Vector<T, Alloc>::Vector(InputIt first, InputIt last, const Alloc& alloc)
: alloc(alloc) {
    sz = std::distance(first, last);
    while (sz > cap)
        cap *= GROWTH_RATE;
    ptr = AllocTraits::allocate(alloc, cap);
    size_t i = 0;
    try {
        while (first != last) {
            AllocTraits::construct(alloc, ptr + i, *first);
            ++i;
            ++first;
        }
    } catch(...) {
        destroy_before(alloc, ptr, cap, i);
        throw;
    }
}

template <class T, class Alloc>
template <class InputIt>
constexpr Vector<T, Alloc>::Vector(InputIt first, InputIt last)
: Vector(first, last, Alloc()) {}

template <class T, class Alloc>
constexpr Vector<T, Alloc>::Vector(std::initializer_list<T> init, const Alloc& alloc)
: Vector(init.begin(), init.end(), alloc) {}

template <class T, class Alloc>
constexpr Vector<T, Alloc>::Vector(std::initializer_list<T> init)
: Vector(init, Alloc()) {}

template <class T, class Alloc>
constexpr Vector<T, Alloc>::Vector(const Vector& other)
: sz(other.sz), cap(other.cap), 
alloc(other.copy_allocator()), 
ptr(AllocTraits::allocate(alloc, cap)) {
    for (size_t i = 0; i < sz; ++i) {
        try {
            AllocTraits::construct(alloc, ptr + i, other.ptr[i]);
        } catch(...) {
            destroy_before(alloc, ptr, cap, i);
            throw;
        }
    }
}

template <class T, class Alloc>
constexpr Vector<T, Alloc>::Vector(Vector&& other) noexcept
: Vector() {
    swap(other);
}

template <class T, class Alloc>
Vector<T, Alloc>::~Vector() {
    destroy_before(alloc, ptr, cap, sz);
}

template <class T, class Alloc>
constexpr auto Vector<T, Alloc>::operator=(const Vector& other) -> Vector& {
    Vector<T, Alloc> copy = other;
    swap(copy);
    return *this;
}

template <class T, class Alloc>
constexpr auto Vector<T, Alloc>::operator=(Vector&& other) -> Vector& {
    Vector<T, Alloc> empty;
    swap(empty);
    swap(other);
    return *this;
}

template <class T, class Alloc>
constexpr auto Vector<T, Alloc>::swap(Vector& other) noexcept -> void {
    std::swap(sz, other.sz);
    std::swap(cap, other.cap);
    std::swap(alloc, other.alloc);
    std::swap(ptr, other.ptr);
}

template <class T, class Alloc>
constexpr auto Vector<T, Alloc>::size() const noexcept -> size_t {
    return sz;
}

template <class T, class Alloc>
constexpr auto Vector<T, Alloc>::empty() const noexcept -> bool {
    return sz == 0;
}

template <class T, class Alloc>
constexpr auto Vector<T, Alloc>::capacity() const noexcept -> size_t {
    return cap;
}

template <class T, class Alloc>
constexpr auto Vector<T, Alloc>::front() const noexcept -> const T& {
    return ptr[0];
}

template <class T, class Alloc>
constexpr auto Vector<T, Alloc>::front() noexcept -> T& {
    return ptr[0];
}

template <class T, class Alloc>
constexpr auto Vector<T, Alloc>::back() const noexcept -> const T& {
    return ptr[sz - 1];
}

template <class T, class Alloc>
constexpr auto Vector<T, Alloc>::back() noexcept -> T& {
    return ptr[sz - 1];
}

template <class T, class Alloc>
constexpr auto Vector<T, Alloc>::data() const noexcept -> const T* {
    return ptr;
}

template <class T, class Alloc>
constexpr auto Vector<T, Alloc>::data() noexcept -> T* {
    return ptr;
}

template <class T, class Alloc>
constexpr auto Vector<T, Alloc>::operator[](size_t idx) const noexcept -> const T& {
    return ptr[idx];
}

template <class T, class Alloc>
constexpr auto Vector<T, Alloc>::operator[](size_t idx) noexcept -> T& {
    return ptr[idx];
}

template <class T, class Alloc>
constexpr auto Vector<T, Alloc>::at(size_t idx) const noexcept -> const T& {
    if (idx >= sz) {
        char* buf;
        sprintf(buf, OUT_OF_RANGE_MSG, idx, sz);
        throw std::out_of_range(buf);
    }
    return ptr[idx];
}

template <class T, class Alloc>
constexpr auto Vector<T, Alloc>::at(size_t idx) noexcept -> T& {
    if (idx >= sz) {
        char* buf;
        sprintf(buf, OUT_OF_RANGE_MSG, idx, sz);
        throw std::out_of_range(buf);
    }
    return ptr[idx];
}

template <class T, class Alloc>
constexpr auto Vector<T, Alloc>::reserve(size_t new_cap) -> void {
    if (new_cap <= cap) 
        return
    size_t buf_cap = cap;
    while (new_cap > buf_cap)
        buf_cap *= GROWTH_RATE;
    reserve_strict(alloc, ptr, cap, buf_cap);
}

template <class T, class Alloc>
constexpr auto Vector<T, Alloc>::shrink_to_fit() -> void {
    reserve_strict(alloc, ptr, cap, sz);
}

template <class T, class Alloc>
constexpr auto Vector<T, Alloc>::resize(size_t new_sz, const T& val) -> void {
    if (sz == new_sz)
        return
    size_t new_cap = cap;
    while (new_sz > new_cap)
        new_cap *= GROWTH_RATE;
    T* buf = AllocTraits::allocate(alloc, new_cap);
    for (size_t i = 0; i < new_sz; ++i) {
        try {
            const T& curr_val = i < sz ? ptr[i] : val;
            AllocTraits::construct(alloc, buf + i, curr_val);
        } catch(...) {
            destroy_before(alloc, buf, new_cap, i);
            throw;
        }
    }
    try {
        AllocTraits::deallocate(alloc, ptr, cap);
    } catch(...) {
        destroy_before(alloc, buf, new_cap, new_sz);
        throw;
    }
    ptr = buf;
    sz = new_sz;
    cap = new_cap;
}

template <class T, class Alloc>
constexpr auto Vector<T, Alloc>::begin() noexcept -> T* {
    return ptr;
}

template <class T, class Alloc>
constexpr auto Vector<T, Alloc>::cbegin() const noexcept -> const T* {
    return ptr;
}

template <class T, class Alloc>
constexpr auto Vector<T, Alloc>::rbegin() noexcept -> reverse_iterator {
    return end();
}

template <class T, class Alloc>
constexpr auto Vector<T, Alloc>::crbegin() const noexcept -> const reverse_iterator {
    return cend();
}

template <class T, class Alloc>
constexpr auto Vector<T, Alloc>::end() noexcept -> T* {
    return ptr + sz;
}

template <class T, class Alloc>
constexpr auto Vector<T, Alloc>::cend() const noexcept -> const T* {
    return ptr + sz;
}

template <class T, class Alloc>
constexpr auto Vector<T, Alloc>::rend() noexcept -> reverse_iterator {
    return begin();
}

template <class T, class Alloc>
constexpr auto Vector<T, Alloc>::crend() const noexcept -> const reverse_iterator {
    return cbegin();
}
