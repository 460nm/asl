// Copyright 2025 Steven Le Rouzic
//
// SPDX-License-Identifier: BSD-3-Clause

#include "asl/testing/testing.hpp"
#include "asl/handle_pool/index_pool.hpp"
#include "asl/hashing/hash.hpp"

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
static_assert(Cfg1::kMaxGen    == 7);
static_assert(Cfg1::kMaxIndex  == 15);

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
static_assert(Cfg2::kMaxGen    == 31);
static_assert(Cfg2::kMaxIndex  == 31);

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
static_assert(Cfg3::kMaxGen    == 63);
static_assert(Cfg3::kMaxIndex  == 31);

static_assert(asl::is_default_constructible<asl::index_pool_handle<5, 5, uint8_t>>);
static_assert(asl::is_trivially_copy_constructible<asl::index_pool_handle<5, 5, uint8_t>>);
static_assert(asl::is_trivially_move_constructible<asl::index_pool_handle<5, 5, uint8_t>>);
static_assert(asl::is_trivially_copy_assignable<asl::index_pool_handle<5, 5, uint8_t>>);
static_assert(asl::is_trivially_move_assignable<asl::index_pool_handle<5, 5, uint8_t>>);
static_assert(asl::is_trivially_destructible<asl::index_pool_handle<5, 5, uint8_t>>);

static_assert(asl::hashable<asl::index_pool_handle<5, 5, uint8_t>>);
static_assert(asl::has_niche<asl::index_pool_handle<5, 5, uint8_t>>);

ASL_TEST(default_is_invalid)
{
    const asl::index_pool_handle<5, 5, uint8_t> idx;
    ASL_TEST_EXPECT(idx.is_null());
}

ASL_TEST(niche_is_invalid)
{
    const asl::index_pool_handle<5, 5, uint8_t> idx{asl::niche_t{}};
    ASL_TEST_EXPECT(idx.is_null());
}

ASL_TEST(construct)
{
    const asl::index_pool_handle<5, 5> idx(9, 11);
    ASL_TEST_EXPECT(!idx.is_null());
    ASL_TEST_EXPECT(idx.index() == 9);
    ASL_TEST_EXPECT(idx.gen() == 11);
}

ASL_TEST(construct_user)
{
    const asl::index_pool_handle<5, 5, Flags, 4> idx(9, 11, kFlag2);
    ASL_TEST_EXPECT(!idx.is_null());
    ASL_TEST_EXPECT(idx.index() == 9);
    ASL_TEST_EXPECT(idx.gen() == 11);
    ASL_TEST_EXPECT(idx.user() == kFlag2);
    static_assert(asl::same_as<Flags, decltype(idx.user())>);
}

ASL_TEST(compare) // NOLINT
{
    const asl::index_pool_handle<5, 5, Flags, 4> idx_default;
    const asl::index_pool_handle<5, 5, Flags, 4> idx0;
    const asl::index_pool_handle<5, 5, Flags, 4> idx1(9, 11, kFlag2);
    const asl::index_pool_handle<5, 5, Flags, 4> idx2(9, 11, kFlag1);
    const asl::index_pool_handle<5, 5, Flags, 4> idx3(9, 11, kFlag1);
    const asl::index_pool_handle<5, 5, Flags, 4> idx4(9, 10, kFlag2);
    const asl::index_pool_handle<5, 5, Flags, 4> idx5(8, 11, kFlag2);

    ASL_TEST_EXPECT(idx0 == idx_default);

    ASL_TEST_EXPECT(idx0 != idx1);
    ASL_TEST_EXPECT(idx0 != idx2);
    ASL_TEST_EXPECT(idx0 != idx3);
    ASL_TEST_EXPECT(idx0 != idx4);
    ASL_TEST_EXPECT(idx0 != idx5);

    ASL_TEST_EXPECT(idx1 != idx2);
    ASL_TEST_EXPECT(idx1 != idx3);
    ASL_TEST_EXPECT(idx1 != idx4);
    ASL_TEST_EXPECT(idx1 != idx5);

    ASL_TEST_EXPECT(idx2 == idx3);
    ASL_TEST_EXPECT(idx2 != idx4);
    ASL_TEST_EXPECT(idx2 != idx5);

    ASL_TEST_EXPECT(idx3 != idx4);
    ASL_TEST_EXPECT(idx3 != idx5);

    ASL_TEST_EXPECT(idx4 != idx5);
}

