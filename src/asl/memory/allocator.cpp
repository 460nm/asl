// Copyright 2025 Steven Le Rouzic
//
// SPDX-License-Identifier: BSD-3-Clause

#include "asl/memory/allocator.hpp"
#include "asl/base/assert.hpp"
#include "asl/base/numeric.hpp"
#include "asl/memory/layout.hpp"

#include <cstdlib>

// @Todo zalloc
// @Todo Cookies
// @Todo Debug values

void* asl::GlobalHeap::alloc(const layout& layout)
{
#if defined(ASL_OS_WINDOWS)
    void* ptr = ::_aligned_malloc(
        static_cast<size_t>(layout.size),
        static_cast<size_t>(layout.align));
#elif defined(ASL_OS_LINUX)
    void* ptr = ::aligned_alloc(
        static_cast<size_t>(layout.align),
        static_cast<size_t>(layout.size));
#endif
    ASL_ASSERT(ptr != nullptr); // @Todo panic
    return ptr;
}

void* asl::GlobalHeap::realloc(void* old_ptr, [[maybe_unused]] const layout& old_layout, const layout& new_layout)
{
#if defined(ASL_OS_WINDOWS)
    return ::_aligned_realloc(old_ptr,
        static_cast<size_t>(new_layout.size),
        static_cast<size_t>(new_layout.align));
#elif defined(ASL_OS_LINUX)
    if (new_layout.align <= old_layout.align)
    {
        void* new_ptr = ::realloc(old_ptr, static_cast<size_t>(new_layout.size));
        ASL_ASSERT(new_ptr != nullptr); // @Todo panic
        return new_ptr;
    }

    void* new_ptr = alloc(new_layout);
    asl::memcpy(new_ptr, old_ptr, asl::min(old_layout.size, new_layout.size));
    dealloc(old_ptr, old_layout);
    return new_ptr;
#endif
}

void asl::GlobalHeap::dealloc(void* ptr, const layout&)
{
#if defined(ASL_OS_WINDOWS)
    ::_aligned_free(ptr);
#elif defined(ASL_OS_LINUX)
    ::free(ptr);
#endif
}
