// Copyright 2025 Steven Le Rouzic
//
// SPDX-License-Identifier: BSD-3-Clause

#include "asl/formatting/format.hpp"
#include "asl/base/float.hpp"

#define JKJ_STD_REPLACEMENT_NAMESPACE_DEFINED 0
#define JKJ_STATIC_DATA_SECTION_DEFINED 0
#include <dragonbox.h>

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

static constexpr bool is_zero(float x)
{
    return (asl::bit_cast<uint32_t>(x) & 0x7fff'ffffU) == 0;
}

static constexpr bool is_zero(double x)
{
    return (asl::bit_cast<uint64_t>(x) & 0x7fff'ffff'ffff'ffffULL) == 0;
}

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

    auto decimal = jkj::dragonbox::to_decimal(value);

    if (decimal.is_negative) { f.write("-"); }

    char buffer[20];
    const asl::string_view digits = asl::format_uint64(decimal.significand, buffer);

    if (decimal.exponent >= 0)
    {
        f.write(digits);
        while (decimal.exponent > 0)
        {
            const isize_t to_write = asl::min(static_cast<isize_t>(decimal.exponent), kZeroCount);
            f.write(asl::string_view(static_cast<const char*>(kZeros), to_write));
            decimal.exponent -= to_write;
        }
    }
    else
    {
        if (digits.size() <= -decimal.exponent)
        {
            f.write("0.");
            decimal.exponent = -decimal.exponent - static_cast<int>(digits.size());
            while (decimal.exponent > 0)
            {
                const isize_t to_write = asl::min(static_cast<isize_t>(decimal.exponent), kZeroCount);
                f.write(asl::string_view(static_cast<const char*>(kZeros), to_write));
                decimal.exponent -= to_write;
            }
            f.write(digits);
        }
        else
        {
            f.write(digits.first(digits.size() + decimal.exponent));
            f.write(".");
            f.write(digits.last(-decimal.exponent));
        }
    }
}

void asl::AslFormat(Formatter& f, float value)
{
    format_float(f, value);
}

void asl::AslFormat(Formatter& f, double value)
{
    format_float(f, value);
}
