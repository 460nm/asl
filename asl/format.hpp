#pragma once

#include "asl/integers.hpp"
#include "asl/meta.hpp"
#include "asl/io.hpp"

namespace asl
{

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

} // namespace internals

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
template<formattable... Args>
void format(writer* w, const char* fmt, const Args&... args)
{
    if constexpr (types_count<Args...> > 0)
    {
        format_internals::type_erased_arg type_erased_args[] = {
            format_internals::type_erased_arg(args)...
        };

        format_internals::format(w, fmt, type_erased_args, types_count<Args...>);
    }
    else
    {
        format_internals::format(w, fmt, nullptr, 0);
    }
}

template<int64_t N>
void AslFormat(formatter& f, const char (&str)[N])
{
    f.write(str, N - 1);
}

void AslFormat(formatter& f, const char* str);

void AslFormat(formatter& f, float);
void AslFormat(formatter& f, double);

void AslFormat(formatter& f, bool);

void AslFormat(formatter& f, uint8_t);
void AslFormat(formatter& f, uint16_t);
void AslFormat(formatter& f, uint32_t);
void AslFormat(formatter& f, uint64_t);

void AslFormat(formatter& f, int8_t);
void AslFormat(formatter& f, int16_t);
void AslFormat(formatter& f, int32_t);
void AslFormat(formatter& f, int64_t);

} // namespace asl
