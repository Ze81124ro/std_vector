#include "vector.hpp"
#include <iterator>
#include <type_traits>

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
            AllocTraits::deallocate(alloc, ptr, cap);
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
    using category = typename std::iterator_traits<InputIt>::iterator_category;
    if constexpr (std::is_base_of_v<category, std::random_access_iterator_tag>)
        sz = last - first;
    else
        sz = std::distance(first, last);
    while (sz > cap)
        cap *= GROWTH_RATE;
    ptr = AllocTraits::allocate(alloc, cap);
    size_t i  = 0;
    while (first != last) {
        try {
            AllocTraits::construct(alloc, ptr + i, *first)
            ++first;
            ++i;
        } catch(...) {
            AllocTraits::deallocate(alloc, ptr, cap);
            throw;
        }
    }
}

template <class T, class Alloc>
template <class InputIt>
constexpr Vector<T, Alloc>::Vector(InputIt first, InputIt last)
: Vector(first, last, Alloc()) {}

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
