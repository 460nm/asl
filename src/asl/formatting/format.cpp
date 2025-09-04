// Copyright 2025 Steven Le Rouzic
//
// SPDX-License-Identifier: BSD-3-Clause

#include "asl/formatting/format.hpp"
#include "asl/base/assert.hpp"
#include "asl/base/bits.hpp"
#include "asl/base/meta.hpp"
#include "asl/base/numeric.hpp"

void asl::format_internals::format(
    Writer* writer,
    string_view fmt,
    span<const type_erased_arg> args)
{
    Formatter f(writer);

    auto arg_it = args.begin();
    auto arg_end = args.end();

    isize_t i = 0;
    while (i < fmt.size())
    {
        if (fmt[i] == '{')
        {
            if (i + 1 < fmt.size())
            {
                if (fmt[i + 1] == '}')
                {
                    f.write(fmt.substr(0, i));
                    fmt = fmt.substr(i + 2);
                    i = 0;

                    if (arg_it == arg_end)
                    {
                        f.write("<ERROR>");
                    }
                    else
                    {
                        arg_it->fn(f, arg_it->data);
                        arg_it++;
                    }

                    continue;
                }

                if (fmt[i + 1] == '{')
                {
                    f.write(fmt.substr(0, i + 1));
                    fmt = fmt.substr(i + 2);
                    i = 0;

                    continue;
                }
            }

            f.write(fmt.substr(0, i));
            fmt = fmt.substr(i + 1);
            i = 0;

            f.write("<ERROR>");
        }
        else if (i + 1 < fmt.size() && fmt[i] == '}' && fmt[i + 1] == '}')
        {
            f.write(fmt.substr(0, i + 1));
            fmt = fmt.substr(i + 2);
            i = 0;
        }
        else
        {
            i += 1;
        }
    }

    f.write(fmt);
}

void asl::AslFormat(Formatter& f, const char* str)
{
    f.write({str, asl::strlen(str)});
}

void asl::AslFormat(Formatter& f, bool v)
{
    if (v)
    {
        f.write("true");
    }
    else
    {
        f.write("false");
    }
}

void asl::AslFormat(Formatter& f, uint8_t v)
{
    AslFormat(f, static_cast<uint64_t>(v));
}

void asl::AslFormat(Formatter& f, uint16_t v)
{
    AslFormat(f, static_cast<uint64_t>(v));
}

void asl::AslFormat(Formatter& f, uint32_t v)
{
    AslFormat(f, static_cast<uint64_t>(v));
}

static constexpr int32_t kMaxUint64Digits = 20;

asl::string_view asl::format_uint64(uint64_t v, asl::span<char, kMaxUint64Digits> buffer)
{
    static constexpr char s_pairs_storage[] = {
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

    static constexpr span s_pairs = s_pairs_storage;
    int32_t cursor = kMaxUint64Digits;

    auto write_two = [&buffer, &cursor](span<const char, 2> str)
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
        const uint64_t x = v % 100;
        v /= 100;
        write_two(s_pairs.subspan(static_cast<isize_t>(x * 2)).first<2>());
    }

    if (v >= 10)
    {
        write_two(s_pairs.subspan(static_cast<isize_t>(v * 2)).first<2>());
    }
    else if (v > 0 || cursor == kMaxUint64Digits)
    {
        ASL_ASSERT(v < 10);
        write_one(static_cast<char>('0' + v));
    }

    return string_view(buffer.data(), kMaxUint64Digits).substr(cursor);
}

void asl::AslFormat(Formatter& f, uint64_t v)
{
    char buffer[kMaxUint64Digits];
    f.write(format_uint64(v, buffer));
}

void asl::AslFormat(Formatter& f, int8_t v)
{
    AslFormat(f, static_cast<int64_t>(v));
}

void asl::AslFormat(Formatter& f, int16_t v)
{
    AslFormat(f, static_cast<int64_t>(v));
}

