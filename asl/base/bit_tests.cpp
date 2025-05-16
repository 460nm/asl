// Copyright 2025 Steven Le Rouzic
//
// SPDX-License-Identifier: BSD-3-Clause

#include "asl/base/bit.hpp"

#include "asl/testing/testing.hpp"

ASL_TEST(has_single_bit)
{
    ASL_TEST_EXPECT(asl::has_single_bit(4U));
    ASL_TEST_EXPECT(asl::has_single_bit(1024U));
    ASL_TEST_EXPECT(asl::has_single_bit(0x8000'0000U));
    ASL_TEST_EXPECT(asl::has_single_bit(uint64_t{0x0000'8000'0000'0000ULL}));
    ASL_TEST_EXPECT(!asl::has_single_bit(0U));
    ASL_TEST_EXPECT(!asl::has_single_bit(3U));
    ASL_TEST_EXPECT(!asl::has_single_bit(341U));
}

ASL_TEST(popcount) // NOLINT(*-cognitive-complexity)
{
    ASL_TEST_EXPECT(asl::popcount(uint8_t{0}) == 0);
    ASL_TEST_EXPECT(asl::popcount(uint8_t{255}) == 8);
    ASL_TEST_EXPECT(asl::popcount(uint8_t{46}) == 4);
    ASL_TEST_EXPECT(asl::popcount(uint16_t{0}) == 0);
    ASL_TEST_EXPECT(asl::popcount(uint16_t{255}) == 8);
    ASL_TEST_EXPECT(asl::popcount(uint16_t{65535}) == 16);
    ASL_TEST_EXPECT(asl::popcount(uint16_t{46}) == 4);
    ASL_TEST_EXPECT(asl::popcount(uint32_t{0}) == 0);
    ASL_TEST_EXPECT(asl::popcount(uint32_t{255}) == 8);
    ASL_TEST_EXPECT(asl::popcount(uint32_t{65535}) == 16);
    ASL_TEST_EXPECT(asl::popcount(uint32_t{65536}) == 1);
    ASL_TEST_EXPECT(asl::popcount(uint32_t{46}) == 4);
    ASL_TEST_EXPECT(asl::popcount(uint32_t{0xffff'ffff}) == 32);
    ASL_TEST_EXPECT(asl::popcount(0x8421'1248'8421'1248) == 16);
    ASL_TEST_EXPECT(asl::popcount(0xffff'ffff'ffff'ffff) == 64);
}

ASL_TEST(countr_zero)
{
    ASL_TEST_EXPECT(asl::countr_zero(uint8_t{0}) == 8);
    ASL_TEST_EXPECT(asl::countr_zero(uint8_t{255}) == 0);
    ASL_TEST_EXPECT(asl::countr_zero(uint8_t{0b00011100}) == 2);
    ASL_TEST_EXPECT(asl::countr_zero(uint8_t{0b10101010}) == 1);
    ASL_TEST_EXPECT(asl::countr_zero(uint8_t{0b10101001}) == 0);
}

ASL_TEST(countr_one)
{
    ASL_TEST_EXPECT(asl::countr_one(uint8_t{0}) == 0);
    ASL_TEST_EXPECT(asl::countr_one(uint8_t{255}) == 8);
    ASL_TEST_EXPECT(asl::countr_one(uint8_t{0b00011100}) == 0);
    ASL_TEST_EXPECT(asl::countr_one(uint8_t{0b10101011}) == 2);
    ASL_TEST_EXPECT(asl::countr_one(uint8_t{0b10101001}) == 1);
}

ASL_TEST(countl_zero)
{
    ASL_TEST_EXPECT(asl::countl_zero(uint8_t{0}) == 8);
    ASL_TEST_EXPECT(asl::countl_zero(uint8_t{255}) == 0);
    ASL_TEST_EXPECT(asl::countl_zero(uint8_t{0b00011100}) == 3);
    ASL_TEST_EXPECT(asl::countl_zero(uint8_t{0b10101010}) == 0);
    ASL_TEST_EXPECT(asl::countl_zero(uint8_t{0b00001001}) == 4);
}

ASL_TEST(countl_one)
{
    ASL_TEST_EXPECT(asl::countl_one(uint8_t{0}) == 0);
    ASL_TEST_EXPECT(asl::countl_one(uint8_t{255}) == 8);
    ASL_TEST_EXPECT(asl::countl_one(uint8_t{0b00011100}) == 0);
    ASL_TEST_EXPECT(asl::countl_one(uint8_t{0b10101010}) == 1);
    ASL_TEST_EXPECT(asl::countl_one(uint8_t{0b11101001}) == 3);
}

ASL_TEST(rotl)
{
    ASL_TEST_EXPECT(asl::rotl(uint32_t{0x4000'0100}, 1) == 0x8000'0200);
    ASL_TEST_EXPECT(asl::rotl(uint32_t{0x4000'0100}, 3) == 0x0000'0802);
    ASL_TEST_EXPECT(asl::rotl(uint32_t{0x4000'0100}, 0) == 0x4000'0100);
    ASL_TEST_EXPECT(asl::rotl(uint32_t{0x4000'0100}, -1) == 0x2000'0080);
    ASL_TEST_EXPECT(asl::rotl(uint32_t{0x4000'0100}, -12) == 0x1004'0000);
}

ASL_TEST(rotr)
{
    ASL_TEST_EXPECT(asl::rotr(uint32_t{0x4000'0100}, -1) == 0x8000'0200);
    ASL_TEST_EXPECT(asl::rotr(uint32_t{0x4000'0100}, -3) == 0x0000'0802);
    ASL_TEST_EXPECT(asl::rotr(uint32_t{0x4000'0100}, 0) == 0x4000'0100);
    ASL_TEST_EXPECT(asl::rotr(uint32_t{0x4000'0100}, 1) == 0x2000'0080);
    ASL_TEST_EXPECT(asl::rotr(uint32_t{0x4000'0100}, 12) == 0x1004'0000);
}

ASL_TEST(byteswap)
{
    ASL_TEST_EXPECT(asl::byteswap(uint32_t{0x1234'5678}) == 0x7856'3412);
    ASL_TEST_EXPECT(asl::byteswap(uint16_t{0x1234}) == 0x3412);
    ASL_TEST_EXPECT(asl::byteswap(uint64_t{0x0123'4567'89ab'cdef}) == 0xefcd'ab89'6745'2301);
}

ASL_TEST(bit_ceil) // NOLINT(*-cognitive-complexity)
{
    ASL_TEST_EXPECT(asl::bit_ceil(uint8_t{0}) == 0);
    ASL_TEST_EXPECT(asl::bit_ceil(uint8_t{1}) == 1);
    ASL_TEST_EXPECT(asl::bit_ceil(uint8_t{2}) == 2);
    ASL_TEST_EXPECT(asl::bit_ceil(uint8_t{3}) == 4);
    ASL_TEST_EXPECT(asl::bit_ceil(uint8_t{4}) == 4);
    ASL_TEST_EXPECT(asl::bit_ceil(uint8_t{5}) == 8);
    ASL_TEST_EXPECT(asl::bit_ceil(uint8_t{6}) == 8);
    ASL_TEST_EXPECT(asl::bit_ceil(uint8_t{127}) == 128);
    ASL_TEST_EXPECT(asl::bit_ceil(uint8_t{128}) == 128);
    ASL_TEST_EXPECT(asl::bit_ceil(uint8_t{254}) == 0);
    ASL_TEST_EXPECT(asl::bit_ceil(uint8_t{255}) == 0);

    ASL_TEST_EXPECT(asl::bit_ceil(uint16_t{0}) == 0);
    ASL_TEST_EXPECT(asl::bit_ceil(uint16_t{1}) == 1);
    ASL_TEST_EXPECT(asl::bit_ceil(uint16_t{2}) == 2);
    ASL_TEST_EXPECT(asl::bit_ceil(uint16_t{3}) == 4);
    ASL_TEST_EXPECT(asl::bit_ceil(uint16_t{4}) == 4);
    ASL_TEST_EXPECT(asl::bit_ceil(uint16_t{5}) == 8);
    ASL_TEST_EXPECT(asl::bit_ceil(uint16_t{6}) == 8);
    ASL_TEST_EXPECT(asl::bit_ceil(uint16_t{32000}) == 32768);
    ASL_TEST_EXPECT(asl::bit_ceil(uint16_t{32768}) == 32768);
    ASL_TEST_EXPECT(asl::bit_ceil(uint16_t{35000}) == 0);
    ASL_TEST_EXPECT(asl::bit_ceil(uint16_t{65535}) == 0);
}

ASL_TEST(bit_floor) // NOLINT(*-cognitive-complexity)
{
    ASL_TEST_EXPECT(asl::bit_floor(uint8_t{0}) == 0);
    ASL_TEST_EXPECT(asl::bit_floor(uint8_t{1}) == 1);
    ASL_TEST_EXPECT(asl::bit_floor(uint8_t{2}) == 2);
    ASL_TEST_EXPECT(asl::bit_floor(uint8_t{3}) == 2);
    ASL_TEST_EXPECT(asl::bit_floor(uint8_t{4}) == 4);
    ASL_TEST_EXPECT(asl::bit_floor(uint8_t{5}) == 4);
    ASL_TEST_EXPECT(asl::bit_floor(uint8_t{6}) == 4);
    ASL_TEST_EXPECT(asl::bit_floor(uint8_t{127}) == 64);
    ASL_TEST_EXPECT(asl::bit_floor(uint8_t{128}) == 128);
    ASL_TEST_EXPECT(asl::bit_floor(uint8_t{254}) == 128);
    ASL_TEST_EXPECT(asl::bit_floor(uint8_t{255}) == 128);

    ASL_TEST_EXPECT(asl::bit_floor(uint16_t{0}) == 0);
    ASL_TEST_EXPECT(asl::bit_floor(uint16_t{1}) == 1);
    ASL_TEST_EXPECT(asl::bit_floor(uint16_t{2}) == 2);
    ASL_TEST_EXPECT(asl::bit_floor(uint16_t{3}) == 2);
    ASL_TEST_EXPECT(asl::bit_floor(uint16_t{4}) == 4);
    ASL_TEST_EXPECT(asl::bit_floor(uint16_t{5}) == 4);
    ASL_TEST_EXPECT(asl::bit_floor(uint16_t{6}) == 4);
    ASL_TEST_EXPECT(asl::bit_floor(uint16_t{32000}) == 16384);
    ASL_TEST_EXPECT(asl::bit_floor(uint16_t{32768}) == 32768);
    ASL_TEST_EXPECT(asl::bit_floor(uint16_t{35000}) == 32768);
    ASL_TEST_EXPECT(asl::bit_floor(uint16_t{65535}) == 32768);
}

ASL_TEST(bit_width) // NOLINT(*-cognitive-complexity)
{
    ASL_TEST_EXPECT(asl::bit_width(uint8_t{0}) == 0);
    ASL_TEST_EXPECT(asl::bit_width(uint8_t{1}) == 1);
    ASL_TEST_EXPECT(asl::bit_width(uint8_t{2}) == 2);
    ASL_TEST_EXPECT(asl::bit_width(uint8_t{3}) == 2);
    ASL_TEST_EXPECT(asl::bit_width(uint8_t{4}) == 3);
    ASL_TEST_EXPECT(asl::bit_width(uint8_t{5}) == 3);
    ASL_TEST_EXPECT(asl::bit_width(uint8_t{6}) == 3);

    ASL_TEST_EXPECT(asl::bit_width(uint16_t{0}) == 0);
    ASL_TEST_EXPECT(asl::bit_width(uint16_t{1}) == 1);
    ASL_TEST_EXPECT(asl::bit_width(uint16_t{2}) == 2);
    ASL_TEST_EXPECT(asl::bit_width(uint16_t{3}) == 2);
    ASL_TEST_EXPECT(asl::bit_width(uint16_t{4}) == 3);
    ASL_TEST_EXPECT(asl::bit_width(uint16_t{5}) == 3);
    ASL_TEST_EXPECT(asl::bit_width(uint16_t{6}) == 3);
    ASL_TEST_EXPECT(asl::bit_width(uint16_t{65535}) == 16);

    ASL_TEST_EXPECT(asl::bit_width(uint32_t{0}) == 0);
    ASL_TEST_EXPECT(asl::bit_width(uint32_t{1}) == 1);
    ASL_TEST_EXPECT(asl::bit_width(uint32_t{2}) == 2);
    ASL_TEST_EXPECT(asl::bit_width(uint32_t{3}) == 2);
    ASL_TEST_EXPECT(asl::bit_width(uint32_t{4}) == 3);
    ASL_TEST_EXPECT(asl::bit_width(uint32_t{5}) == 3);
    ASL_TEST_EXPECT(asl::bit_width(uint32_t{6}) == 3);
    ASL_TEST_EXPECT(asl::bit_width(uint32_t{65535}) == 16);
    ASL_TEST_EXPECT(asl::bit_width(uint32_t{65536}) == 17);
    ASL_TEST_EXPECT(asl::bit_width(uint32_t{0xffff'ffff}) == 32);

    ASL_TEST_EXPECT(asl::bit_width(uint64_t{0}) == 0);
    ASL_TEST_EXPECT(asl::bit_width(uint64_t{1}) == 1);
    ASL_TEST_EXPECT(asl::bit_width(uint64_t{2}) == 2);
    ASL_TEST_EXPECT(asl::bit_width(uint64_t{3}) == 2);
    ASL_TEST_EXPECT(asl::bit_width(uint64_t{4}) == 3);
    ASL_TEST_EXPECT(asl::bit_width(uint64_t{5}) == 3);
    ASL_TEST_EXPECT(asl::bit_width(uint64_t{6}) == 3);
    ASL_TEST_EXPECT(asl::bit_width(uint64_t{65535}) == 16);
    ASL_TEST_EXPECT(asl::bit_width(uint64_t{65536}) == 17);
    ASL_TEST_EXPECT(asl::bit_width(uint64_t{0xffff'ffff}) == 32);
}
