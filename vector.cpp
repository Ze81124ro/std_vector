#include "vector.hpp"

template <class T, class Allocator>
constexpr Vector<T, Allocator>::Vector(const Allocator& alloc) noexcept:
alloc(alloc) {}

template <class T, class Allocator>
constexpr Vector<T, Allocator>::Vector() noexcept(noexcept(Allocator())):
Vector(Allocator()) {}

template <class T, class Allocator>
constexpr Vector<T, Allocator>::Vector(size_type count, const Allocator& alloc): 
sz(count), alloc(alloc) {
    while (cap < sz) cap *= GROWTH_RATE;
    ptr = AllocTraits::allocate(alloc, cap);
}

template <class T, class Allocator>
Vector<T, Allocator>::Vector(size_type count, const_reference value, const Allocator& alloc):
Vector(count, alloc) {
    for (size_type i = 0; i < sz; ++i) {
        try {
            AllocTraits::construct(alloc, ptr + i, value);
        } catch(...) {
            for (size_type j = 0; j < i; ++j)
                AllocTraits::destroy(alloc, ptr + j);
        }
    }
}

template <class T, class Allocator>
Vector<T, Allocator>::Vector(size_type count, rvalue_reference value, const Allocator& alloc):
Vector(count, alloc) {
    for (size_type i = 0; i < sz; ++i) {
        try {
            AllocTraits::construct(alloc, ptr + i, std::move(value));
        } catch(...) {
            for (size_type j = 0; j < i; ++j)
                AllocTraits::destroy(alloc, ptr + j);
        }
    }
}

template <class T, class Allocator>
Vector<T, Allocator>::Vector(size_type count, const_pointer arr, const Allocator& alloc): 
Vector(count, alloc) {
    for (size_type i = 0; i < sz; ++i) {
        try {
            AllocTraits::construct(alloc, ptr + i, arr[i]);
        } catch(...) {
            for (size_type j = 0; j < i; ++j)
                AllocTraits::destroy(ptr + j);
        }
    }
}

template <class T, class Allocator>
Vector<T, Allocator>::Vector(std::initializer_list<T> init, const Allocator& alloc): 
Vector(init.size(), init.begin(), alloc) {}

template <class T, class Allocator>
Vector<T, Allocator>::Vector(const Vector& other, const Allocator& alloc): 
Vector(other.sz, other.ptr, alloc) {}

template <class T, class Allocator>
Vector<T, Allocator>::Vector(const Vector& other):
Vector(other, AllocTraits::select_on_container_copy_construction(other.alloc)) {}

template <class T, class Allocator>
Vector<T, Allocator>::Vector(Vector&& other) noexcept:
cap(other.cap), sz(other.sz), alloc(std::move(other.alloc)), ptr(other.ptr) {
    other.cap = 0;
    other.sz = 0;
    other.alloc = std::move(alloc);
    other.ptr = nullptr;
}

template <class T, class Allocator>
constexpr void Vector<T, Allocator>::swap(Vector& other) noexcept {
    std::swap(cap, other.cap);
    std::swap(sz, other.sz);
    std::swap(ptr, other.ptr);
}

template <class T, class Allocator>
auto Vector<T, Allocator>::operator=(const Vector& other) & noexcept -> Vector& {
    Vector copy = other;
    swap(copy);
    return *this;
}

template <class T, class Allocator>
Vector<T, Allocator>::~Vector() {
    if (ptr == nullptr) return;
    for (size_type i = 0; i < sz; ++i)
        (ptr + i)->~T();
    delete[] reinterpret_cast<uint8_t*>(ptr);
}

template <class T, class Allocator>
constexpr auto Vector<T, Allocator>::size() const noexcept -> size_type {
    return sz;
}

template <class T, class Allocator>
constexpr auto Vector<T, Allocator>::capacity() const noexcept -> size_type {
    return cap;
}

template <class T, class Allocator>
constexpr bool Vector<T, Allocator>::empty() const noexcept {
    return sz == 0;
}

template <class T, class Allocator>
constexpr auto Vector<T, Allocator>::data() const noexcept -> const_pointer {
    return ptr;
}

template <class T, class Allocator>
constexpr auto Vector<T, Allocator>::data() noexcept -> pointer {
    return ptr;
}

template <class T, class Allocator>
constexpr auto Vector<T, Allocator>::begin() const noexcept -> const_iterator {
    return ptr;
}

template <class T, class Allocator>
constexpr auto Vector<T, Allocator>::begin() noexcept -> iterator {
    return ptr;
}

template <class T, class Allocator>
constexpr auto Vector<T, Allocator>::cbegin() const noexcept -> const_iterator {
    return ptr;
}

template <class T, class Allocator>
constexpr auto Vector<T, Allocator>::rbegin() const noexcept -> const_reverse_iterator {
    return ptr + sz - 1;
}

template <class T, class Allocator>
constexpr auto Vector<T, Allocator>::rbegin() noexcept -> reverse_iterator {
    return ptr + sz - 1;
}

template <class T, class Allocator>
constexpr auto Vector<T, Allocator>::crbegin() const noexcept -> const_reverse_iterator {
    return ptr + sz - 1;
}

template <class T, class Allocator>
constexpr auto Vector<T, Allocator>::end() const noexcept -> const_iterator {
    return ptr + sz;
}

template <class T, class Allocator>
constexpr auto Vector<T, Allocator>::end() noexcept -> iterator {
    return ptr + sz;
}

template <class T, class Allocator>
constexpr auto Vector<T, Allocator>::cend() const noexcept -> const_iterator {
    return ptr + sz;
}

template <class T, class Allocator>
constexpr auto Vector<T, Allocator>::rend() const noexcept -> const_reverse_iterator {
    return ptr - 1;
}

template <class T, class Allocator>
constexpr auto Vector<T, Allocator>::rend() noexcept -> reverse_iterator {
    return ptr - 1;
}

template <class T, class Allocator>
constexpr auto Vector<T, Allocator>::crend() const noexcept -> const_reverse_iterator {
    return ptr - 1;
}

template <class T, class Allocator>
constexpr auto Vector<T, Allocator>::operator[](size_type idx) const noexcept -> const_reference {
    return *(begin() + idx);
}

template <class T, class Allocator>
constexpr auto Vector<T, Allocator>::operator[](size_type idx) noexcept -> reference {
    return *(begin() + idx);
}

template <class T, class Allocator>
constexpr auto Vector<T, Allocator>::at(size_type idx) const -> const_reference {
    if (idx >= sz) throw std::out_of_range("");
    else return (*this)[idx];
}

template <class T, class Allocator>
constexpr auto Vector<T, Allocator>::at(size_type idx) -> reference {
    if (idx >= sz) throw std::out_of_range("");
    else return (*this)[idx];
}

template <class T, class Allocator>
constexpr auto Vector<T, Allocator>::front() const noexcept -> const_reference {
    return *begin();
}

template <class T, class Allocator>
constexpr auto Vector<T, Allocator>::front() noexcept -> reference {
    return *begin();
}

template <class T, class Allocator>
constexpr auto Vector<T, Allocator>::back() const noexcept -> const_reference {
    return *--end();
}

template <class T, class Allocator>
constexpr auto Vector<T, Allocator>::back() noexcept -> reference {
    return *--end();
}

// template <class... Args>
// constexpr auto emplace(const_iterator pos, Args&&... args) -> iterator
// template <class... Args>
// constexpr reference emplace_back(Args&&... args);
// constexpr iterator insert(const_iterator pos, const_reference value);
// void push_back(const_reference value);
