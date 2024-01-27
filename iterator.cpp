#include "iterator.hpp"

template <class T>
template <Direction dir, Mutability mut>
constexpr Vector<T>::directed_iterator<dir, mut>::directed_iterator() noexcept: data(nullptr) {}

template <class T>
template <Direction dir, Mutability mut>
constexpr Vector<T>::directed_iterator<dir, mut>::directed_iterator(pointer data) noexcept: data(data) {}

template <class T>
template <Direction dir, Mutability mut>
Vector<T>::directed_iterator<dir, mut>::operator directed_iterator<dir, Mutability::Constant>() const noexcept {
    return data;
}

template <class T>
template <Direction dir, Mutability mut>
auto Vector<T>::directed_iterator<dir, mut>::operator*() const noexcept -> reference {
    return *data;
}

template <class T>
template <Direction dir, Mutability mut>
auto Vector<T>::directed_iterator<dir, mut>::operator->() const noexcept -> pointer {
    return data;
}

auto Vector<T>::directed_iterator<dir, mut>::operator[](difference_type n) const noexcept -> reference {
    return *(*this + n);
}

template <class T>
template <Direction dir, Mutability mut>
auto Vector<T>::directed_iterator<dir, mut>::operator+=(difference_type n) & noexcept -> directed_iterator& {
    data += n * static_cast<int>(dir);
    return *this;
}

template <class T>
template <Direction dir, Mutability mut>
auto Vector<T>::directed_iterator<dir, mut>::operator-=(difference_type n) & noexcept -> directed_iterator& {
    return *this += -n;
}

template <class T>
template <Direction dir, MUtability mut>
auto Vector<T>::directed_iterator<dir, mut>::operator++() & noexcept -> directed_iterator& {
    return *this += 1;
}

auto Vector<T>::directed_iterator<dir, mut>::operator--() & noexcept -> directed_iterator& {
    return *this += -1;
}

auto Vector<T>::directed_iterator<dir, mut>::operator++(int) & noexcept -> directed_iterator {
    directed_iterator res = *this;
    ++(*this);
    return res;
}

auto Vector<T>::directed_iterator<dir, mut>::operator--(int) & noexcept -> directed_iterator {
    directed_iterator res = *this;
    --(*this);
    return res;
}

auto Vector<T>::directed_iterator<dir, mut>::operator+(const directed_iterator& it, difference_type n) -> directed_iterator {
    directed_iterator res = it;
    res += n;
    return res;
}

auto Vector<T>::directed_iterator<dir, mut>::operator+(difference_type n, const directed_iterator& it) -> directed_iterator {
    return it + n;
}

auto Vector<T>::directed_iterator<dir, mut>::operator-(const directed_iterator& it1, const directed_iterator& it2) -> difference_type {
    return it1.data - it2.data;
}

