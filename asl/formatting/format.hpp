#pragma once

#include "asl/base/integers.hpp"
#include "asl/base/meta.hpp"
#include "asl/io/writer.hpp"
#include "asl/types/span.hpp"
#include "asl/strings/string_view.hpp"

namespace asl
{

class Formatter;

template<typename T>
concept formattable = requires (Formatter& f, const T& value)
{
    AslFormat(f, value);
};

namespace format_internals
{

struct type_erased_arg
{
    const void* data;
    void (*fn)(Formatter&, const void*);

    template<formattable T>
    static constexpr void erased_fn(Formatter& f, const void* data)
    {
        AslFormat(f, *reinterpret_cast<const T*>(data));
    }

    template<formattable T>
    explicit constexpr type_erased_arg(const T& arg)
        : data{&arg}
        , fn{erased_fn<T>}
    {}
};

void format(Writer*, string_view fmt, span<const type_erased_arg> args);

} // namespace internals

class Formatter
{
    Writer* m_writer;

public:
    explicit constexpr Formatter(Writer* writer)
        : m_writer{writer}
    {}

    constexpr void write(string_view s)
    {
        m_writer->write(as_bytes(s.as_span()));
    }

    constexpr Writer* writer() const { return m_writer; }
};

template<formattable... Args>
void format(Writer* w, string_view fmt, const Args&... args)
{
    if constexpr (types_count<Args...> > 0)
    {
        format_internals::type_erased_arg type_erased_args[] = {
            format_internals::type_erased_arg(args)...
        };

        format_internals::format(w, fmt, type_erased_args);
    }
    else
    {
        format_internals::format(w, fmt, {});
    }
}

template<isize_t N>
void AslFormat(Formatter& f, const char (&str)[N])
{
    f.write(string_view(str, N - 1));
}

void AslFormat(Formatter& f, const char* str);

inline void AslFormat(Formatter& f, string_view sv)
{
    f.write(sv);
}

void AslFormat(Formatter& f, float);
void AslFormat(Formatter& f, double);

void AslFormat(Formatter& f, bool);

void AslFormat(Formatter& f, uint8_t);
void AslFormat(Formatter& f, uint16_t);
void AslFormat(Formatter& f, uint32_t);
void AslFormat(Formatter& f, uint64_t);

void AslFormat(Formatter& f, int8_t);
void AslFormat(Formatter& f, int16_t);
void AslFormat(Formatter& f, int32_t);
void AslFormat(Formatter& f, int64_t);

string_view format_uint64(uint64_t value, span<char, 20> buffer);

} // namespace asl
