#pragma once

#include "asl/integers.hpp"
#include "asl/meta.hpp"
#include "asl/utility.hpp"

namespace asl
{

// @Todo Move this to io
class writer
{
public:
    writer() = default;
    ASL_DELETE_COPY_MOVE(writer);
    virtual ~writer() = default;
    
    // @Todo Use string view, or span of bytes?
    virtual void write(const char* str, int64_t len) = 0;
};

class formatter;

template<typename T>
concept formattable = requires (formatter& f, const T& value)
{
    AslFormat(f, value);
};

namespace format_internals
{

struct type_erased_arg
{
    const void* data;
    void (*fn)(formatter&, const void*);

    template<formattable T>
    static constexpr void erased_fn(formatter& f, const void* data)
    {
        AslFormat(f, *reinterpret_cast<const T*>(data));
    }

    template<formattable T>
    explicit constexpr type_erased_arg(const T& arg)
        : data{&arg}
        , fn{erased_fn<T>}
    {}
};

// @Todo Use span
void format(writer*, const char* fmt, const type_erased_arg* args, int64_t arg_count);

} // internals

class formatter
{
    writer* m_writer;

public:
    explicit constexpr formatter(writer* writer)
        : m_writer{writer}
    {}

    // @Todo Use string_view
    constexpr void write(const char* s, int64_t len)
    {
        m_writer->write(s, len);
    }
};

// @Todo Use string_view
template<typename... Args>
void format(writer* w, const char* fmt, const Args&... args)
{
    auto type_erased_args[] = {
        format_internals::type_erased_arg(args)...
    };

    // @Todo Use array extent
    format_internals::format(w, fmt, type_erased_args, types_count<Args...>);
}

} // namespace asl
