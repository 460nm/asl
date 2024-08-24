#pragma once

#include "asl/integers.hpp"
#include "asl/meta.hpp"
#include "asl/utility.hpp"

namespace asl
{

// @Todo Shitty span, improve this
template<is_object T, isize_t kLen>
struct span
{
    static constexpr bool kDynamic = kLen < 0;

    using size_type = select_t<kDynamic, isize_t, empty>;

    constexpr span(T* begin, isize_t len) requires kDynamic
        : m_begin{begin}
        , m_len{len}
    {}

    constexpr span(T* begin) requires (!kDynamic)
        : m_begin{begin}
        , m_len{}
    {}
    
    T* m_begin;
    ASL_NO_UNIQUE_ADDRESS size_type m_len;
};

namespace ptr_internal
{

template<is_void T>
struct void_metadata
{
    using metadata = empty;
    using pointee = T;

    constexpr auto deref(pointee* ptr) { return ptr; }
};

template<is_array T>
struct array_metadata {};

template<is_object T>
struct array_metadata<T[]>
{
    using metadata = isize_t;
    using pointee = T;

    constexpr auto deref(pointee* ptr)
    {
        return span<pointee, -1>(ptr, m_len);
    }

    isize_t m_len;
};

template<is_object T, isize_t N>
struct array_metadata<T[N]>
{
    using metadata = empty;
    using pointee = T[N];

    constexpr auto deref(pointee* ptr)
    {
        return span<T, N>(static_cast<T*>(*ptr));
    }
};

template<is_object T>
struct object_metadata
{
    using metadata = empty;
    using pointee = T;

    constexpr auto deref(pointee* ptr) { return ptr; }
};

template<is_func T>
struct func_metadata
{
    using metadata = empty;
    using pointee = tame_t<T>* const;

    constexpr auto deref(pointee* ptr) { return *ptr; }
};

template<is_ref T>
struct ref_metadata
{
    using metadata = empty;
    using pointee = un_ref_t<T>* const;

    constexpr auto deref(pointee* ptr) { return *ptr; }
};

template<is_void   T> void_metadata<T>   select_ptr_metadata(types<T>);
template<is_array  T> array_metadata<T>  select_ptr_metadata(types<T>);
template<is_object T> object_metadata<T> select_ptr_metadata(types<T>) requires (!is_array<T>);
template<is_func   T> func_metadata<T>   select_ptr_metadata(types<T>);
template<is_ref    T> ref_metadata<T>    select_ptr_metadata(types<T>);

template<typename T>
using metadata = decltype(select_ptr_metadata(types<T>{}));

} // namespace ptr_internal

template<typename T>
concept ptr_metadata = requires (T metadata, typename T::pointee* ptr)
{
    is_object<typename T::metadata>;
    is_object<typename T::pointee>;

    { metadata.deref(ptr) };
};

template<typename T>
class ptr
{
    using meta = ptr_internal::metadata<T>;
    static_assert(ptr_metadata<meta>);

    meta::pointee* m_ptr;
    ASL_NO_UNIQUE_ADDRESS meta m_meta;
    
public:
    
};

} // namespace asl
