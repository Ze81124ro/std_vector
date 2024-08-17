#include "vector.hpp"
#include <cstddef>
#include <initializer_list>
#include <iterator>
#include <memory>
#include <type_traits>
#include <Utility>

template <class T, class Alloc>
constexpr Alloc Vector<T, Alloc>::copy_allocator() const {
    return AllocTraits::select_on_container_copy_construction(alloc);
}

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
            try {
                AllocTraits::deallocate(alloc, ptr, cap);
            } catch(...) {}
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
        bool condition = first != last;
        while (condition) {
            AllocTraits::construct(alloc, ptr + i, *first);
            ++i;
            ++first;
            condition = first != last;
        }
    } catch(...) {
        while (i) {
            --i;
            try {
                AllocTraits::destroy(alloc, ptr + i);
            } catch(...) {
                break;
            }
        }
        try {
            AllocTraits::deallocate(alloc, ptr, cap);
        } catch(...) {}
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
            AllocTraits::construct(alloc, ptr + i, *(other.ptr + i));
        } catch(...) {
            while (i) {
                --i;
                try {
                    AllocTraits::destroy(alloc, ptr + i);
                } catch(...) {
                    break;
                }
            }
            try {
                AllocTraits::deallocate(alloc, ptr, cap);
            } catch(...) {}
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
    for (size_t i = 0; i < sz; ++i) {
        try {
            AllocTraits::destroy(alloc, ptr + i);
        } catch(...) {
            break;
        }
    }
    try {
        AllocTraits::deallocate(alloc, ptr, cap);
    } catch(...) {}
}

template <class T, class Alloc>
constexpr auto Vector<T, Alloc>::swap(const Vector& other) noexcept -> void {
    std::swap(sz, other.sz);
    std::swap(cap, other.cap);
    std::swap(alloc, other.alloc);
    std::swap(ptr, other.ptr);
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
