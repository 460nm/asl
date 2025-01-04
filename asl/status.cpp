#include "asl/status.hpp"
#include "asl/allocator.hpp"
#include "asl/string.hpp"
#include "asl/atomic.hpp"

using Allocator = asl::DefaultAllocator;
static Allocator g_allocator{};

namespace
{

struct StatusInternal
{
    // @Todo Use custom allocator
    asl::string<> msg;
    asl::status_code code;
    asl::atomic<int32_t> ref_count;

    constexpr StatusInternal(asl::string_view msg_, asl::status_code code_)
        : msg{msg_}
        , code{code_}
    {
        atomic_store(&ref_count, 1);
    }
};

} // anonymous namespace

asl::status::status(status_code code, string_view msg)
    : m_payload{alloc_new<StatusInternal>(g_allocator, msg, code)}
{}

asl::status_code asl::status::code_internal() const
{
    ASL_ASSERT(m_payload && (bit_cast<uintptr_t>(m_payload) & 1) == 0);
    return reinterpret_cast<const StatusInternal*>(m_payload)->code;
}

void asl::status::unref()
{
    ASL_ASSERT(!is_inline());
    auto* internal = reinterpret_cast<StatusInternal*>(m_payload);
    if (atomic_fetch_decrement(&internal->ref_count, memory_order::release) == 1)
    {
        atomic_fence(memory_order::acquire);
        alloc_delete(g_allocator, internal);
    }
}

