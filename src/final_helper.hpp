#ifndef FINAL_HELPER_H

#define FINAL_HELPER_H

#include <type_traits>

namespace detail {
template <class Alloc, bool>
struct FinalHelperImpl: Alloc {};

template <class Alloc>
struct FinalHelperImpl<Alloc, true> {
public:
    constexpr FinalHelperImpl(const Alloc& alloc) noexcept;
    constexpr operator const Alloc&() const noexcept;
    constexpr operator Alloc&() noexcept;
private:
    Alloc alloc;
};

template <class Alloc>
using FinalHelper = FinalHelperImpl<Alloc, std::is_final_v<Alloc>>;
}

#define HEADER_INCLUDES
#ifndef IMPL_INCLUDES
#include "final_helper.cpp"
#endif
#undef HEADER_INCLUDES

#endif
