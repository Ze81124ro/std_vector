#include "vector.hpp"

template <class T, Mutability mut>
struct TypesByMutability {
    using reference = const T&;
    using pointer = const T*;
};

template <class T>
struct TypesByMutability<T, Mutability::Mutable> {
    using reference = T&;
    using pointer = T*;
};

template <class T, Mutability mut>   
using reference_t = typename TypesByMutability<T, mut>::reference;

template <class T, Mutability mut>
using pointer_t = typename TypesByMutability<T, mut>::pointer;

template <class T>
template <Direction dir, Mutability mut>
class Vector<T>::directed_iterator {
private:
    pointer_t<T, mut> data;
public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = pointer_t<T, mut>;
    using reference = reference_t<T, mut>;
    constexpr directed_iterator() noexcept;
    constexpr directed_iterator(pointer data) noexcept;
    operator directed_iterator<dir, Mutability::Constant>() const noexcept;
    reference operator*() const noexcept;
    pointer operator->() const noexcept;
    reference operator[](difference_type n) const noexcept;
    directed_iterator& operator+=(difference_type n) & noexcept;
    directed_iterator& operator-=(difference_type n) & noexcept;
    directed_iterator& operator++() & noexcept;
    directed_iterator& operator--() & noexcept;
    directed_iterator operator++(int) & noexcept;
    directed_iterator operator--(int) & noexcept;
};
