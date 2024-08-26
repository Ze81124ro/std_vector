#include "vector.hpp"
#include <cstddef>
#include <cstdio>
#include <algorithm>
#include <compare>
#include <initializer_list>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <utility>

namespace {
    static const char* OUT_OF_RANGE_MSG = "Index %zu is greater or equal to Vector size %zu in Vector::at.";

    template <class T, class Alloc>
    constexpr auto copy_allocator(const Vector<T, Alloc>& vec) -> Alloc {
        using AllocTraits = std::allocator_traits<Alloc>;
        return AllocTraits::select_on_container_copy_construction(vec.get_allocator());
    }

    template <class T, class Alloc>
    constexpr auto destroy_before(Alloc& alloc, T* buf, size_t cap, size_t i) -> void {
        using AllocTraits = std::allocator_traits<Alloc>;
        while (i) {
            --i;
            try {
                AllocTraits::destroy(alloc, buf + i);
            } catch(...) {
                break;
            }
        }
        try {
            AllocTraits::deallocate(alloc, buf, cap);
        } catch(...) {}
    }

    template <class T, class Alloc>
    constexpr auto reserve_strict(Alloc& alloc, T*& ptr, size_t sz, size_t& cap, size_t new_cap) -> void {
        using AllocTraits = std::allocator_traits<Alloc>;
        T* buf = AllocTraits::allocate(alloc, new_cap);
        for (size_t i = 0; i < sz; ++i) {
            try {
                AllocTraits::construct(alloc, buf + i, ptr[i]);
            } catch(...) {
                destroy_before(alloc, buf, new_cap, i);
                throw;
            }
        }
        try {
            AllocTraits::deallocate(alloc, ptr, cap);
        } catch(...) {
            destroy_before(alloc, buf, new_cap, sz);
            throw;
        }
        ptr = buf;
        cap = new_cap;
    }
}

template <class T, class Alloc>
template <class InputIt>
constexpr Vector<T, Alloc>::Vector(InputIt first, InputIt last, const Alloc& alloc, bool copy)
: alloc(alloc), sz(std::distance(first, last) * copy), cap(INITIAL_CAP * copy) {
    if (!copy) 
        return;
    while (sz > cap)
        cap *= GROWTH_RATE;
    ptr = AllocTraits::allocate(alloc, cap);
    for (size_t i = 0; i < sz; ++i, ++first) {
        try {
            AllocTraits::construct(alloc, ptr + i, *first);
        } catch(...) {
            destroy_before(alloc, ptr, cap, i);
            throw;
        }
    }
}

template <class InputIt, class Alloc>
Vector(InputIt first, InputIt last, const Alloc& alloc) 
-> Vector<typename std::iterator_traits<InputIt>::value_type, Alloc>;

template <class T, class Alloc> 
constexpr Vector<T, Alloc>::Vector(const Alloc& alloc) noexcept
: alloc(alloc), sz(0), cap(0) {}

template <class T, class Alloc>
constexpr Vector<T, Alloc>::Vector() noexcept(noexcept(Alloc()))
: Vector(Alloc()) {}

template <class T, class Alloc>
constexpr Vector<T, Alloc>::Vector(size_t sz, const T& val, const Alloc& alloc)
: alloc(alloc), sz(sz) {
    while (sz > cap)
        cap *= GROWTH_RATE;
    ptr = AllocTraits::allocate(alloc, cap);
    for (size_t i = 0; i < sz; ++i) {
        try {
            AllocTraits::construct(alloc, ptr + i, val);
        } catch(...) {
            destroy_before(alloc, ptr, cap, i);
            throw;
        }
    }
}

template <class T, class Alloc>
constexpr Vector<T, Alloc>::Vector(size_t sz, const T& val)
: Vector(sz, val, Alloc()) {}

template <class T, class Alloc>
constexpr Vector<T, Alloc>::Vector(size_t sz, const Alloc& alloc)
: Vector(sz, T(), alloc) {}

template <class T, class Alloc>
constexpr Vector<T, Alloc>::Vector(size_t sz)
: Vector(sz, T(), Alloc()) {}

template <class T, class Alloc>
template <class InputIt>
constexpr Vector<T, Alloc>::Vector(InputIt first, InputIt last, const Alloc& alloc)
: Vector(first, last, alloc, true) {}

template <class T, class Alloc>
template <class InputIt>
constexpr Vector<T, Alloc>::Vector(InputIt first, InputIt last)
: Vector(first, last, Alloc()) {}

template <class T, class Alloc>
constexpr Vector<T, Alloc>::Vector(std::initializer_list<T> init, const Alloc& alloc)
: Vector(init.begin(), init.end(), alloc) {}

template <class T, class Alloc>
constexpr Vector<T, Alloc>::Vector(std::initializer_list<T> init)
: Vector(init, Alloc()) {}

