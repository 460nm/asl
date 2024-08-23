#pragma once

#include "asl/integers.hpp"
#include "asl/meta.hpp"

namespace asl
{
namespace ptr_internal
{

template<is_void T>
struct void_metadata
{
    using metadata = empty;
    using pointee = T;
};

template<is_array T>
struct array_metadata {};

template<typename T>
struct array_metadata<T[]>
{
    using metadata = isize_t;
    using pointee = T;
};

template<typename T, isize_t N>
struct array_metadata<T[N]>
{
    using metadata = empty;
    using pointee = T[N];
};

template<is_object T>
struct object_metadata
{
    using metadata = empty;
    using pointee = T;
};

template<is_func T>
struct func_metadata
{
    using metadata = empty;
    using pointee = tame_t<T>*;
};

template<is_ref T>
struct ref_metadata
{
    using metadata = empty;
    using pointee = un_ref_t<T>*;
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
concept ptr_metadata = requires
{
    is_object<typename T::metadata>;
    is_object<typename T::pointee>;
};

} // namespace asl