void asl::AslFormat(Formatter& f, int32_t v)
{
    AslFormat(f, static_cast<int64_t>(v));
}

void asl::AslFormat(Formatter& f, int64_t v)
{
    if (v < 0)
    {
        f.write("-");
        const uint64_t absolute_value = ~(std::bit_cast<uint64_t>(v) - 1);
        AslFormat(f, absolute_value);
    }
    else
    {
        AslFormat(f, static_cast<uint64_t>(v));
    }
}
static constexpr isize_t kZeroCount = 100;
static constexpr char kZeros[kZeroCount] = {
    '0', '0', '0', '0', '0', '0', '0', '0', '0', '0',
    '0', '0', '0', '0', '0', '0', '0', '0', '0', '0',
    '0', '0', '0', '0', '0', '0', '0', '0', '0', '0',
    '0', '0', '0', '0', '0', '0', '0', '0', '0', '0',
    '0', '0', '0', '0', '0', '0', '0', '0', '0', '0',
    '0', '0', '0', '0', '0', '0', '0', '0', '0', '0',
    '0', '0', '0', '0', '0', '0', '0', '0', '0', '0',
    '0', '0', '0', '0', '0', '0', '0', '0', '0', '0',
    '0', '0', '0', '0', '0', '0', '0', '0', '0', '0',
    '0', '0', '0', '0', '0', '0', '0', '0', '0', '0',
};

static constexpr bool is_zero(float32_t x)
{
    return (std::bit_cast<uint32_t>(x) & 0x7fff'ffffU) == 0;
}

static constexpr bool is_zero(float64_t x)
{
    return (std::bit_cast<uint64_t>(x) & 0x7fff'ffff'ffff'ffffULL) == 0;
}

namespace asl
{

void jkj_dragonbox_to_decimal(
    float value,
    bool* is_negative,
    int* exponent,
    uint64_t* significand);

void jkj_dragonbox_to_decimal(
    double value,
    bool* is_negative,
    int* exponent,
    uint64_t* significand);

} // namespace asl

template<asl::is_floating_point T>
static void format_float(asl::Formatter& f, T value)
{
    if (asl::is_infinity(value))
    {
        if (value > 0)
        {
            f.write("Infinity"_sv);
        }
        else
        {
            f.write("-Infinity"_sv);
        }
        return;
    }

    if (is_zero(value))
    {
        f.write("0"_sv);
        return;
    }

    if (asl::is_nan(value))
    {
        f.write("NaN"_sv);
        return;
    }

    bool is_negative{};
    int exponent{};
    uint64_t significand{};

    asl::jkj_dragonbox_to_decimal(value, &is_negative, &exponent, &significand);

    if (is_negative) { f.write("-"); }

    char buffer[20];
    const asl::string_view digits = asl::format_uint64(significand, buffer);

    if (exponent >= 0)
    {
        f.write(digits);
        while (exponent > 0)
        {
            const isize_t to_write = asl::min(static_cast<isize_t>(exponent), kZeroCount);
            f.write(asl::string_view(static_cast<const char*>(kZeros), to_write));
            exponent -= static_cast<int>(to_write);
        }
    }
    else
    {
        if (digits.size() <= -exponent)
        {
            f.write("0.");
            exponent = -exponent - static_cast<int>(digits.size());
            while (exponent > 0)
            {
                const isize_t to_write = asl::min(static_cast<isize_t>(exponent), kZeroCount);
                f.write(asl::string_view(static_cast<const char*>(kZeros), to_write));
                exponent -= static_cast<int>(to_write);
            }
            f.write(digits);
        }
        else
        {
            f.write(digits.first(digits.size() + exponent));
            f.write(".");
            f.write(digits.last(-exponent));
        }
    }
}

void asl::AslFormat(Formatter& f, float32_t value)
{
    format_float(f, value);
}

void asl::AslFormat(Formatter& f, float64_t value)
{
    format_float(f, value);
}