template <class T, class Alloc>
constexpr Vector<T, Alloc>::Vector(const Vector& other)
: Vector(other, other.copy_allocator()) {}

template <class T, class Alloc>
constexpr Vector<T, Alloc>::Vector(const Vector& other, const Alloc& alloc)
: Vector(other.cbegin(), other.cend(), alloc) {}

template <class T, class Alloc>
constexpr Vector<T, Alloc>::Vector(Vector&& other) noexcept
: Vector(other.alloc) {
    swap(other);
}

template <class T, class Alloc>
constexpr Vector<T, Alloc>::Vector(Vector&& other, const Alloc& alloc)
: Vector(
std::make_move_iterator(other.cbegin()),
std::make_move_iterator(other.cend()),
alloc,
&alloc == &other.alloc) {
    if (&alloc != &other.alloc)
        return;
    swap(other);
}

template <class T, class Alloc>
Vector<T, Alloc>::~Vector() {
    destroy_before(alloc, ptr, cap, sz);
}

template <class T, class Alloc>
constexpr auto Vector<T, Alloc>::operator=(const Vector& other) & -> Vector& {
    bool propagate = typename AllocTraits::propagate_on_container_copy_assignment();
    Vector<T, Alloc> copy(other, propagate ? other.alloc : alloc);
    swap(copy);
    return *this;
}

template <class T, class Alloc>
constexpr auto Vector<T, Alloc>::operator=(Vector&& other) & -> Vector& {
    bool propagate = typename AllocTraits::propagate_on_container_move_assignment();
    Vector<T, Alloc> copy(other, propagate ? other.alloc : alloc);
    Vector<T, Alloc> empty;
    swap(copy);
    other.swap(empty);
    return *this;
}

template <class T, class Alloc>
constexpr auto Vector<T, Alloc>::swap(Vector& other) & noexcept -> void {
    std::swap(sz, other.sz);
    std::swap(cap, other.cap);
    std::swap(alloc, other.alloc);
    std::swap(ptr, other.ptr);
}

template <class T, class Alloc>
constexpr auto Vector<T, Alloc>::size() const noexcept -> size_t {
    return sz;
}

template <class T, class Alloc>
constexpr auto Vector<T, Alloc>::empty() const noexcept -> bool {
    return sz == 0;
}

template <class T, class Alloc>
constexpr auto Vector<T, Alloc>::capacity() const noexcept -> size_t {
    return cap;
}

template <class T, class Alloc>
constexpr auto Vector<T, Alloc>::get_allocator() const noexcept -> Alloc {
    return alloc;
}

template <class T, class Alloc>
constexpr auto Vector<T, Alloc>::front() const noexcept -> const T& {
    return ptr[0];
}

template <class T, class Alloc>
constexpr auto Vector<T, Alloc>::front() noexcept -> T& {
    return ptr[0];
}

template <class T, class Alloc>
constexpr auto Vector<T, Alloc>::back() const noexcept -> const T& {
    return ptr[sz - 1];
}

template <class T, class Alloc>
constexpr auto Vector<T, Alloc>::back() noexcept -> T& {
    return ptr[sz - 1];
}

template <class T, class Alloc>
constexpr auto Vector<T, Alloc>::data() const noexcept -> const T* {
    return ptr;
}

template <class T, class Alloc>
constexpr auto Vector<T, Alloc>::data() noexcept -> T* {
    return ptr;
}

template <class T, class Alloc>
constexpr auto Vector<T, Alloc>::operator[](size_t idx) const noexcept -> const T& {
    return ptr[idx];
}

template <class T, class Alloc>
constexpr auto Vector<T, Alloc>::operator[](size_t idx) noexcept -> T& {
    return ptr[idx];
}

template <class T, class Alloc>
constexpr auto Vector<T, Alloc>::at(size_t idx) const -> const T& {
    if (idx >= sz) {
        char* buf;
        sprintf(buf, OUT_OF_RANGE_MSG, idx, sz);
        throw std::out_of_range(buf);
    }
    return ptr[idx];
}

template <class T, class Alloc>
constexpr auto Vector<T, Alloc>::at(size_t idx) -> T& {
    if (idx >= sz) {
        char* buf;
        sprintf(buf, OUT_OF_RANGE_MSG, idx, sz);
        throw std::out_of_range(buf);
    }
    return ptr[idx];
}

template <class T, class Alloc>
constexpr auto Vector<T, Alloc>::assign(size_t new_sz, const T& val) -> void {
    *this = Vector(new_sz, val);
}

template <class T, class Alloc>
template <class InputIt>
constexpr auto Vector<T, Alloc>::assign(InputIt first, InputIt last) -> void {
    *this = Vector(first, last);
}

