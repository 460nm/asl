// Copyright 2025 Steven Le Rouzic
//
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#include "asl/base/support.hpp"
#include "asl/base/assert.hpp"
#include "asl/base/numeric.hpp"
#include "asl/containers/buffer.hpp"
#include "asl/allocator/allocator.hpp"
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

    static constexpr isize_t chunk_index(isize_t i)
    {
        static constexpr int kChunkSizeLog2 = countr_zero(uint64_t{kChunkSize});
        return i >> kChunkSizeLog2;
    }

    static constexpr isize_t index_in_chunk(isize_t i)
    {
        static constexpr isize_t kMask = kChunkSize - 1;
        return i & kMask;
    }

    struct PerChunkIterator
    {
        isize_t from_chunk;
        isize_t to_chunk;

        isize_t from_index_in_chunk;
        isize_t to_index_in_chunk;

        [[nodiscard]] constexpr bool has_more() const
        {
            return from_chunk <= to_chunk;
        }

        void advance()
        {
            from_chunk += 1;
            from_index_in_chunk = 0;
        }

        [[nodiscard]] constexpr isize_t chunk() const { return from_chunk; }

        span<maybe_uninit<T>> make_span(Chunk& chunk) const
        {
            isize_t from = from_index_in_chunk;
            isize_t to = (from_chunk == to_chunk) ? to_index_in_chunk : kChunkSize - 1;
            return chunk.as_span().subspan(from, to - from + 1);
        }
    };

    PerChunkIterator make_index_iterator(isize_t from, isize_t to)
    {
        return PerChunkIterator {
            chunk_index(from), chunk_index(to),
            index_in_chunk(from), index_in_chunk(to)
        };
    }

    buffer<Chunk*, Allocator>    m_chunks;
    isize_t                      m_size{};

    void resize_uninit_inner(isize_t new_size)
    {
        ASL_ASSERT(new_size >= 0);

        if constexpr (!is_trivially_destructible<T>)
        {
            const isize_t old_size = size();
            if (new_size < old_size)
            {
                for (PerChunkIterator it = make_index_iterator(new_size, old_size - 1);
                    it.has_more();
                    it.advance())
                {
                    auto span = it.make_span(*m_chunks[it.chunk()]);
                    for (auto& el: span)
                    {
                        el.destroy_unsafe();
                    }
                }
            }
        }

        reserve_capacity(new_size);
        m_size = new_size;
    }

    template<typename... Args>
    void resize_construct(isize_t new_size, Args&&... args)
        requires constructible_from<T, Args&&...>
    {
        const isize_t old_size = m_size;
        resize_uninit_inner(new_size);

        if (new_size > old_size)
        {
            for (PerChunkIterator it = make_index_iterator(old_size, new_size - 1);
                it.has_more();
                it.advance())
            {
                auto span = it.make_span(*m_chunks[it.chunk()]);
                for (auto& uninit: span)
                {
                    uninit.construct_unsafe(std::forward<Args>(args)...);
                }
            }
        }
    }

    void copy_from(const chunked_buffer& other)
        requires copyable<T>
    {
        const isize_t this_size = size();
        isize_t to_copy_assign = asl::min(other.size(), this_size);

        resize_uninit_inner(other.size());

        for (PerChunkIterator it = make_index_iterator(0, to_copy_assign - 1);
            it.has_more();
            it.advance())
        {
            auto to_span = it.make_span(*m_chunks[it.chunk()]);
            auto from_span = it.make_span(*other.m_chunks[it.chunk()]);

            copy_assign_n(
                reinterpret_cast<T*>(to_span.data()), // NOLINT(*-reinterpret-cast)
                reinterpret_cast<const T*>(from_span.data()), // NOLINT(*-reinterpret-cast)
                to_span.size());
        }

        if (other.size() > this_size)
        {
            for (PerChunkIterator it = make_index_iterator(to_copy_assign, other.size() - 1);
                it.has_more();
                it.advance())
            {
                auto to_span = it.make_span(*m_chunks[it.chunk()]);
                auto from_span = it.make_span(*other.m_chunks[it.chunk()]);

                copy_uninit_n(
                    reinterpret_cast<T*>(to_span.data()), // NOLINT(*-reinterpret-cast)
                    reinterpret_cast<const T*>(from_span.data()), // NOLINT(*-reinterpret-cast)
                    to_span.size());
            }
        }

        ASL_ASSERT(size() == other.size());
    }

