// Copyright 2025 Steven Le Rouzic
//
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#include "asl/base/meta.hpp"

namespace std
{

enum class byte : unsigned char {};

// [support.types.byteops]

template<asl::is_integral IntType>
constexpr byte& operator<<=(byte& b, IntType shift)
{
    return b = b << shift;
}

template<asl::is_integral IntType>
constexpr byte operator<<(byte b, IntType shift)
{
    return static_cast<byte>(static_cast<unsigned int>(b) << shift);
}

template<asl::is_integral IntType>
constexpr byte& operator>>=(byte& b, IntType shift)
{
    return b = b >> shift;
}

template<asl::is_integral IntType>
constexpr byte operator>>(byte b, IntType shift)
{
    return static_cast<byte>(static_cast<unsigned int>(b) >> shift);
}

constexpr byte operator|(byte l, byte r)
{
    return static_cast<byte>(static_cast<unsigned int>(l) | static_cast<unsigned int>(r));
}

constexpr byte& operator|=(byte& l, byte r)
{
    return l = l | r;
}

constexpr byte operator&(byte l, byte r)
{
    return static_cast<byte>(static_cast<unsigned int>(l) & static_cast<unsigned int>(r));
}

constexpr byte& operator&=(byte& l, byte r)
{
    return l = l & r;
}

constexpr byte operator^(byte l, byte r)
{
    return static_cast<byte>(static_cast<unsigned int>(l) ^ static_cast<unsigned int>(r));
}

constexpr byte& operator^=(byte& l, byte r)
{
    return l = l ^ r;
}

constexpr byte operator~(byte b)
{
    return static_cast<byte>(~static_cast<unsigned int>(b));
}

template<asl::is_integral IntType>
constexpr IntType to_integer(byte b)
{
    return static_cast<IntType>(b);
}

} // namespace std
