#include "iterator.hpp"

template <class T>
template <Direction dir, Mutability mut>
constexpr Vector<T>::directed_iterator<dir, mut>::directed_iterator() noexcept: data(nullptr) {}

template <class T>
template <Direction dir, Mutability mut>
constexpr Vector<T>::directed_iterator<dir, mut>::directed_iterator(typename Vector<T>::directed_iterator<dir, mut>::pointer data) noexcept: data(data) {}

template <class T>
template <Direction dir, Mutability mut>
Vector<T>::directed_iterator<dir, mut>::operator directed_iterator<dir, Mutability::Constant>() const noexcept {
    return data;
}

template <class T>
template <Direction dir, Mutability mut>
typename Vector<T>::directed_iterator<dir, mut>::reference Vector<T>::directed_iterator<dir, mut>::operator*() const noexcept {
    return *data;
}

template <class T>
template <Direction dir, Mutability mut>
typename Vector<T>::directed_iterator<dir, mut>::pointer Vector<T>::directed_iterator<dir, mut>::operator->() const noexcept {
    return data;
}

// template <class T>
// template <>
