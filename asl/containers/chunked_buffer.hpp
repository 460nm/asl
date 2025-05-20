// Copyright 2025 Steven Le Rouzic
//
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#include "asl/base/utility.hpp"
#include "asl/base/assert.hpp"
#include "asl/base/numeric.hpp"
#include "asl/containers/buffer.hpp"
#include "asl/memory/allocator.hpp"
#include "asl/types/array.hpp"
#include "asl/types/maybe_uninit.hpp"

namespace asl
{

template<
    is_object T,
    isize_t kChunkSize,
    allocator Allocator = DefaultAllocator>
class chunked_buffer
{
    static_assert(kChunkSize > 0 && is_pow2(kChunkSize));

    using Chunk = array<maybe_uninit<T>, kChunkSize>;

    buffer<Chunk*, Allocator>    m_chunks;
    isize_t                      m_size{};

    void resize_uninit_inner(isize_t new_size)
    {
        if constexpr (!trivially_destructible<T>)
        {
            const isize_t old_size = size();
            if (new_size < old_size)
            {
                // @Todo Destroy
            }
        }
        reserve_capacity(new_size);
        m_size = new_size;
    }

public:
    constexpr chunked_buffer()
        requires default_constructible<Allocator>
        = default;

    explicit constexpr chunked_buffer(Allocator allocator)
        : m_chunks{std::move(allocator)}
    {}

    // @Todo copy constructor
    constexpr chunked_buffer(const chunked_buffer& other)
        requires copyable<T> && copy_constructible<Allocator>
    {}

    constexpr chunked_buffer(chunked_buffer&& other)
        : m_chunks{std::move(other.m_chunks)}
        , m_size{asl::exchange(other.m_size, 0)}
    {
        ASL_ASSERT(m_chunks.size() == 0);
    }
    
    // @Todo copy assignment
    constexpr chunked_buffer& operator=(const chunked_buffer& other)
        requires copyable<T>
    {
        if (&other == this) { return *this; }
        return *this;
    }
    
    // @Todo move assignment
    constexpr chunked_buffer& operator=(chunked_buffer&& other)
    {
        if (&other == this) { return *this; }
        return *this;
    }

    ~chunked_buffer()
    {
        destroy();
    }

    void clear()
    {
        if constexpr (trivially_destructible<T>)
        {
            m_size = 0;
        }
        else
        {
            for (Chunk* chunk: m_chunks)
            {
                if (m_size == 0) { break; }

                isize_t in_chunk = asl::min(m_size, kChunkSize);
                destroy_n(&(*chunk)[0].as_init_unsafe(), in_chunk);
            
                m_size -= in_chunk;
            }
            ASL_ASSERT(m_size == 0);
        }
    }

    void destroy()
    {
        clear();
        ASL_ASSERT(size() == 0);
        
        for (Chunk* chunk:  m_chunks)
        {
            alloc_delete(m_chunks.allocator(), chunk);
        }

        m_chunks.destroy();
    }

    [[nodiscard]] constexpr isize_t size() const { return m_size; }

    [[nodiscard]] constexpr bool is_empty() const { return size() == 0; }

    [[nodiscard]] constexpr isize_t capacity() const
    {
        return m_chunks.size() * kChunkSize;
    }

    // @Todo iterators
    // @Todo element access
    // @Todo push

    void reserve_capacity(isize_t new_capacity)
    {
        new_capacity = round_up_pow2(new_capacity, kChunkSize);
        if (new_capacity <= capacity()) { return; }

        const isize_t required_chunks = new_capacity / kChunkSize;
        const isize_t additional_chunks = required_chunks - m_chunks.size();
        ASL_ASSERT(additional_chunks > 0);

        m_chunks.reserve_capacity(required_chunks);
        for (isize_t i = 0; i < additional_chunks; ++i)
        {
            auto* chunk = alloc_uninit<Chunk>(m_chunks.allocator());
            m_chunks.push(chunk);
        }
    }
    
    // @Todo resize (default/with value)
    // @Todo resize_zero

    // @Todo resize_uninit
    void resize_uninit(isize_t new_size)
        requires trivially_default_constructible<T>
    {
        resize_uninit_inner(new_size);
    }
};

} // namespace asl

