#pragma once

#include "asl/layout.hpp"
#include "asl/memory.hpp"
#include "asl/meta.hpp"
#include "asl/utility.hpp"

namespace asl
{

template<is_object T>
class maybe_uninit
{
    union
    {
        alignas(align_of<T>) char m_storage[size_of<T>];
        T m_value;
    };

public:
    constexpr maybe_uninit() {} // NOLINT(*-member-init)
    
    maybe_uninit(const maybe_uninit&) = delete;
    maybe_uninit(maybe_uninit&&) = delete;

    maybe_uninit& operator=(const maybe_uninit&) = delete;
    maybe_uninit& operator=(maybe_uninit&&) = delete;
    
    constexpr ~maybe_uninit() = default;
    constexpr ~maybe_uninit() requires (!trivially_destructible<T>) {}
    
    constexpr void*       uninit_ptr() && = delete;
    constexpr const void* uninit_ptr() const& { return m_storage; }
    constexpr void*       uninit_ptr() &      { return m_storage; }

    // @Safety Pointer must only be accessed when in initialized state.
    constexpr       T* init_ptr_unsafe() && = delete;
    constexpr const T* init_ptr_unsafe() const& { return &m_value; }
    constexpr       T* init_ptr_unsafe() &      { return &m_value; }

    // @Safety Reference must only be accessed when in initialized state.
    constexpr       T& as_init_unsafe() && = delete;
    constexpr const T& as_init_unsafe() const& { return m_value; }
    constexpr       T& as_init_unsafe() &      { return m_value; }

    // @Safety Must be called only when in uninitialized state.
    template<typename... Args>
    constexpr void init_unsafe(Args&&... args) &
    {
        new(uninit_ptr()) T(ASL_FWD(args)...);
    }

    // @Safety Must be called only when in initialized state.
    constexpr void uninit_unsafe() &
    {
        if constexpr (!trivially_destructible<T>)
        {
            init_ptr_unsafe()->~T();
        }
    }
};

} // namespace asl
