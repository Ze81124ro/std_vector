#ifndef VECTOR_BOOL_H

#define VECTOR_BOOL_H

#include <cstddef>
#include <iterator>

template <class T, class Alloc>
class Vector;

template <class Alloc>
class Vector<bool, Alloc> {
private:
    class BitReference;
public:
    using value_type = bool;
    using allocator_type = Alloc;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using reference = BitReference;
    using const_reference = bool;
    using pointer = BitReference;
    using const_pointer = const pointer;
    using iterator = BitReference;
    using const_iterator = const iterator;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = const reverse_iterator;
private:
};

#endif
