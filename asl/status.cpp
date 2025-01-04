#include "asl/status.hpp"
#include "asl/allocator.hpp"
#include "asl/string.hpp"
#include "asl/atomic.hpp"

// @Todo Use custom allocator
using Allocator = asl::DefaultAllocator;
static Allocator g_allocator{};

namespace
{

struct StatusInternal
{
    asl::string<Allocator> msg;
    asl::status_code code;
    asl::atomic<int32_t> ref_count;

    constexpr StatusInternal(asl::string_view msg_, asl::status_code code_)
        : msg{msg_, g_allocator}
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
    ASL_ASSERT(!is_inline());
    return reinterpret_cast<const StatusInternal*>(m_payload)->code;
}

asl::string_view asl::status::message_internal() const
{
    ASL_ASSERT(!is_inline());
    return reinterpret_cast<const StatusInternal*>(m_payload)->msg;
}

void asl::status::ref()
{
    ASL_ASSERT(!is_inline());
    auto* internal = reinterpret_cast<StatusInternal*>(m_payload);
    atomic_fetch_increment(&internal->ref_count, memory_order::relaxed);
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

