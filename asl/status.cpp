#include "asl/status.hpp"
#include "asl/allocator.hpp"
#include "asl/string.hpp"
#include "asl/atomic.hpp"
#include "asl/format.hpp"
#include "asl/string_builder.hpp"

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

    constexpr StatusInternal(asl::string<Allocator>&& msg_, asl::status_code code_)
        : msg{ASL_MOVE(msg_)}
        , code{code_}
    {
        ASL_ASSERT(code != asl::status_code::ok);
        atomic_store(&ref_count, 1);
    }
};

} // anonymous namespace

asl::status::status(status_code code, string_view msg)
    : m_payload{alloc_new<StatusInternal>(g_allocator, msg, code)}
{}

asl::status::status(status_code code, string_view fmt, span<format_internals::type_erased_arg> args)
{
    StringWriter<Allocator> sink{g_allocator};
    format_internals::format(&sink, fmt, args);
    m_payload = alloc_new<StatusInternal>(g_allocator, ASL_MOVE(sink).finish(), code);
}

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

void asl::AslFormat(asl::Formatter& f, const asl::status& s)
{
    string_view status_str{};
    
    switch (s.code())
    {
        case status_code::ok: status_str = "ok"_sv; break;
        case status_code::unknown: status_str = "unknown"_sv; break;
        case status_code::internal: status_str = "internal"_sv; break;
        case status_code::runtime: status_str = "runtime"_sv; break;
        case status_code::invalid_argument: status_str = "invalid_argument"_sv; break;
        default: status_str = "<unknown>"_sv; break;
    }

    if (s.is_inline())
    {
        format(f.writer(), "[{}]", status_str);
    }
    else
    {
        format(f.writer(), "[{}: {}]", status_str, s.message_internal());
    }
}

