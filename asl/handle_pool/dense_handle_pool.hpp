// Copyright 2025 Steven Le Rouzic
//
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#include "asl/handle_pool/index_pool.hpp"
#include "asl/memory/allocator.hpp"
#include "asl/containers/chunked_buffer.hpp"


namespace asl
{

// @Todo If we want the allocator to be non-copyable, we could
// introduce a reference allocator type that is copyable, and store
// the "main" allocator in the pool.

template<
    is_object T,
    int kIndexBits,
    int kGenBits,
    typename UserType = empty,
    int kUserBits = 0,
    isize_t kChunkSize = 32,
    allocator Allocator = DefaultAllocator>
requires moveable<T> && copyable<Allocator>
class DenseHandlePool
{
    using ThisIndexPool = IndexPool<kIndexBits, kGenBits, UserType, kUserBits, isize_t, Allocator>;

    struct Slot
    {
        ThisIndexPool::handle h;
        T obj;

        template<typename... Args>
        explicit Slot(ThisIndexPool::handle h, Args&&... args)
            : h{h}
            , obj(std::forward<Args>(args)...)
        {}
    };

    using Buffer = chunked_buffer<Slot, kChunkSize, Allocator>;

    ThisIndexPool m_index_pool{};
    Buffer        m_buffer{};

    using config = ThisIndexPool::handle::config;

    template<typename... Args>
    isize_t push(Args&&... args)
        requires constructible_from<T, Args&&...>
    {
        m_buffer.push(typename ThisIndexPool::handle{}, std::forward<Args>(args)...);
        return m_buffer.size() - 1;
    }

public:
    using handle = ThisIndexPool::handle;

    DenseHandlePool() requires default_constructible<Allocator> = default;

    explicit DenseHandlePool(const Allocator& allocator)
        : m_index_pool(allocator)
        , m_buffer(allocator)
    {}

    ASL_DELETE_COPY(DenseHandlePool);
    ASL_DEFAULT_MOVE(DenseHandlePool);
    ~DenseHandlePool() = default;

    [[nodiscard]] bool is_full() const
    {
        return m_index_pool.is_full();
    }

    bool is_valid(handle h) const
    {
        return m_index_pool.is_valid(h);
    }

    template<typename... Args>
    option<handle> acquire(config::UserType user, Args&&... args)
        requires config::kHasUser && constructible_from<T, Args&&...>
    {
        if (is_full()) { return nullopt; }
        const isize_t obj_index = push(std::forward<Args>(args)...);
        const auto handle = m_index_pool.acquire_ensure(user, obj_index);
        m_buffer[obj_index].h = handle;
        return handle;
    }

    template<typename... Args>
    option<handle> acquire(Args&&... args)
        requires (!config::kHasUser) && constructible_from<T, Args&&...>
    {
        if (is_full()) { return nullopt; }
        const isize_t obj_index = push(std::forward<Args>(args)...);
        const auto handle = m_index_pool.acquire_ensure(obj_index);
        m_buffer[obj_index].h = handle;
        return handle;
    }

    template<typename... Args>
    handle acquire_ensure(config::UserType user, Args&&... args)
        requires config::kHasUser && constructible_from<T, Args&&...>
    {
        ASL_ASSERT_RELEASE(!is_full());
        const isize_t obj_index = push(std::forward<Args>(args)...);
        const auto handle = m_index_pool.acquire_ensure(user, obj_index);
        m_buffer[obj_index].h = handle;
        return handle;
    }

    template<typename... Args>
    handle acquire_ensure(Args&&... args)
        requires (!config::kHasUser) && constructible_from<T, Args&&...>
    {
        ASL_ASSERT_RELEASE(!is_full());
        const isize_t obj_index = push(std::forward<Args>(args)...);
        const auto handle = m_index_pool.acquire_ensure(obj_index);
        m_buffer[obj_index].h = handle;
        return handle;
    }

    void release(handle to_release_handle)
    {
        if (!is_valid(to_release_handle)) { return; }

        const auto to_release_index = *m_index_pool.get_payload(to_release_handle);
        if (to_release_index < m_buffer.size() - 1)
        {
            const auto to_swap_index = m_buffer.size() - 1;
            const auto to_swap_handle = m_buffer[to_swap_index].h;

            m_buffer[to_release_index] = std::move(m_buffer[to_swap_index]);
            m_index_pool.exchange_payload(to_swap_handle, to_release_index);
        }

        m_buffer.pop();
        m_index_pool.release(to_release_handle);
    }

    auto get(this auto&& self, handle h)
        -> copy_const_t<un_ref_t<decltype(self)>, T>*
    {
        if (!self.is_valid(h)) { return nullptr; }
        const auto index = *self.m_index_pool.get_payload(h);
        return &self.m_buffer[index].obj;
    }

    auto get_ensure(this auto&& self, handle h)
        -> copy_cref_t<decltype(self), T>
    {
        ASL_ASSERT_RELEASE(self.is_valid(h));
        const auto index = *self.m_index_pool.get_payload(h);
        return std::forward<decltype(self)>(self).m_buffer[index].obj;
    }
};

} // namespace asl
