#include "asl/status.hpp"
#include "asl/allocator.hpp"

using Allocator = asl::DefaultAllocator;
static Allocator g_allocator{};

namespace
{

struct StatusInternal
{
    asl::string_view msg;
    asl::status_code code;
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

