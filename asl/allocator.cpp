#include "asl/allocator.hpp"
#include "asl/assert.hpp"
#include "asl/utility.hpp"
#include "asl/memory.hpp"

#include <cstdlib>

// @Todo zalloc
// @Todo Cookies
// @Todo Debug values

void* asl::GlobalHeap::alloc(const layout& layout)
{
#if ASL_OS_WINDOWS
    void* ptr = ::_aligned_malloc(
        static_cast<size_t>(layout.size),
        static_cast<size_t>(layout.align));
#elif ASL_OS_LINUX
    void* ptr = ::aligned_alloc(
        static_cast<size_t>(layout.size),
        static_cast<size_t>(layout.align));
#endif
    ASL_ASSERT(ptr != nullptr); // @Todo panic
    return ptr;
}

void* asl::GlobalHeap::realloc(void* old_ptr, const layout& old_layout, const layout& new_layout)
{
    if (new_layout.align <= old_layout.align)
    {
        void* new_ptr = ::realloc(old_ptr, static_cast<size_t>(new_layout.size));
        ASL_ASSERT(new_ptr != nullptr); // @Todo panic
        return new_ptr;
    }

    void* new_ptr = alloc(new_layout);
    asl::memcpy(new_ptr, old_ptr, asl::min(old_layout.size, new_layout.size));
    return new_ptr;
}

void asl::GlobalHeap::dealloc(void* ptr, const layout&)
{
    ::free(ptr);
}
