#pragma once

#include "asl/meta.hpp"

namespace asl
{

enum class memory_order : int // NOLINT(*-enum-size)
{
    relaxed        = __ATOMIC_RELAXED,
    acquire        = __ATOMIC_ACQUIRE,
    release        = __ATOMIC_RELEASE,
    acq_rel        = __ATOMIC_ACQ_REL,
    seq_cst        = __ATOMIC_SEQ_CST,
};

template<typename T> struct atomic { T m_value{}; };

inline void atomic_fence(memory_order order)
{
    __atomic_thread_fence((int)order);
}

template<is_integer T>
inline void atomic_store(atomic<T>* a, T value, memory_order order = memory_order::relaxed)
{
    __atomic_store(&a->m_value, &value, (int)order);
}

template<is_integer T>
inline T atomic_load(atomic<T>* a, memory_order order = memory_order::relaxed)
{
    T value;
    __atomic_load(&a->m_value, &value, (int)order);
    return value;
}

template<typename T>
inline T atomic_fetch_increment(atomic<T>* a, memory_order order = memory_order::relaxed)
{
    return __atomic_fetch_add(&a->m_value, 1, (int)order);
}

template<typename T>
inline T atomic_fetch_decrement(atomic<T>* a, memory_order order = memory_order::relaxed)
{
    return __atomic_fetch_sub(&a->m_value, 1, (int)order);
}

} // namespace asl

