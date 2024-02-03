#include <cstddef>
#include <cstdint>
#include <exception>
#include <initializer_list>
#include <iterator>
#include <memory>
#include <new>
#include <stdexcept>
#include <type_traits>
#include <utility>

template <class T, class Allocator>
class Vector {
private:
    static const int INITIAL_CAP = 1;
    static const int GROWTH_RATE = 2;
    size_t cap = INITIAL_CAP;
    size_t sz = 0;
    T* ptr = reinterpret_cast<T*>(new uint8_t[cap * sizeof(T)]);
    const Allocator& alloc;
    using AllocTraits = std::allocator_traits<Allocator>;
public:
    using allocator_type = Allocator;
    using value_type = T;
    using size_type = size_t;
    using difference_type = std::ptrdiff_t;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;
    using iterator = T*;
    using const_iterator = const T*;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
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
    constexpr const_pointer data() const noexcept;
    constexpr pointer data() noexcept;
    constexpr const_iterator begin() const noexcept;
    constexpr iterator begin() noexcept;
    constexpr const_iterator cbegin() const noexcept;
    constexpr const_reverse_iterator rbegin() const noexcept;
    constexpr reverse_iterator rbegin() noexcept;
    constexpr const_reverse_iterator crbegin() const noexcept;
    constexpr const_iterator end() const noexcept;
    constexpr iterator end() noexcept;
    constexpr const_iterator cend() const noexcept;
    constexpr const_reverse_iterator rend() const noexcept;
    constexpr reverse_iterator rend() noexcept;
    constexpr const_reverse_iterator crend() const noexcept;
    constexpr const_reference operator[](size_type idx) const noexcept;
    constexpr reference operator[](size_type idx) noexcept;
    constexpr const_reference at(size_type idx) const;
    constexpr reference at(size_type idx);
    constexpr const_reference front() const noexcept;
    constexpr reference front() noexcept;
    constexpr const_reference back() const noexcept;
    constexpr reference back() noexcept;
    constexpr void reserve(size_type count);
    template <class... Args>
    constexpr iterator emplace(const_iterator pos, Args&&... args);
    template <class... Args>
    constexpr reference emplace_back(Args&&... args);
    constexpr iterator insert(const_iterator pos, const_reference value);
    void push_back(const_reference value);
};
