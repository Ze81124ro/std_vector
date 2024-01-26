#include <cstddef>
#include <cstdint>
#include <exception>
#include <initializer_list>
#include <iterator>
#include <new>
#include <stdexcept>
#include <type_traits>
#include <utility>

enum class Direction: int {
    Straight = 1,
    Reverse = -1
};

enum class Mutability: int {
    Constant,
    Mutable
};

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
    Vector(size_type count, const_pointer arr);
    Vector(std::initializer_list<T> init);
    Vector(const Vector& other);
    constexpr void swap(Vector& other) noexcept;
    Vector& operator=(const Vector& other) & noexcept;
    ~Vector();
    constexpr size_type size() const noexcept;
    constexpr size_type capacity() const noexcept;
    constexpr bool empty() const noexcept;
    const_pointer data() const noexcept;
    pointer data() noexcept;
    const_iterator begin() const noexcept;
    iterator begin() noexcept;
    const_iterator cbegin() const noexcept;
    const_reverse_iterator rbegin() const noexcept;
    reverse_iterator rbegin() noexcept;
    const_reverse_iterator crbegin() const noexcept;
    const_iterator end() const noexcept;
    iterator end() noexcept;
    const_iterator cend() const noexcept;
    const_reverse_iterator rend() const noexcept;
    reverse_iterator rend() noexcept;
    const_reverse_iterator crend() const noexcept;
};
