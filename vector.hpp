#include <cstddef>
#include <cstdint>
#include <exception>
#include <initializer_list>
#include <iterator>
#include <new>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include "iterator.hpp"

template <class T>
class Vector {
private:
    static const int INITIAL_CAP = 1;
    static const int GROWTH_RATE = 2;
    size_t cap = INITIAL_CAP;
    size_t sz = 0;
    T* ptr = reinterpret_cast<T*>(new uint8_t[cap * sizeof(T)]);
    template <Direction dir, Mutability mut>
    class directed_iterator;
public:
    using value_type = T;
    using size_type = size_t;
    using difference_type = std::ptrdiff_t;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;
    using iterator = directed_iterator<Direction::Straight, Mutability::Mutable>;
    using const_iterator = directed_iterator<Direction::Straight, Mutability::Constant>;
    using reverse_iterator = directed_iterator<Direction::Reverse, Mutability::Mutable>;
    using const_reverse_iterator = directed_iterator<Direction::Reverse, Mutability::Constant>;
    constexpr Vector();
    explicit constexpr Vector(size_type count);
    Vector(size_type count, const_reference val);
};