ASL_TEST(hashing) // NOLINT
{
    const asl::index_pool_handle<4, 4> idx0(asl::niche_t{});
    const asl::index_pool_handle<4, 4> idx1{};
    const asl::index_pool_handle<4, 4> idx2(1, 1);
    const asl::index_pool_handle<4, 4> idx3(1, 1);
    const asl::index_pool_handle<4, 4> idx4(2, 1);
    const asl::index_pool_handle<4, 4> idx5(1, 2);

    ASL_TEST_EXPECT(asl::hash_value(idx0) != asl::hash_value(idx1));
    ASL_TEST_EXPECT(asl::hash_value(idx0) != asl::hash_value(idx2));
    ASL_TEST_EXPECT(asl::hash_value(idx0) != asl::hash_value(idx3));
    ASL_TEST_EXPECT(asl::hash_value(idx0) != asl::hash_value(idx4));
    ASL_TEST_EXPECT(asl::hash_value(idx0) != asl::hash_value(idx5));

    ASL_TEST_EXPECT(asl::hash_value(idx1) != asl::hash_value(idx2));
    ASL_TEST_EXPECT(asl::hash_value(idx1) != asl::hash_value(idx3));
    ASL_TEST_EXPECT(asl::hash_value(idx1) != asl::hash_value(idx4));
    ASL_TEST_EXPECT(asl::hash_value(idx1) != asl::hash_value(idx5));

    ASL_TEST_EXPECT(asl::hash_value(idx2) == asl::hash_value(idx3));
    ASL_TEST_EXPECT(asl::hash_value(idx2) != asl::hash_value(idx4));
    ASL_TEST_EXPECT(asl::hash_value(idx2) != asl::hash_value(idx5));

    ASL_TEST_EXPECT(asl::hash_value(idx3) != asl::hash_value(idx4));
    ASL_TEST_EXPECT(asl::hash_value(idx3) != asl::hash_value(idx5));

    ASL_TEST_EXPECT(asl::hash_value(idx4) != asl::hash_value(idx5));
}

ASL_TEST(hashing_in_option) // NOLINT
{
    const asl::option<asl::index_pool_handle<4, 4>> idx0;
    const asl::option<asl::index_pool_handle<4, 4>> idx1{asl::index_pool_handle<4, 4>()};
    const asl::option<asl::index_pool_handle<4, 4>> idx2{asl::index_pool_handle<4, 4>(1, 1)};
    const asl::option<asl::index_pool_handle<4, 4>> idx3{asl::index_pool_handle<4, 4>(1, 1)};
    const asl::option<asl::index_pool_handle<4, 4>> idx4{asl::index_pool_handle<4, 4>(2, 1)};
    const asl::option<asl::index_pool_handle<4, 4>> idx5{asl::index_pool_handle<4, 4>(1, 2)};

    ASL_TEST_EXPECT(asl::hash_value(idx0) != asl::hash_value(idx1));
    ASL_TEST_EXPECT(asl::hash_value(idx0) != asl::hash_value(idx2));
    ASL_TEST_EXPECT(asl::hash_value(idx0) != asl::hash_value(idx3));
    ASL_TEST_EXPECT(asl::hash_value(idx0) != asl::hash_value(idx4));
    ASL_TEST_EXPECT(asl::hash_value(idx0) != asl::hash_value(idx5));

    ASL_TEST_EXPECT(asl::hash_value(idx1) != asl::hash_value(idx2));
    ASL_TEST_EXPECT(asl::hash_value(idx1) != asl::hash_value(idx3));
    ASL_TEST_EXPECT(asl::hash_value(idx1) != asl::hash_value(idx4));
    ASL_TEST_EXPECT(asl::hash_value(idx1) != asl::hash_value(idx5));

    ASL_TEST_EXPECT(asl::hash_value(idx2) == asl::hash_value(idx3));
    ASL_TEST_EXPECT(asl::hash_value(idx2) != asl::hash_value(idx4));
    ASL_TEST_EXPECT(asl::hash_value(idx2) != asl::hash_value(idx5));

    ASL_TEST_EXPECT(asl::hash_value(idx3) != asl::hash_value(idx4));
    ASL_TEST_EXPECT(asl::hash_value(idx3) != asl::hash_value(idx5));

    ASL_TEST_EXPECT(asl::hash_value(idx4) != asl::hash_value(idx5));
}

ASL_TEST(simple_pool) // NOLINT
{
    using Pool = asl::IndexPool<8, 8>;
    Pool pool;

    auto a = pool.acquire_ensure();
    auto b = pool.acquire_ensure();

    ASL_TEST_EXPECT(!a.is_null());
    ASL_TEST_EXPECT(!b.is_null());
    ASL_TEST_EXPECT(a.index() == 0);
    ASL_TEST_EXPECT(b.index() == 1);
    ASL_TEST_EXPECT(a.gen() == 0);
    ASL_TEST_EXPECT(b.gen() == 0);
    ASL_TEST_EXPECT(!pool.is_full());

    ASL_TEST_EXPECT(a != b);

    ASL_TEST_EXPECT(!pool.is_valid({}));
    ASL_TEST_EXPECT(pool.is_valid(a));
    ASL_TEST_EXPECT(pool.is_valid(b));

    pool.release(a);

    ASL_TEST_EXPECT(!pool.is_valid(a));
    ASL_TEST_EXPECT(pool.is_valid(b));

    auto c = pool.acquire_ensure();

    ASL_TEST_EXPECT(!c.is_null());
    ASL_TEST_EXPECT(c.index() == 0);
    ASL_TEST_EXPECT(c.gen() == 1);

    ASL_TEST_EXPECT(a != c);
    ASL_TEST_EXPECT(b != c);

    ASL_TEST_EXPECT(!pool.is_valid(a));
    ASL_TEST_EXPECT(pool.is_valid(b));
    ASL_TEST_EXPECT(pool.is_valid(c));

    pool.release(b);

    ASL_TEST_EXPECT(!pool.is_valid(a));
    ASL_TEST_EXPECT(!pool.is_valid(b));
    ASL_TEST_EXPECT(pool.is_valid(c));

    pool.release(c);

    ASL_TEST_EXPECT(!pool.is_valid(a));
    ASL_TEST_EXPECT(!pool.is_valid(b));
    ASL_TEST_EXPECT(!pool.is_valid(c));
}