template <class T, class Alloc>
constexpr auto Vector<T, Alloc>::assign(std::initializer_list<T> init) -> void {
    *this = Vector(init);
}

template <class T, class Alloc>
constexpr auto Vector<T, Alloc>::reserve(size_t new_cap) -> void {
    if (new_cap <= cap) 
        return;
    size_t buf_cap = cap;
    while (new_cap > buf_cap)
        buf_cap *= GROWTH_RATE;
    reserve_strict(alloc, ptr, sz, cap, buf_cap);
}

template <class T, class Alloc>
constexpr auto Vector<T, Alloc>::shrink_to_fit() -> void {
    reserve_strict(alloc, ptr, sz, cap, sz);
}

template <class T, class Alloc>
constexpr auto Vector<T, Alloc>::resize(size_t new_sz, const T& val) -> void {
    if (sz == new_sz)
        return;
    size_t new_cap = cap;
    while (new_sz > new_cap)
        new_cap *= GROWTH_RATE;
    T* buf = AllocTraits::allocate(alloc, new_cap);
    for (size_t i = 0; i < new_sz; ++i) {
        try {
            const T& curr_val = i < sz ? ptr[i] : val;
            AllocTraits::construct(alloc, buf + i, curr_val);
        } catch(...) {
            destroy_before(alloc, buf, new_cap, i);
            throw;
        }
    }
    try {
        AllocTraits::deallocate(alloc, ptr, cap);
    } catch(...) {
        destroy_before(alloc, buf, new_cap, new_sz);
        throw;
    }
    ptr = buf;
    sz = new_sz;
    cap = new_cap;
}

template <class T, class Alloc>
constexpr auto Vector<T, Alloc>::resize(size_t new_sz) -> void {
    resize(new_sz, T());
}

template <class T, class Alloc>
template <class... Args>
constexpr auto Vector<T, Alloc>::emplace(const T* pos, Args&&... args) -> T* {
    if (pos == cend())
        emplace_back(std::forward<Args>(args)...);
    size_t new_cap = cap;
    if (sz + 1 > new_cap)
        new_cap *= GROWTH_RATE;
    T* buf = AllocTraits::allocate(alloc, new_cap);
    const T* iter = cbegin();
    for (size_t i = 0; i <= sz; ++i, ++iter) {
        try {
            if (iter < pos) {
                AllocTraits::construct(alloc, buf + i, ptr[i]);
                continue;
            }
            if (iter > pos) {
                AllocTraits::construct(alloc, buf + i, ptr[i]);
                continue;
            }
            AllocTraits::construct(alloc, buf + i, std::forward<Args>(args)...);
        } catch(...) {
            destroy_before(alloc, buf, new_cap, i);
            throw;
        }
    }
    try {
        AllocTraits::deallocate(alloc, ptr, cap);
    } catch(...) {
        destroy_before(alloc, buf, new_cap, sz + 1);
        throw;
    }
    T* res = buf + (pos - ptr);
    ptr = buf;
    ++sz;
    cap = new_cap;
    return res;
}

template <class T, class Alloc>
constexpr auto Vector<T, Alloc>::insert(const T* pos, const T& val) -> T* {
    return emplace(pos, val);
}

template <class T, class Alloc>
constexpr auto Vector<T, Alloc>::insert(const T* pos, T&& val) -> T* {
    return emplace(pos, std::move_if_noexcept(val));
}

template <class T, class Alloc>
constexpr auto Vector<T, Alloc>::insert(const T* pos, size_t new_sz, const T& val) -> T* {
    if (new_sz == 0)
        return pos;
    T* res = pos;
    for (size_t i = 0; i < new_sz; ++i) {
        try {
            res = insert(res + 1, val);
        } catch(...) {
            try {
                erase(res - i, res);
            } catch(...) {}
            throw;
        }
    }
    return res - new_sz + 1;
}

template <class T, class Alloc>
template <class InputIt>
constexpr auto Vector<T, Alloc>::insert(const T* pos, InputIt first, InputIt last) -> T* {
    if (first == last)
        return pos;
    size_t new_sz = std::distance(first, last);
    T* res = pos;
    for (size_t i = 0; i < new_sz; ++i, ++first) {
        try {
            res = insert(res + 1, *first);
        } catch(...) {
            try {
                erase(res - i, res);
            } catch(...) {}
            throw;
        }
    }
    return res - new_sz + 1;
}

template <class T, class Alloc>
constexpr auto Vector<T, Alloc>::insert(const T* pos, std::initializer_list<T> init) -> T* {
    return insert(pos, init.begin(), init.end());
}

