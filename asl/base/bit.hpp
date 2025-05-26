// Copyright 2025 Steven Le Rouzic
//
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#include "asl/base/integers.hpp"
#include "asl/base/meta.hpp"

namespace asl
{

constexpr bool has_single_bit(is_unsigned_integer auto x)
{
    return x != 0 && ((x - 1) & x) == 0;
}

constexpr int popcount(uint8_t v)
{
    v = v - ((v >> 1) & 0x55);
    v = (v & 0x33) + ((v >> 2) & 0x33);
    return (v + (v >> 4)) & 0x0F;
}

constexpr int popcount(uint16_t v)
{
    v = v - ((v >> 1) & 0x5555);
    v = (v & 0x3333) + ((v >> 2) & 0x3333);
    return static_cast<uint16_t>((v + (v >> 4) & 0x0F0F) * uint16_t{0x0101}) >> 8;
}

constexpr int popcount(uint32_t v)
{
    v = v - ((v >> 1) & 0x5555'5555);
    v = (v & 0x3333'3333) + ((v >> 2) & 0x3333'3333);
    return static_cast<int>(((v + (v >> 4) & 0x0F0F'0F0F) * 0x0101'0101) >> 24);
}

constexpr int popcount(uint64_t v)
{
    v = v - ((v >> 1) & 0x5555'5555'5555'5555);
    v = (v & 0x3333'3333'3333'3333) + ((v >> 2) & 0x3333'3333'3333'3333);
    return static_cast<int>(((v + (v >> 4) & 0x0F0F'0F0F'0F0F'0F0F) * 0x0101'0101'0101'0101) >> 56);
}

constexpr uint8_t propagate_right_one(uint8_t v)
{
    v = v | (v >> 1);
    v = v | (v >> 2);
    v = v | (v >> 4);
    return v;
}

constexpr uint16_t propagate_right_one(uint16_t v)
{
    v = v | (v >> 1);
    v = v | (v >> 2);
    v = v | (v >> 4);
    v = v | (v >> 8);
    return v;
}

constexpr uint32_t propagate_right_one(uint32_t v)
{
    v = v | (v >> 1);
    v = v | (v >> 2);
    v = v | (v >> 4);
    v = v | (v >> 8);
    v = v | (v >> 16);
    return v;
}

constexpr uint64_t propagate_right_one(uint64_t v)
{
    v = v | (v >> 1);
    v = v | (v >> 2);
    v = v | (v >> 4);
    v = v | (v >> 8);
    v = v | (v >> 16);
    v = v | (v >> 32);
    return v;
}

constexpr int countr_zero(is_unsigned_integer auto v)
{
    v = ~v & (v - 1);
    return popcount(v);
}

constexpr int countr_one(is_unsigned_integer auto v)
{
    v = v & (~v - 1);
    return popcount(v);
}

constexpr int countl_zero(is_unsigned_integer auto v)
{
    v = ~propagate_right_one(v);
    return popcount(v);
}

constexpr int countl_one(is_unsigned_integer auto v)
{
    v = ~v;
    v = ~propagate_right_one(v);
    return popcount(v);
}

template<is_unsigned_integer T>
constexpr T rotr(T v, int s);

template<is_unsigned_integer T>
constexpr T rotl(T v, int s) // NOLINT(*-no-recursion)
{
    static constexpr int N = sizeof(decltype(v)) * 8;
    s = s % N;
    return (s >= 0) ? (v << s) | (v >> ((N - s) % N)) : rotr(v, -s);
}

template<is_unsigned_integer T>
constexpr T rotr(T v, int s) // NOLINT(*-no-recursion)
{
    static constexpr int N = sizeof(decltype(v)) * 8;
    s = s % N;
    return (s >= 0) ? (v >> s) | (v << ((N - s) % N)) : rotl(v, -s);
}

constexpr uint16_t byteswap(uint16_t v)
{
    return static_cast<uint16_t>((v << 8U) | (v >> 8U));
}

constexpr uint32_t byteswap(uint32_t v)
{
    return rotr(v & 0x00ff'00ff, 8) | rotl(v & 0xff00'ff00, 8);
}

constexpr uint64_t byteswap(uint64_t v)
{
    return (uint64_t{byteswap(static_cast<uint32_t>(v))} << 32)
        | uint64_t{byteswap(static_cast<uint32_t>(v >> 32))};
}

constexpr auto bit_ceil(is_unsigned_integer auto v)
{
    v -= 1;
    v = propagate_right_one(v);
    v += 1;
    return v;
}

constexpr auto bit_floor(is_unsigned_integer auto v)
{
    v = propagate_right_one(v);
    v = v - (v >> 1);
    return v;
}

constexpr int bit_width(is_unsigned_integer auto v)
{
    static constexpr int N = sizeof(decltype(v)) * 8;
    return N - countl_zero(v);
}

} // namespace asl

