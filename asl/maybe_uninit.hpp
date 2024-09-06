#pragma once

#include "asl/annotations.hpp"
#include "asl/layout.hpp"
#include "asl/memory.hpp"
#include "asl/meta.hpp"

namespace asl
{

template<is_object T>
class alignas(align_of<T>) maybe_uninit
{
    char m_storage[size_of<T>];

public:
    constexpr void* uninit_ptr() const { return m_storage; }

    // @Safety Pointer must only be accessed when in initialized state.
    constexpr const T* init_ptr(unsafe) const { return reinterpret_cast<const T*>(m_storage); }
    constexpr       T* init_ptr(unsafe)       { return reinterpret_cast<      T*>(m_storage); }

    // @Safety Reference must only be accessed when in initialized state.
    constexpr const T& as_init(unsafe) const { return *reinterpret_cast<const T*>(m_storage); }
    constexpr       T& as_init(unsafe)       { return *reinterpret_cast<      T*>(m_storage); }

    // @Safety Must be called only when in uninitialized state.
    template<typename... Args>
    inline void init(unsafe, Args&&... args)
    {
        new(uninit_ptr()) T(ASL_FWD(args)...);
    }

    // @Safety Must be called only when in initialized state.
    inline void uninit(unsafe)
    {
        if constexpr (!trivially_destructible<T>)
        {
            init_ptr(unsafe("Caller has checked init state"))->~T();
        }
    }
};

} // namespace asl