template <class T, class Alloc>
template <class... Args>
constexpr auto Vector<T, Alloc>::emplace_back(Args&&... args) -> T& {
    reserve(sz + 1);
    try {
        AllocTraits::construct(alloc, end(), std::forward<Args>(args)...);
    } catch(...) {
        try {
            AllocTraits::destroy(alloc, end());
        } catch(...) {}
        throw;
    }
    ++sz;
    return back();
}

template <class T, class Alloc>
constexpr auto Vector<T, Alloc>::push_back(const T& val) -> void {
    emplace_back(val);
}

template <class T, class Alloc>
constexpr auto Vector<T, Alloc>::push_back(T&& val) -> void {
    emplace_back(std::move_if_noexcept(val));
}

template <class T, class Alloc>
constexpr auto Vector<T, Alloc>::erase(const T* pos) -> T* {
    return erase(pos, pos + 1);
}

template <class T, class Alloc>
constexpr auto Vector<T, Alloc>::erase(const T* first, const T* last) -> T* {
    size_t new_sz = std::distance(first, last);
    T* buf = AllocTraits::allocate(alloc, cap);
    T* iter = cbegin();
    for (size_t i = 0; i < sz; ++i, ++iter) {
        try {
            if (iter < first) {
                AllocTraits::construct(alloc, buf + i, ptr[i]);
            }
            if (iter >= last) {
                AllocTraits::construct(alloc, buf - new_sz + i, ptr[i]);
            }
        } catch(...) {
            destroy_before(alloc, buf, cap, iter < first ? i : i - new_sz);
            throw;
        }
    }
    try {
        AllocTraits::deallocate(alloc, ptr, cap);
    } catch(...) {
        destroy_before(alloc, buf, cap, sz - 1);
        throw;
    }
    T* res = buf + (last - ptr);
    ptr = buf;
    sz -= new_sz;
    return res;
}

template <class T, class Alloc>
constexpr auto Vector<T, Alloc>::pop_back() -> void {
    erase(cend() - 1);
}

template <class T, class Alloc>
constexpr auto Vector<T, Alloc>::clear() -> void {
    erase(cbegin(), cend());
}

template <class T, class Alloc>
constexpr auto Vector<T, Alloc>::begin() noexcept -> T* {
    return ptr;
}

template <class T, class Alloc>
constexpr auto Vector<T, Alloc>::cbegin() const noexcept -> const T* {
    return ptr;
}

template <class T, class Alloc>
constexpr auto Vector<T, Alloc>::rbegin() noexcept -> reverse_iterator {
    return end();
}

template <class T, class Alloc>
constexpr auto Vector<T, Alloc>::crbegin() const noexcept -> const reverse_iterator {
    return cend();
}

template <class T, class Alloc>
constexpr auto Vector<T, Alloc>::end() noexcept -> T* {
    return ptr + sz;
}

template <class T, class Alloc>
constexpr auto Vector<T, Alloc>::cend() const noexcept -> const T* {
    return ptr + sz;
}

template <class T, class Alloc>
constexpr auto Vector<T, Alloc>::rend() noexcept -> reverse_iterator {
    return begin();
}

template <class T, class Alloc>
constexpr auto Vector<T, Alloc>::crend() const noexcept -> const reverse_iterator {
    return cbegin();
}

template <class T, class Alloc>
constexpr auto operator==(const Vector<T, Alloc>& lhs, const Vector<T, Alloc>& rhs) -> bool {
    if (lhs.size() != rhs.size())
        return false;
    for (size_t i = 0; i < lhs.size(); ++i)
        if (lhs[i] != rhs[i])
            return false;
    return true;
}

template <class T, class Alloc>
constexpr auto operator<=>(const Vector<T, Alloc>& lhs, const Vector<T, Alloc>& rhs)
-> std::strong_ordering {
    for (size_t i = 0; i < std::min(lhs.size(), rhs.size()); ++i) {
        std::strong_ordering elem_order = lhs[i] <=> rhs[i];
        if (elem_order != 0)
            return elem_order;
    }
    std::strong_ordering sz_order = lhs.size() <=> rhs.size();
    return sz_order;
}

template <class T, class Alloc, class Pred>
constexpr auto erase_if(Vector<T, Alloc>& vec, Pred pred) -> size_t {
    size_t res = 0;
    for (size_t i = 0; i < vec.size(); ++i) {
        if (pred(vec[i])) {
            vec.erase(vec.begin() + i);
            ++i;
            ++res;
        }
    }
    return res;
}

template <class T, class Alloc, class U>
constexpr auto erase(Vector<T, Alloc>& vec, const U& val) -> size_t {
    return erase_if(vec, [val] (const T& elem) -> bool {
        return elem = val;
    });
}

namespace std {
    template <class T, class Alloc>
    constexpr auto swap(Vector<T, Alloc>& lhs, Vector<T, Alloc>& rhs) -> void {
        lhs.swap(rhs);
    }
}
