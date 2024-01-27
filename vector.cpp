#include "vector.hpp"

template <class T>
constexpr Vector<T>::Vector() {}

template <class T>
constexpr Vector<T>::Vector(size_type count): sz(count), ptr(nullptr) {
    while (cap < sz) cap *= GROWTH_RATE;
    ptr = reinterpret_cast<T*>(new uint8_t[cap * sizeof(T)]);
}

template <class T>
Vector<T>::Vector(size_type count, const_reference value): Vector(count) {
    for (size_type i = 0; i < sz; ++i) {
        try {
            new(ptr + i) T(value);
        } catch(...) {
            for (size_type j = 0; j < i; ++j)
                (ptr + j)->~T();
        }
    }
}

template <class T>
Vector<T>::Vector(size_type count, const_pointer arr): Vector(count) {
    for (size_type i = 0; i < sz; ++i) {
        try {
            new(ptr + i) T(arr[i]);
        } catch(...) {
            for (size_type j = 0; j < i; ++j)
                (ptr + j)->~T();
        }
    }
}

template <class T>
Vector<T>::Vector(std::initializer_list<T> init): Vector(init.size(), init.begin()) {}

template <class T>
Vector<T>::Vector(const Vector& other): Vector(other.sz, other.ptr) {}

template <class T>
constexpr void Vector<T>::swap(Vector& other) noexcept {
    std::swap(cap, other.cap);
    std::swap(sz, other.sz);
    std::swap(ptr, other.ptr);
}

template <class T>
auto Vector<T>::operator=(const Vector& other) & noexcept -> Vector& {
    Vector copy = other;
    swap(copy);
    return *this;
}

template <class T>
Vector<T>::~Vector() {
    for (size_type i = 0; i < sz; ++i)
        (ptr + i)->~T();
    delete[] reinterpret_cast<uint8_t*>(ptr);
}

template <class T>
constexpr auto Vector<T>::size() const noexcept -> size_type {
    return sz;
}

template <class T>
constexpr auto Vector<T>::capacity() const noexcept -> size_type {
    return cap;
}

template <class T>
constexpr bool Vector<T>::empty() const noexcept {
    return sz == 0;
}

template <class T>
auto Vector<T>::data() const noexcept -> const_pointer {
    return ptr;
}

template <class T>
auto Vector<T>::data() noexcept -> pointer {
    return ptr;
}

template <class T>
auto Vector<T>::begin() const noexcept -> const_iterator {
    return data();
}

template <class T>
auto Vector<T>::begin() noexcept -> iterator {
    return data();
}

template <class T>
auto Vector<T>::cbegin() const noexcept -> const_iterator {
    return data();
}

template <class T>
auto Vector<T>::rbegin() const noexcept -> const_reverse_iterator {
    return data() + sz - 1;
}

template <class T>
auto Vector<T>::rbegin() noexcept -> reverse_iterator {
    return data() + sz - 1;
}

template <class T>
auto Vector<T>::crbegin() const noexcept -> const_reverse_iterator {
    return data() + sz - 1;
}

template <class T>
auto Vector<T>::end() const noexcept -> const_iterator {
    return data() + sz;
}

template <class T>
auto Vector<T>::end() noexcept -> iterator {
    return data() + sz;
}

template <class T>
auto Vector<T>::cend() const noexcept -> const_iterator {
    return data() + sz;
}

template <class T>
auto Vector<T>::rend() const noexcept -> const_reverse_iterator {
    return data() - 1;
}

template <class T>
auto Vector<T>::rend() noexcept -> reverse_iterator {
    return data() - 1;
}

template <class T>
auto Vector<T>::crend() const noexcept -> const_reverse_iterator {
    return data() - 1;
}
