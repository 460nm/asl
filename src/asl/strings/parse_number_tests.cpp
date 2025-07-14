// Copyright 2025 Steven Le Rouzic
//
// SPDX-License-Identifier: BSD-3-Clause

#include "asl/strings/parse_number.hpp"
#include "asl/base/numeric.hpp"
#include "asl/testing/testing.hpp"

ASL_TEST(parse_float_error)
{
    const asl::string_view sv = "this is not a number lmao";
    auto res = asl::parse_float32(sv);
    ASL_TEST_EXPECT(!res.ok());
}

ASL_TEST(parse_float_empty)
{
    const asl::string_view sv = "";
    auto res = asl::parse_float32(sv);
    ASL_TEST_EXPECT(!res.ok());
}

ASL_TEST(parse_float_simple)
{
    const asl::string_view sv = "3.1415";
    auto res = asl::parse_float32(sv);
    ASL_TEST_EXPECT(res.ok());
    ASL_TEST_EXPECT(asl::are_nearly_equal(res.value().value, 3.1415F));
    ASL_TEST_EXPECT(res.value().remaining.size() == 0);
}

ASL_TEST(parse_float_integer)
{
    const asl::string_view sv = "31415";
    auto res = asl::parse_float32(sv);
    ASL_TEST_EXPECT(res.ok());
    ASL_TEST_EXPECT(res.value().value == 31415.0F);
    ASL_TEST_EXPECT(res.value().remaining.size() == 0);
}

ASL_TEST(parse_float_scientific)
{
    const asl::string_view sv = "314.15e-2";
    auto res = asl::parse_float32(sv);
    ASL_TEST_EXPECT(res.ok());
    ASL_TEST_EXPECT(asl::are_nearly_equal(res.value().value, 3.1415F));
    ASL_TEST_EXPECT(res.value().remaining.size() == 0);
}

ASL_TEST(parse_float_suffix)
{
    const asl::string_view sv = "3.1415 yoyoyo";
    auto res = asl::parse_float32(sv);
    ASL_TEST_EXPECT(res.ok());
    ASL_TEST_EXPECT(asl::are_nearly_equal(res.value().value, 3.1415F));
    ASL_TEST_EXPECT(res.value().remaining == " yoyoyo");
}

ASL_TEST(parse_int)
{
    const asl::string_view sv = "926473";
    auto res = asl::parse_uint32(sv);
    ASL_TEST_EXPECT(res.ok());
    ASL_TEST_EXPECT(res.value().value == 926473);
    ASL_TEST_EXPECT(res.value().remaining.is_empty());
}

ASL_TEST(parse_int_negative)
{
    const asl::string_view sv = "-926473";
    auto res = asl::parse_int32(sv);
    ASL_TEST_EXPECT(res.ok());
    ASL_TEST_EXPECT(res.value().value == -926473);
    ASL_TEST_EXPECT(res.value().remaining.is_empty());
}

ASL_TEST(parse_int_suffix)
{
    const asl::string_view sv = "926473 what's this then";
    auto res = asl::parse_uint32(sv);
    ASL_TEST_EXPECT(res.ok());
    ASL_TEST_EXPECT(res.value().value == 926473);
    ASL_TEST_EXPECT(res.value().remaining == " what's this then");
}

ASL_TEST(parse_uint_with_minus)
{
    const asl::string_view sv = "-926473";
    auto res = asl::parse_uint32(sv);
    ASL_TEST_EXPECT(!res.ok());
}

ASL_TEST(parse_int_with_only_minus)
{
    const asl::string_view sv = "-@";
    auto res = asl::parse_int32(sv);
    ASL_TEST_EXPECT(!res.ok());
}

ASL_TEST(parse_uint_invalid)
{
    const asl::string_view sv = "abcd";
    auto res = asl::parse_uint32(sv);
    ASL_TEST_EXPECT(!res.ok());
}

ASL_TEST(parse_uint_empty)
{
    const asl::string_view sv = "";
    auto res = asl::parse_uint32(sv);
    ASL_TEST_EXPECT(!res.ok());
}

ASL_TEST(parse_uint_overflow)
{
    ASL_TEST_EXPECT(!asl::parse_uint16("80000").ok());
    ASL_TEST_EXPECT(!asl::parse_uint16("65536").ok());
}

ASL_TEST(parse_uint16_max)
{
    const asl::string_view sv = "65535";
    auto res = asl::parse_uint16(sv);
    ASL_TEST_EXPECT(res.ok());
    ASL_TEST_EXPECT(res.value().value == 65535);
    ASL_TEST_EXPECT(res.value().remaining.is_empty());
}

ASL_TEST(parse_uint16_zero)
{
    const asl::string_view sv = "0";
    auto res = asl::parse_uint16(sv);
    ASL_TEST_EXPECT(res.ok());
    ASL_TEST_EXPECT(res.value().value == 0);
    ASL_TEST_EXPECT(res.value().remaining.is_empty());
}

ASL_TEST(parse_uint16_zeros)
{
    const asl::string_view sv = "00000";
    auto res = asl::parse_uint16(sv);
    ASL_TEST_EXPECT(res.ok());
    ASL_TEST_EXPECT(res.value().value == 0);
    ASL_TEST_EXPECT(res.value().remaining.is_empty());
}

ASL_TEST(parse_int_overflow)
{
    ASL_TEST_EXPECT(!asl::parse_int16("80000").ok());
    ASL_TEST_EXPECT(!asl::parse_int16("40000").ok());
    ASL_TEST_EXPECT(!asl::parse_int16("32768").ok());
    ASL_TEST_EXPECT(!asl::parse_int16("-80000").ok());
    ASL_TEST_EXPECT(!asl::parse_int16("-40000").ok());
    ASL_TEST_EXPECT(!asl::parse_int16("-32769").ok());
}

ASL_TEST(parse_int16_max)
{
    const asl::string_view sv = "32767";
    auto res = asl::parse_int16(sv);
    ASL_TEST_EXPECT(res.ok());
    ASL_TEST_EXPECT(res.value().value == 32767);
    ASL_TEST_EXPECT(res.value().remaining.is_empty());
}

ASL_TEST(parse_int16_min)
{
    const asl::string_view sv = "-32768";
    auto res = asl::parse_int16(sv);
    ASL_TEST_EXPECT(res.ok());
    ASL_TEST_EXPECT(res.value().value == -32768);
    ASL_TEST_EXPECT(res.value().remaining.is_empty());
}

ASL_TEST(parse_hex)
{
    const asl::string_view sv = "1000a";
    auto res = asl::parse_uint32(sv, 16);
    ASL_TEST_EXPECT(res.ok());
    ASL_TEST_EXPECT(res.value().value == 65546);
    ASL_TEST_EXPECT(res.value().remaining.is_empty());
}

ASL_TEST(parse_bin)
{
    const asl::string_view sv = "101010";
    auto res = asl::parse_uint32(sv, 2);
    ASL_TEST_EXPECT(res.ok());
    ASL_TEST_EXPECT(res.value().value == 42);
    ASL_TEST_EXPECT(res.value().remaining.is_empty());
}

ASL_TEST(parse_oct)
{
    const asl::string_view sv = "644";
    auto res = asl::parse_uint32(sv, 8);
    ASL_TEST_EXPECT(res.ok());
    ASL_TEST_EXPECT(res.value().value == 6 * 64 + 4 * 8 + 4);
    ASL_TEST_EXPECT(res.value().remaining.is_empty());
}