ASL_TEST(pool_acquire_release_a_lot)
{
    using Pool = asl::IndexPool<3, 3>;
    Pool pool;

    for (int i = 0; i < 80; ++i)
    {
        pool.release(pool.acquire_ensure());
        ASL_TEST_EXPECT(!pool.is_full());
    }
}

ASL_TEST(pool_acquire_past_capacity)
{
    using Pool = asl::IndexPool<3, 3>;
    Pool pool;

    auto idx = pool.acquire_ensure();

    for (int i = 0; i < 7; ++i)
    {
        ASL_TEST_EXPECT(!pool.is_full());
        ASL_TEST_EXPECT(pool.acquire().has_value());
    }

    for (int i = 0; i < 8; ++i)
    {
        ASL_TEST_EXPECT(pool.is_full());
        ASL_TEST_EXPECT(!pool.acquire().has_value());
    }

    pool.release(idx);
    ASL_TEST_EXPECT(!pool.is_full());
    ASL_TEST_EXPECT(pool.acquire().has_value());
    ASL_TEST_EXPECT(pool.is_full());
    ASL_TEST_EXPECT(!pool.acquire().has_value());
}

ASL_TEST(pool_with_user_type)
{
    using Pool = asl::IndexPool<8, 8, Flags, 4>;
    Pool pool;

    const auto a = pool.acquire_ensure(kFlag0);
    const auto b = pool.acquire_ensure(kFlag2);

    ASL_TEST_EXPECT(a.user() == kFlag0);
    ASL_TEST_EXPECT(b.user() == kFlag2);
}

ASL_TEST(pool_with_payload)
{
    using Pool = asl::IndexPool<8, 8, asl::empty, 0, int>;
    Pool pool;

    const auto a = pool.acquire_ensure(37);
    const auto b = pool.acquire_ensure(28);

    ASL_TEST_EXPECT(*pool.get_payload(a) == 37);
    ASL_TEST_EXPECT(*pool.get_payload(b) == 28);
    ASL_TEST_EXPECT(pool.get_payload({}) == nullptr);

    pool.release(a);

    ASL_TEST_EXPECT(pool.get_payload(a) == nullptr);
    ASL_TEST_EXPECT(*pool.get_payload(b) == 28);
}

ASL_TEST(pool_with_payload_and_user)
{
    using Pool = asl::IndexPool<8, 8, Flags, 8, int>;
    Pool pool;

    const auto a = pool.acquire_ensure(kFlag1, 37);
    const auto b = pool.acquire_ensure(kFlag0, 28);

    ASL_TEST_EXPECT(a.user() == kFlag1);
    ASL_TEST_EXPECT(b.user() == kFlag0);

    ASL_TEST_EXPECT(*pool.get_payload(a) == 37);
    ASL_TEST_EXPECT(*pool.get_payload(b) == 28);
    ASL_TEST_EXPECT(pool.get_payload({}) == nullptr);

    pool.release(a);

    ASL_TEST_EXPECT(pool.get_payload(a) == nullptr);
    ASL_TEST_EXPECT(*pool.get_payload(b) == 28);
}

ASL_TEST(pool_exchange_payload)
{
    using Pool = asl::IndexPool<8, 8, asl::empty, 0, int>;
    Pool pool;

    const auto a = pool.acquire_ensure(37);
    const auto b = pool.acquire_ensure(28);

    ASL_TEST_EXPECT(*pool.get_payload(a) == 37);
    ASL_TEST_EXPECT(*pool.get_payload(b) == 28);

    ASL_TEST_EXPECT(!pool.exchange_payload({}, 0).has_value());

    ASL_TEST_EXPECT(pool.exchange_payload(a, 101).value() == 37);
    ASL_TEST_EXPECT(pool.exchange_payload(b, 102).value() == 28);

    ASL_TEST_EXPECT(*pool.get_payload(a) == 101);
    ASL_TEST_EXPECT(*pool.get_payload(b) == 102);
}

