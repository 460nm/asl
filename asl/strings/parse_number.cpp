// Copyright 2025 Steven Le Rouzic
//
// SPDX-License-Identifier: BSD-3-Clause

#include "asl/strings/parse_number.hpp"

namespace asl
{

bool parse_float_impl(const char** begin, const char* end, float*);
bool parse_double_impl(const char** begin, const char* end, double*);

} // namespace asl

asl::status_or<asl::parse_number_result<float>> asl::parse_float(asl::string_view sv)
{
    const auto* begin = sv.data();
    // NOLINTNEXTLINE(*-pointer-arithmetic)
    const auto* end = begin + sv.size();
    if (float value{}; parse_float_impl(&begin, end, &value))
    {
        return parse_number_result<float>{
            .value     = value,
            .remaining = string_view{begin, end},
        };
    }
    return invalid_argument_error();
}

asl::status_or<asl::parse_number_result<double>> asl::parse_double(asl::string_view sv)
{
    const auto* begin = sv.data();
    // NOLINTNEXTLINE(*-pointer-arithmetic)
    const auto* end = begin + sv.size();
    if (float value{}; parse_float_impl(&begin, end, &value))
    {
        return parse_number_result<double>{
            .value     = value,
            .remaining = string_view{begin, end},
        };
    }
    return invalid_argument_error();
}

namespace
{

constexpr int8_t kBase16Table[256] = {
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
     0,  1,  2,  3,  4,  5,  6,  7,  8,  9, -1, -1, -1, -1, -1, -1,
    -1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
};

// @Todo Maybe monomorph this for common bases (2, 16, 10)?

template<typename T>
asl::status_or<asl::parse_number_result<T>> parse_integer(asl::string_view sv, int base)
{
    ASL_ASSERT(base >= 2 && base <= 16);

    if (sv.is_empty()) { return asl::invalid_argument_error(); }

    T value = 0;
    bool is_negative = false;

    if (asl::is_signed_integer<T> && sv[0] == '-')
    {
        is_negative = true;
        sv = sv.substr(1);
    }

    isize_t cursor = 0;
    while (cursor < sv.size())
    {
        // NOLINTNEXTLINE(*-array-index)
        int8_t digit = kBase16Table[static_cast<uint8_t>(sv[cursor])];
        if (digit < 0 || digit >= base) { break; }

        if (__builtin_mul_overflow(value, static_cast<T>(base), &value))
        {
            return asl::invalid_argument_error("overflow");
        }

        if (asl::is_signed_integer<T> && is_negative)
        {
            digit = static_cast<T>(-digit);
        }

        if (__builtin_add_overflow(value, static_cast<T>(digit), &value))
        {
            return asl::invalid_argument_error("overflow");
        }

        cursor += 1;
    }

    if (cursor == 0)
    {
        return asl::invalid_argument_error();
    }

    return asl::parse_number_result<T>{
        .value     = value,
        .remaining = sv.substr(cursor),
    };
}

} // anonymous namespace

asl::status_or<asl::parse_number_result<uint8_t>> asl::parse_uint8(string_view sv, int base)
{
    return parse_integer<uint8_t>(sv, base);
}

asl::status_or<asl::parse_number_result<uint16_t>> asl::parse_uint16(string_view sv, int base)
{
    return parse_integer<uint16_t>(sv, base);
}

asl::status_or<asl::parse_number_result<uint32_t>> asl::parse_uint32(string_view sv, int base)
{
    return parse_integer<uint32_t>(sv, base);
}

asl::status_or<asl::parse_number_result<uint64_t>> asl::parse_uint64(string_view sv, int base)
{
    return parse_integer<uint64_t>(sv, base);
}

asl::status_or<asl::parse_number_result<int8_t>> asl::parse_int8(string_view sv, int base)
{
    return parse_integer<int8_t>(sv, base);
}

asl::status_or<asl::parse_number_result<int16_t>> asl::parse_int16(string_view sv, int base)
{
    return parse_integer<int16_t>(sv, base);
}

asl::status_or<asl::parse_number_result<int32_t>> asl::parse_int32(string_view sv, int base)
{
    return parse_integer<int32_t>(sv, base);
}

asl::status_or<asl::parse_number_result<int64_t>> asl::parse_int64(string_view sv, int base)
{
    return parse_integer<int64_t>(sv, base);
}

