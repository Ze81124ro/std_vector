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
