#ifndef HEADER_INCLUDES
#define IMPL_INCLUDES
#include "final_helper.hpp"
#undef IMPL_INCLUDES
#endif

template <class Alloc>
constexpr detail::FinalHelperImpl<Alloc, true>::FinalHelperImpl(const Alloc& alloc) noexcept
: alloc(alloc) {}

template <class Alloc>
constexpr detail::FinalHelperImpl<Alloc, true>::operator const Alloc&() const noexcept {
    return alloc;
}

template <class Alloc>
constexpr detail::FinalHelperImpl<Alloc, true>::operator Alloc&() noexcept {
    return alloc;
}
