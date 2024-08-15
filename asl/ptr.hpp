#pragma once

#include "asl/integers.hpp"
#include "asl/meta.hpp"
#include "asl/utility.hpp"

namespace asl {

constexpr auto addr(auto&& ref) { return __builtin_addressof(ref); }

namespace ptr_internal {

template<is_object T>
class object_meta
{
public:
    using pointee = T;
    using metadata = empty;
};

template<is_void T>
class void_meta
{
public:
    using pointee = T;
    using metadata = empty;
};

template<typename T>
class array_meta;

template<is_object T, isize_t N>
class array_meta<T[N]>
{
public:
    using pointee = T[N];
    using metadata = empty;
};

template<is_object T>
class array_meta<T[]>
{
public:
    using pointee = T;
    using metadata = isize_t;
};

template<typename T>
class ptr_like_meta
{
    static_assert(is_func<T> || is_ref<T>);
    
public:
    using pointee = as_raw_ptr_t<T>;
    using metadata = empty;
};

template<typename T>
constexpr auto get_meta()
{
    if constexpr (is_object<T>)
    {
        return object_meta<T>{};
    }
    else if constexpr (is_void<T>)
    {
        return void_meta<T>{};
    }
    else if constexpr (is_array<T>)
    {
        return array_meta<T>{};
    }
    else if constexpr (is_ref<T> || is_func<T>)
    {
        return ptr_like_meta<T>{};
    }
}

template<typename T>
using meta = decltype(get_meta<T>());

} // namespace ptr_internal

template<typename T>
concept is_ptr_metadata = requires
{
    requires is_object<typename T::pointee> || is_void<typename T::pointee> || is_array<typename T::pointee>;
    requires is_object<typename T::metadata>;
};

template<typename T>
class ptr final
{
    using meta = ptr_internal::meta<T>;
    static_assert(is_ptr_metadata<meta>);
    
public:
    using pointee = meta::pointee;
    using metadata = meta::metadata;

private:
    pointee* m_ptr = nullptr;
    ASL_NO_UNIQUE_ADDRESS metadata m_metadata;

public:
};

} // namespace asl
