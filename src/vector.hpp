#ifndef VECTOR_H

#define VECTOR_H

#include <cstddef>
#include <compare>
#include <initializer_list>
#include <iterator>
#include <memory>

template <class T, class Alloc = std::allocator<T>>
class Vector {
public:
    using value_type = T;
    using allocator_type = Alloc;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using reference = T&;
    using const_reference = const reference;
    using pointer = T*;
    using const_pointer = const pointer;
    using iterator = pointer;
    using const_iterator = const iterator;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = const reverse_iterator;
    constexpr Vector() noexcept(noexcept(Alloc()));
    explicit constexpr Vector(const Alloc& alloc) noexcept;
    constexpr Vector(size_t sz, const T& val, const Alloc& alloc);
    constexpr Vector(size_t sz, const T& val);
    constexpr Vector(size_t sz, const Alloc& alloc);
    explicit constexpr Vector(size_t sz);
    template <class InputIt>
    constexpr Vector(InputIt first, InputIt last, const Alloc& alloc);
    template <class InputIt>
    constexpr Vector(InputIt first, InputIt last);
    constexpr Vector(std::initializer_list<T> init, const Alloc& alloc);
    constexpr Vector(std::initializer_list<T> init);
    constexpr Vector(const Vector& other);
    constexpr Vector(const Vector& other, const Alloc& alloc);
    constexpr Vector(Vector&& other) noexcept;
    constexpr Vector(Vector&& other, const Alloc& alloc);
    ~Vector();
    constexpr Vector& operator=(const Vector& rhs) &;
    constexpr Vector& operator=(Vector&& other) &;
    constexpr void swap(Vector& other) & noexcept;
    constexpr size_t size() const noexcept;
    constexpr bool empty() const noexcept;
    constexpr size_t capacity() const noexcept;
    constexpr Alloc get_allocator() const noexcept;
    constexpr const T& front() const noexcept;
    constexpr T& front() noexcept;
    constexpr const T& back() const noexcept;
    constexpr T& back() noexcept;
    constexpr const T* data() const noexcept;
    constexpr T* data() noexcept;
    constexpr const T& operator[](size_t idx) const noexcept;
    constexpr T& operator[](size_t idx) noexcept;
    constexpr const T& at(size_t idx) const;
    constexpr T& at(size_t idx);
    constexpr void assign(size_t new_sz, const T& val);
    template <class InputIt>
    constexpr void assign(InputIt first, InputIt last);
    constexpr void assign(std::initializer_list<T> init);
    constexpr void reserve(size_t new_cap);
    constexpr void shrink_to_fit();
    constexpr void resize(size_t new_sz, const T& val);
    constexpr void resize(size_t new_sz);
    template <class... Args>
    constexpr T* emplace(const T* pos, Args&&... args);
    constexpr T* insert(const T* pos, const T& val);
    constexpr T* insert(const T* pos, T&& val);
    constexpr T* insert(const T* pos, size_t new_sz, const T& val);
    template <class InputIt>
    constexpr T* insert(const T* pos, InputIt first, InputIt last);
    constexpr T* insert(const T* pos, std::initializer_list<T> init);
    template <class... Args>
    constexpr T& emplace_back(Args&&... args);
    constexpr void push_back(const T& val);
    constexpr void push_back(T&& val);
    constexpr T* erase(const T* pos);
    constexpr T* erase(const T* first, const T* last);
    constexpr void pop_back();
    constexpr void clear();
    constexpr T* begin() noexcept;
    constexpr T* end() noexcept;
    constexpr const T* cbegin() const noexcept;
    constexpr const T* cend() const noexcept;
    constexpr reverse_iterator rbegin() noexcept;
    constexpr reverse_iterator rend() noexcept;
    constexpr const reverse_iterator crbegin() const noexcept;
    constexpr const reverse_iterator crend() const noexcept;
private:
    using AllocTraits = std::allocator_traits<Alloc>;
    template <class InputIt>
    constexpr Vector(InputIt first, InputIt last, const Alloc& alloc, bool copy);
    static const size_t INITIAL_CAP = 4;
    static const size_t GROWTH_RATE = 2;
    Alloc alloc;
    size_t sz;
    size_t cap = INITIAL_CAP;
    T* ptr = nullptr;
};

template <class T, class Alloc>
constexpr bool operator==(const Vector<T, Alloc>& lhs, const Vector<T, Alloc>& rhs);

template <class T, class Alloc>
constexpr std::strong_ordering operator<=>(const Vector<T, Alloc>& lhs, const Vector<T, Alloc>& rhs);

template <class T, class Alloc, class U = T>
constexpr size_t erase(Vector<T, Alloc>& vec, const U& val);

template <class T, class Alloc, class Pred>
constexpr size_t erase_if(Vector<T, Alloc>& vec, Pred pred);

namespace std {
    template <class T, class Alloc>
    constexpr void swap(Vector<T, Alloc>& lhs, Vector<T, Alloc>& rhs);
}

#include "vector.cpp"

#endif
