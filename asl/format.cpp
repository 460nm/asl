#include "asl/format.hpp"
#include "asl/utility.hpp"
#include "asl/assert.hpp"

void asl::format_internals::format(
    writer* writer,
    const char* fmt,
    const type_erased_arg* args,
    int64_t arg_count)
{
    formatter f(writer);


    int64_t arg = 0;

    const char* begin = fmt;
    while (*fmt != '\0')
    {
        if (fmt[0] == '{')
        {
            if (fmt[1] == '}')
            {
                if (arg < arg_count)
                {
                    f.write(begin, fmt - begin);
                    fmt += 2;
                    begin = fmt;

                    args[arg].fn(f, args[arg].data);
                    arg += 1;
                }
                else
                {
                    f.write(begin, fmt - begin);
                    fmt += 2;
                    begin = fmt;

                    f.write("<ERROR>", 7);
                }
            }
            else if (fmt[1] == '{')
            {
                fmt += 1;
                f.write(begin, fmt - begin);
                fmt += 1;
                begin = fmt;
            }
            else
            {
                f.write(begin, fmt - begin);
                fmt += 1;
                begin = fmt;

                f.write("<ERROR>", 7);
            }
        }
        else if (fmt[0] == '}' && fmt[1] == '}')
        {
            fmt += 1;
            f.write(begin, fmt - begin);
            fmt += 1;
            begin = fmt;
        }
        else
        {
            fmt += 1;
        }
    }

    f.write(begin, fmt - begin);
}

void asl::AslFormat(formatter& f, float)
{
    f.write("<FLOAT>", 7); // @Todo Float formatting
}

void asl::AslFormat(formatter& f, double)
{
    f.write("<DOUBLE>", 8); // @Todo Float formatting
}

void asl::AslFormat(formatter& f, uint8_t v)
{
    AslFormat(f, static_cast<uint64_t>(v));
}

void asl::AslFormat(formatter& f, uint16_t v)
{
    AslFormat(f, static_cast<uint64_t>(v));
}

void asl::AslFormat(formatter& f, uint32_t v)
{
    AslFormat(f, static_cast<uint64_t>(v));
}

void asl::AslFormat(formatter& f, uint64_t v)
{
    static constexpr char pairs[] = {
        '0', '0', '0', '1', '0', '2', '0', '3', '0', '4',
        '0', '5', '0', '6', '0', '7', '0', '8', '0', '9',
        '1', '0', '1', '1', '1', '2', '1', '3', '1', '4',
        '1', '5', '1', '6', '1', '7', '1', '8', '1', '9',
        '2', '0', '2', '1', '2', '2', '2', '3', '2', '4',
        '2', '5', '2', '6', '2', '7', '2', '8', '2', '9',
        '3', '0', '3', '1', '3', '2', '3', '3', '3', '4',
        '3', '5', '3', '6', '3', '7', '3', '8', '3', '9',
        '4', '0', '4', '1', '4', '2', '4', '3', '4', '4',
        '4', '5', '4', '6', '4', '7', '4', '8', '4', '9',
        '5', '0', '5', '1', '5', '2', '5', '3', '5', '4',
        '5', '5', '5', '6', '5', '7', '5', '8', '5', '9',
        '6', '0', '6', '1', '6', '2', '6', '3', '6', '4',
        '6', '5', '6', '6', '6', '7', '6', '8', '6', '9',
        '7', '0', '7', '1', '7', '2', '7', '3', '7', '4',
        '7', '5', '7', '6', '7', '7', '7', '8', '7', '9',
        '8', '0', '8', '1', '8', '2', '8', '3', '8', '4',
        '8', '5', '8', '6', '8', '7', '8', '8', '8', '9',
        '9', '0', '9', '1', '9', '2', '9', '3', '9', '4',
        '9', '5', '9', '6', '9', '7', '9', '8', '9', '9',
    };

    static constexpr int32_t kMaxDigits = 20;
    char buffer[kMaxDigits];
    int32_t cursor = kMaxDigits;

    auto write_two = [&buffer, &cursor](const char* str)
    {
        ASL_ASSERT(cursor >= 2);
        buffer[--cursor] = str[1];
        buffer[--cursor] = str[0];
    };

    auto write_one = [&buffer, &cursor](char c)
    {
        ASL_ASSERT(cursor >= 1);
        buffer[--cursor] = c;
    };

    while (v >= 100)
    {
        uint64_t x = v % 100;
        v /= 100;
        write_two(pairs + x * 2);
    }

    if (v >= 10)
    {
        write_two(pairs + v * 2);
    }
    else if (v > 0 || cursor == kMaxDigits)
    {
        ASL_ASSERT(v < 10);
        write_one('0' + (char)v);
    }

    f.write(buffer + cursor, kMaxDigits - cursor);
}

void asl::AslFormat(formatter& f, int8_t v)
{
    AslFormat(f, static_cast<int64_t>(v));
}

void asl::AslFormat(formatter& f, int16_t v)
{
    AslFormat(f, static_cast<int64_t>(v));
}

void asl::AslFormat(formatter& f, int32_t v)
{
    AslFormat(f, static_cast<int64_t>(v));
}

void asl::AslFormat(formatter& f, int64_t v)
{
    if (v < 0)
    {
        f.write("-", 1);
        uint64_t absolute_value = ~(bit_cast<uint64_t>(v) - 1);
        AslFormat(f, absolute_value);
    }
    else
    {
        AslFormat(f, static_cast<uint64_t>(v));
    }
}
