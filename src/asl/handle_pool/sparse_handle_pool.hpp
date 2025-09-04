// Copyright 2025 Steven Le Rouzic
//
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#include "asl/handle_pool/index_pool.hpp"
#include "asl/allocator/allocator.hpp"
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
requires copyable<Allocator>
class SparseHandlePool
{
    using ThisIndexPool = IndexPool<kIndexBits, kGenBits, UserType, kUserBits, empty, Allocator>;

    struct Slot
    {
        ThisIndexPool::handle h;
        maybe_uninit<T> obj;

        Slot() = default;
        ASL_DELETE_COPY_MOVE(Slot);

        ~Slot()
        {
            if (!h.is_null())
            {
                obj.destroy_unsafe();
            }
        }
    };

    using Buffer = chunked_buffer<Slot, kChunkSize, Allocator>;

    ThisIndexPool m_index_pool{};
    Buffer        m_buffer{};

    using config = ThisIndexPool::handle::config;

    template<typename... Args>
    void set_object(ThisIndexPool::handle h, Args&&... args)
    {
        const auto index = static_cast<isize_t>(h.index());
        if (m_buffer.size() <= index)
        {
            m_buffer.resize(index + 1);
        }
        m_buffer[index].h = h;
        m_buffer[index].obj.construct_unsafe(std::forward<Args>(args)...);
    }

public:
    using handle = ThisIndexPool::handle;

    SparseHandlePool() requires is_default_constructible<Allocator> = default;

    explicit SparseHandlePool(const Allocator& allocator)
        : m_index_pool(allocator)
        , m_buffer(allocator)
    {}

    ASL_DELETE_COPY(SparseHandlePool);
    ASL_DEFAULT_MOVE(SparseHandlePool);
    ~SparseHandlePool() = default;

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
        const auto handle = m_index_pool.acquire_ensure(user);
        set_object(handle, std::forward<Args>(args)...);
        return handle;
    }

    template<typename... Args>
    option<handle> acquire(Args&&... args)
        requires (!config::kHasUser) && constructible_from<T, Args&&...>
    {
        if (is_full()) { return nullopt; }
        const auto handle = m_index_pool.acquire_ensure();
        set_object(handle, std::forward<Args>(args)...);
        return handle;
    }

    template<typename... Args>
    handle acquire_ensure(config::UserType user, Args&&... args)
        requires config::kHasUser && constructible_from<T, Args&&...>
    {
        ASL_ASSERT_RELEASE(!is_full());
        const auto handle = m_index_pool.acquire_ensure(user);
        set_object(handle, std::forward<Args>(args)...);
        return handle;
    }

    template<typename... Args>
    handle acquire_ensure(Args&&... args)
        requires (!config::kHasUser) && constructible_from<T, Args&&...>
    {
        ASL_ASSERT_RELEASE(!is_full());
        const auto handle = m_index_pool.acquire_ensure();
        set_object(handle, std::forward<Args>(args)...);
        return handle;
    }

    void release(handle h)
    {
        if (!is_valid(h)) { return; }
        auto& slot = m_buffer[static_cast<isize_t>(h.index())];
        slot.h = {};
        slot.obj.destroy_unsafe();
        m_index_pool.release(h);
    }

    auto get(this auto&& self, handle h)
        -> copy_const_t<remove_ref_t<decltype(self)>, T>*
    {
        if (!self.is_valid(h)) { return nullptr; }
        return &std::forward<decltype(self)>(self).m_buffer[static_cast<isize_t>(h.index())]
            .obj.as_init_unsafe();
    }

    auto get_ensure(this auto&& self, handle h)
        -> copy_cref_t<decltype(self), T>
    {
        ASL_ASSERT_RELEASE(self.is_valid(h));
        return std::forward<decltype(self)>(self).m_buffer[static_cast<isize_t>(h.index())]
            .obj.as_init_unsafe();
    }
};

} // namespace asl
