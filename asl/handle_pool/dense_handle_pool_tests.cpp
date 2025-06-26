// Copyright 2025 Steven Le Rouzic
//
// SPDX-License-Identifier: BSD-3-Clause

#include "asl/testing/testing.hpp"
#include "asl/tests/types.hpp"
#include "asl/handle_pool/dense_handle_pool.hpp"

ASL_TEST(acquire_release) // NOLINT
{
    asl::DenseHandlePool<int, 1, 1> pool;

    ASL_TEST_EXPECT(!pool.is_full());

    const auto a = pool.acquire_ensure(6);
    const auto b = pool.acquire_ensure(7);

    ASL_TEST_EXPECT(pool.is_valid(a));
    ASL_TEST_EXPECT(pool.is_valid(b));
    ASL_TEST_EXPECT(pool.is_full());

    ASL_TEST_EXPECT(pool.get_ensure(a) == 6);
    ASL_TEST_EXPECT(pool.get_ensure(b) == 7);

    pool.release(a);
    ASL_TEST_EXPECT(!pool.is_valid(a));
    ASL_TEST_EXPECT(pool.is_valid(b));
    ASL_TEST_EXPECT(!pool.is_full());
    ASL_TEST_EXPECT(pool.get(a) == nullptr);
    ASL_TEST_EXPECT(*pool.get(b) == 7);

    const auto c = pool.acquire_ensure(8);

    ASL_TEST_EXPECT(!pool.is_valid(a));
    ASL_TEST_EXPECT(pool.is_valid(b));
    ASL_TEST_EXPECT(pool.is_valid(c));
    ASL_TEST_EXPECT(pool.is_full());

    ASL_TEST_EXPECT(*pool.get(b) == 7);
    ASL_TEST_EXPECT(*pool.get(c) == 8);
    ASL_TEST_EXPECT(pool.get(a) == nullptr);
}

ASL_TEST(element_destructor)
{
    asl::DenseHandlePool<DestructorObserver, 8, 8> pool;
    bool d[3]{};

    const auto d0 = pool.acquire_ensure(&d[0]);
    const auto d1 = pool.acquire_ensure(&d[1]);
    const auto d2 = pool.acquire_ensure(&d[2]);

    ASL_TEST_EXPECT(!d[0]);
    ASL_TEST_EXPECT(!d[1]);
    ASL_TEST_EXPECT(!d[2]);

    pool.release(d1);
    ASL_TEST_EXPECT(!d[0]);
    ASL_TEST_EXPECT(d[1]);
    ASL_TEST_EXPECT(!d[2]);

    pool.release(d0);
    ASL_TEST_EXPECT(d[0]);
    ASL_TEST_EXPECT(d[1]);
    ASL_TEST_EXPECT(!d[2]);

    pool.release(d2);
    ASL_TEST_EXPECT(d[0]);
    ASL_TEST_EXPECT(d[1]);
    ASL_TEST_EXPECT(d[2]);
}

ASL_TEST(destructor)
{
    bool d[3]{};

    {
        asl::DenseHandlePool<DestructorObserver, 8, 8> pool;

        pool.acquire_ensure(&d[0]);
        const auto d1 = pool.acquire_ensure(&d[1]);
        pool.acquire_ensure(&d[2]);

        ASL_TEST_EXPECT(!d[0]);
        ASL_TEST_EXPECT(!d[1]);
        ASL_TEST_EXPECT(!d[2]);

        pool.release(d1);
        ASL_TEST_EXPECT(!d[0]);
        ASL_TEST_EXPECT(d[1]);
        ASL_TEST_EXPECT(!d[2]);
    }

    ASL_TEST_EXPECT(d[0]);
    ASL_TEST_EXPECT(d[1]);
    ASL_TEST_EXPECT(d[2]);
}

enum Flags : uint8_t
{
    kFlag1 = 1,
    kFlag2 = 2,
};

ASL_TEST(user_type)
{
    asl::DenseHandlePool<int, 8, 8, Flags> pool;

    auto a = pool.acquire_ensure(kFlag2, 22);
    auto b = pool.acquire_ensure(kFlag1, 11);

    ASL_TEST_EXPECT(pool.get_ensure(a) == 22);
    ASL_TEST_EXPECT(pool.get_ensure(b) == 11);

    ASL_TEST_EXPECT(a.user() == kFlag2);
    ASL_TEST_EXPECT(b.user() == kFlag1);
}
