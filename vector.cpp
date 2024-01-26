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
Vector<T>& Vector<T>::operator=(const Vector& other) & noexcept {
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
constexpr Vector<T>::size_type Vector<T>::size() const noexcept {
    return sz;
}

template <class T>
constexpr Vector<T>::size_type Vector<T>::capacity() const noexcept {
    return cap;
}

template <class T>
constexpr bool Vector<T>::empty() const noexcept {
    return sz == 0;
}

template <class T>
Vector<T>::const_pointer Vector<T>::data() const noexcept {
    return ptr;
}

template <class T>
Vector<T>::pointer Vector<T>::data() noexcept {
    return ptr;
}

template <class T>
Vector<T>::const_iterator Vector<T>::begin() const noexcept {
    return data();
}

template <class T>
Vector<T>::iterator Vector<T>::begin() noexcept {
    return data();
}

template <class T>
Vector<T>::const_iterator Vector<T>::cbegin() const noexcept {
    return data();
}

template <class T>
Vector<T>::const_reverse_iterator Vector<T>::rbegin() const noexcept {
    return data();
}

template <class T>
Vector<T>::reverse_iterator Vector<T>::rbegin() noexcept {
    return data();
}

template <class T>
Vector<T>::const_reverse_iterator Vector<T>::crbegin() const noexcept {
    return data();
}

template <class T>
Vector<T>::const_iterator Vector<T>::end() const noexcept {
    return data() + sz;
}

template <class T>
Vector<T>::iterator Vector<T>::end() noexcept {
    return data() + sz;
}

template <class T>
Vector<T>::const_iterator Vector<T>::cend() const noexcept {
    return data() + sz;
}

template <class T>
Vector<T>::const_reverse_iterator Vector<T>::rend() const noexcept {
    return data() + sz;
}

template <class T>
Vector<T>::reverse_iterator Vector<T>::rend() noexcept {
    return data() + sz;
}

template <class T>
Vector<T>::const_reverse_iterator Vector<T>::crend() const noexcept {
    return data() + sz;
}
