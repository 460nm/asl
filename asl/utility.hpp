#pragma once

#include "asl/meta.hpp"
#include "asl/layout.hpp"
#include "asl/assert.hpp"

#define ASL_MOVE(expr_) (static_cast<::asl::un_ref_t<decltype(expr_)>&&>(expr_))

#define ASL_FWD(expr_) (static_cast<decltype(expr_)&&>(expr_))

namespace asl
{

struct in_place_t {};
static constexpr in_place_t in_place{};

template<moveable T>
constexpr void swap(T& a, T& b)
{
    T tmp{ASL_MOVE(a)};
    a = ASL_MOVE(b);
    b = ASL_MOVE(tmp);
}

template<typename T, typename U>
T exchange(T& obj, U&& new_value)
{
    T old_value = ASL_MOVE(obj);
    obj = ASL_FWD(new_value);
    return old_value;
}

template<trivially_copy_constructible U, trivially_copy_constructible T>
constexpr U bit_cast(T value) requires (size_of<T> == size_of<U>)
{
    return __builtin_bit_cast(U, value); 
}

template<typename T>
constexpr T min(T a, T b)
{
    return (a <= b) ? a : b;
}

template<typename T>
constexpr T max(T a, T b)
{
    return (a >= b) ? a : b;
}

constexpr uint64_t round_up_pow2(uint64_t v)
{
    ASL_ASSERT(v <= 0x8000'0000'0000'0000);

    v -= 1;
    
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v |= v >> 32;

    return v + 1;
}

constexpr bool is_pow2(isize_t v)
{
    return v > 0 && ((v - 1) & v) == 0;
}

#define ASL_DELETE_COPY(T)                         \
    T(const T&) = delete;                          \
    T& operator=(const T&) = delete;

#define ASL_DELETE_MOVE(T)                         \
    T(T&&) = delete;                               \
    T& operator=(T&&) = delete;

#define ASL_DELETE_COPY_MOVE(T)                    \
    ASL_DELETE_COPY(T)                             \
    ASL_DELETE_MOVE(T)

} // namespace asl
