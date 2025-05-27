// Copyright 2025 Steven Le Rouzic
//
// SPDX-License-Identifier: BSD-3-Clause

#include "asl/testing/testing.hpp"
#include "asl/handle_pool/index_pool.hpp"

enum Flags: uint8_t {
    kFlag0 = 0,
    kFlag1 = 1,
    kFlag2 = 2,
};

using Cfg1 = asl::index_pool_config<4, 3>;
static_assert(!Cfg1::kHasUser);
static_assert(Cfg1::kUserBits == 0);
static_assert(asl::same_as<Cfg1::HandleType, uint8_t>);
static_assert(Cfg1::kValidMask == uint8_t{0x80});
static_assert(Cfg1::kIndexMask == uint8_t{0x0f});
static_assert(Cfg1::kGenMask   == uint8_t{0x70});
static_assert(Cfg1::kGenShift  == 4);

using Cfg2 = asl::index_pool_config<5, 5, Flags>;
static_assert(Cfg2::kHasUser);
static_assert(Cfg2::kUserBits == 8);
static_assert(asl::same_as<Cfg2::PrimitiveUserType, uint8_t>);
static_assert(asl::same_as<Cfg2::HandleType, uint32_t>);
static_assert(Cfg2::kValidMask == uint32_t{0x8000'0000});
static_assert(Cfg2::kIndexMask == uint32_t{0x0000'001f});
static_assert(Cfg2::kGenMask   == uint32_t{0x0000'03e0});
static_assert(Cfg2::kUserMask  == uint32_t{0x0003'fc00});
static_assert(Cfg2::kGenShift  == 5);
static_assert(Cfg2::kUserShift == 10);

using Cfg3 = asl::index_pool_config<5, 6, Flags, 4>;
static_assert(Cfg3::kHasUser);
static_assert(Cfg3::kUserBits == 4);
static_assert(asl::same_as<Cfg3::PrimitiveUserType, uint8_t>);
static_assert(asl::same_as<Cfg3::HandleType, uint16_t>);
static_assert(Cfg3::kValidMask == uint16_t{0x8000});
static_assert(Cfg3::kIndexMask == uint16_t{0x001f});
static_assert(Cfg3::kGenMask   == uint16_t{0x07e0});
static_assert(Cfg3::kUserMask  == uint16_t{0x7800});
static_assert(Cfg3::kGenShift  == 5);
static_assert(Cfg3::kUserShift == 11);

ASL_TEST(test)
{
}
