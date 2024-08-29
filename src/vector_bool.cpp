#include "vector_bool.hpp"
#include <iterator>

namespace {
    void destroy_before(Alloc& alloc, uint8_t* buf, size_t cap, size_t i) {
        using AllocTraits = std::allocator_traits<Alloc>::rebind_traits<uint8_t>;
        while (i) {
            --i;
            try {
                AllocTraits::destry(alloc, buf + i);
            } catch(...) {
                break;
            }
        }
        try {
            AllocTraits::deallocate(alloc, buf, cap * BITS_IN_BYTE);
        } catch(...) {}
    }
}

template <class Alloc>
constexpr Vector<bool, Alloc>::Vector() noexcept(noexcept(Alloc()))
: Vector(Alloc()) {}

template <class Alloc>
constexpr Vector<bool, Alloc>::Vector(const Alloc& alloc)
: alloc(alloc), cap(0) {}

template <class Alloc>
constexpr Vector<bool, Alloc>::Vector(size_t sz, const T& val, const Alloc& alloc)
: alloc(alloc), sz(sz) {
    while (sz > cap)
        cap *= GROWTH_RATE;
    ptr = AllocTraits::allocate(alloc, cap * BITS_IN_BYTE);
    for (size_t i = 0; i < sz; ++i) {
        try {
            AllocTraits::construct(alloc, ptr + i, val);
        } catch(...) {
            destroy_before(alloc, ptr, cap, i);
            throw;
        }
    }
}

template <class Alloc>
constexpr Vector<bool, Alloc>::Vector(size_t sz, const T& val)
: Vector(sz, val, Alloc()) {}

template <class Alloc>
constexpr Vector<bool, Alloc>::Vector(size_t sz)
: Vector(sz, T(), Alloc()) {}

template <class Alloc>
constexpr Vector<bool, Alloc>::Vector(size_t sz, const Alloc& alloc)
: alloc(alloc), sz(sz) {
    while (sz > cap)
        cap *= GROWTH_RATE;
    ptr = AllocTraits::allocate(alloc, cap * BITS_IN_BYTE);
    for (size_t i = 0; i < sz; ++i) {
        try {
            AllocTraits::construct(alloc, ptr + i);
        } catch(...) {
            destroy_before(alloc, ptr, cap, i);
            throw;
        }
    }
}

template <class Alloc>
template <class InputIt>
constexpr Vector<bool, Alloc>::Vector(InputIt first, InputIt last, const Alloc& alloc)
: alloc(alloc), sz(std::distance(first, last)) {
    while (sz > cap)
        cap *= GROWTH_RATE;
    ptr = AllocTraits::allocate(alloc, cap * BITS_IN_BYTE);
    for (size_t i = 0; i < sz; ++i, ++first) {
        try {
            AllocTraits::construct(alloc, ptr + i, *first);
        } catch(...) {
            destroy_before(alloc, ptr, cap, i);
            throw;
        }
    }
}

template <class Alloc>
template <class InputIt>
constexpr Vector<bool, Alloc>::Vector(InputIt first, InputIt last)
: Vector(first, last, Alloc()) {}
