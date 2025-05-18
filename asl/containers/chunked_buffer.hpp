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

public:
    constexpr chunked_buffer()
        requires default_constructible<Allocator>
        = default;

    explicit constexpr chunked_buffer(Allocator allocator)
        : m_chunks{std::move(allocator)}
    {}

    // @Todo copy constructor
    // @Todo move constructor
    // @Todo copy assignment
    // @Todo move assignment
    // @Todo destructor
    // @Todo iterators
    // @Todo element access
    // @Todo clear
    // @Todo push
    // @Todo destroy
    // @Todo reserve_capacity
    // @Todo resize (default/with value)
    // @Todo resize_zero
    // @Todo resize_uninit

    [[nodiscard]] constexpr isize_t size() const { return m_size; }

    [[nodiscard]] constexpr bool is_empty() const { return size() == 0; }

    [[nodiscard]] constexpr isize_t capacity() const
    {
        return m_chunks.size() * kChunkSize;
    }
};

} // namespace asl

