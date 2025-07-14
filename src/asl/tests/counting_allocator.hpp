// Copyright 2025 Steven Le Rouzic
//
// SPDX-License-Identifier: BSD-3-Clause

#include "asl/memory/allocator.hpp"

struct CountingAllocator
{
    struct Stats
    {
        isize_t alloc_count{};
        isize_t realloc_count{};
        isize_t dealloc_count{};
        isize_t alive_bytes{};

        [[nodiscard]] constexpr isize_t any_alloc_count() const
        {
            return alloc_count + realloc_count;
        }
    };

    Stats* stats;

    [[nodiscard]]
    void* alloc(const asl::layout& layout) const
    {
        stats->alloc_count += 1;
        stats->alive_bytes += layout.size;
        return asl::GlobalHeap::alloc(layout);
    }

    void* realloc(void* ptr, const asl::layout& old, const asl::layout& new_layout) const
    {
        stats->realloc_count += 1;
        stats->alive_bytes += new_layout.size - old.size;
        return asl::GlobalHeap::realloc(ptr, old, new_layout);
    }

    void dealloc(void* ptr, const asl::layout& layout) const
    {
        stats->dealloc_count += 1;
        stats->alive_bytes -= layout.size;
        asl::GlobalHeap::dealloc(ptr, layout);
    }

    constexpr bool operator==(const CountingAllocator&) const { return true; }
};
static_assert(asl::allocator<CountingAllocator>);