public:
    constexpr chunked_buffer()
        requires is_default_constructible<Allocator>
        = default;

    explicit constexpr chunked_buffer(Allocator allocator)
        : m_chunks{std::move(allocator)}
    {}

    constexpr chunked_buffer(const chunked_buffer& other)
        requires copyable<T> && copy_constructible<Allocator>
        : m_chunks{other.m_chunks.allocator_copy()}
    {
        copy_from(other);
    }

    constexpr chunked_buffer(chunked_buffer&& other)
        : m_chunks{std::move(other.m_chunks)}
        , m_size{std::exchange(other.m_size, 0)}
    {
        ASL_ASSERT(other.m_chunks.size() == 0);
    }

    constexpr chunked_buffer& operator=(const chunked_buffer& other)
        requires copyable<T>
    {
        if (&other == this) { return *this; }
        copy_from(other);
        return *this;
    }

    constexpr chunked_buffer& operator=(chunked_buffer&& other)
    {
        if (&other == this) { return *this; }
        destroy();
        m_chunks = std::move(other.m_chunks);
        m_size = std::exchange(other.m_size, 0);
        ASL_ASSERT(other.m_chunks.size() == 0);
        return *this;
    }

    ~chunked_buffer()
    {
        destroy();
    }

    void clear()
    {
        if constexpr (is_trivially_destructible<T>)
        {
            m_size = 0;
        }
        else if (m_size > 0)
        {
            resize_uninit_inner(0);
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

    constexpr auto&& operator[](this auto&& self, isize_t i)
    {
        ASL_ASSERT(i >= 0 && i < self.m_size);
        return std::forward_like<decltype(self)>(
            (*std::forward<decltype(self)>(self).m_chunks[chunk_index(i)])
                [index_in_chunk(i)].as_init_unsafe()
        );
    }

    constexpr T& push(auto&&... args)
        requires constructible_from<T, decltype(args)&&...>
    {
        const isize_t chunk = chunk_index(m_size);
        const isize_t in_chunk = index_in_chunk(m_size);

        if (m_size == capacity())
        {
            resize_uninit_inner(m_size + 1);
        }
        else
        {
            m_size += 1;
        }

        void* uninit = &(*m_chunks[chunk])[in_chunk];
        return *construct_at<T>(uninit, std::forward<decltype(args)>(args)...);
    }

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
            // @Todo(C++26) _unsafe shouldn't be needed with trivial unions
            auto* chunk = alloc_uninit_unsafe<Chunk>(m_chunks.allocator());
            m_chunks.push(chunk);
        }
    }

    void resize(isize_t new_size)
        requires is_default_constructible<T>
    {
        if constexpr (is_trivially_default_constructible<T>)
        {
            resize_zero(new_size);
        }
        else
        {
            resize_construct(new_size);
        }
    }

    void resize(isize_t new_size, const T& value)
        requires copy_constructible<T>
    {
        resize_construct(new_size, value);
    }

    void resize_zero(isize_t new_size)
        requires is_trivially_default_constructible<T>
    {
        const isize_t old_size = m_size;
        resize_uninit_inner(new_size);

        if (new_size > old_size)
        {
            for (PerChunkIterator it = make_index_iterator(old_size, new_size - 1);
                it.has_more();
                it.advance())
            {
                auto span = it.make_span(*m_chunks[it.chunk()]);
                asl::memzero(span.data(), span.size_bytes());
            }
        }
    }

    void resize_uninit(isize_t new_size)
        requires is_trivially_default_constructible<T>
    {
        resize_uninit_inner(new_size);
    }

    void pop()
    {
        ASL_ASSERT(size() > 0);
        resize_uninit_inner(size() - 1);
    }

    template<typename Chunk>
    class generic_iterator
    {
        isize_t     m_index;
        span<Chunk> m_chunks;

    public:
        constexpr generic_iterator(isize_t index, span<Chunk> chunks)
            : m_index{index}
            , m_chunks{chunks}
        {}

        constexpr generic_iterator& operator++()
        {
            m_index += 1;
            return *this;
        }

        constexpr generic_iterator operator++(int)
        {
            auto tmp = *this;
            m_index += 1;
            return tmp;
        }

        constexpr bool operator==(this generic_iterator self, generic_iterator other)
        {
            ASL_ASSERT(self.m_chunks.data() == other.m_chunks.data());
            return self.m_index == other.m_index;
        }

        constexpr auto& operator*(this generic_iterator self)
        {
            ASL_ASSERT(self.m_index >= 0);
            return (*self.m_chunks[chunk_index(self.m_index)])[index_in_chunk(self.m_index)].as_init_unsafe();
        }

        constexpr auto* operator->(this generic_iterator self)
        {
            return &*self;
        }
    };

    using iterator       = generic_iterator<Chunk*>;
    using const_iterator = generic_iterator<const Chunk* const>;

    constexpr iterator begin() { return iterator{0, m_chunks}; }
    constexpr iterator end()   { return iterator{m_size, m_chunks}; }

    constexpr const_iterator begin() const
    {
        return const_iterator{0, {m_chunks.data(), m_chunks.size()}};
    }

    constexpr const_iterator end() const
    {
        return const_iterator{m_size, {m_chunks.data(), m_chunks.size()}};
    }
};

} // namespace asl

