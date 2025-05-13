// Copyright 2025 Steven Le Rouzic
//
// SPDX-License-Identifier: BSD-3-Clause

#include "asl/types/array.hpp"

#include "asl/testing/testing.hpp"
#include "asl/tests/types.hpp"

static_assert(sizeof(asl::array<int32_t, 8>) == 32);

static_assert(asl::default_constructible<asl::array<int, 6>>);
static_assert(asl::trivially_default_constructible<asl::array<int, 6>>);

static_assert(asl::default_constructible<asl::array<TrivialType, 6>>);
static_assert(asl::trivially_default_constructible<asl::array<TrivialType, 6>>);
static_assert(asl::trivially_copy_constructible<asl::array<TrivialType, 6>>);
static_assert(asl::trivially_copy_assignable<asl::array<TrivialType, 6>>);
static_assert(asl::trivially_move_constructible<asl::array<TrivialType, 6>>);
static_assert(asl::trivially_move_assignable<asl::array<TrivialType, 6>>);

static_assert(asl::default_constructible<asl::array<TrivialTypeDefaultValue, 6>>);
static_assert(!asl::trivially_default_constructible<asl::array<TrivialTypeDefaultValue, 6>>);

static_assert(asl::trivially_destructible<asl::array<int, 6>>);
static_assert(asl::trivially_destructible<asl::array<TrivialType, 6>>);
static_assert(!asl::trivially_destructible<asl::array<WithDestructor, 6>>);

static_assert(asl::copyable<asl::array<Copyable, 6>>);
static_assert(!asl::copyable<asl::array<MoveableOnly, 6>>);
static_assert(!asl::copyable<asl::array<Pinned, 6>>);

static_assert(asl::moveable<asl::array<Copyable, 6>>);
static_assert(asl::moveable<asl::array<MoveableOnly, 6>>);
static_assert(!asl::moveable<asl::array<Pinned, 6>>);

ASL_TEST(construct_default)
{
    asl::array<int, 4> arr{};

    ASL_TEST_EXPECT(static_cast<void*>(&arr) == static_cast<void*>(arr.data()));

    ASL_TEST_EXPECT(arr[0] == 0);
    ASL_TEST_EXPECT(arr[1] == 0);
    ASL_TEST_EXPECT(arr[2] == 0);
    ASL_TEST_EXPECT(arr[3] == 0);

    ASL_TEST_EXPECT(arr.data()[0] == 0); // NOLINT
    ASL_TEST_EXPECT(arr.data()[1] == 0); // NOLINT
    ASL_TEST_EXPECT(arr.data()[2] == 0); // NOLINT
    ASL_TEST_EXPECT(arr.data()[3] == 0); // NOLINT
}

ASL_TEST(construct)
{
    asl::array<int, 4> arr{10, 11, 12, 13};

    ASL_TEST_EXPECT(arr[0] == 10);
    ASL_TEST_EXPECT(arr[1] == 11);
    ASL_TEST_EXPECT(arr[2] == 12);
    ASL_TEST_EXPECT(arr[3] == 13);

    ASL_TEST_EXPECT(arr.data()[0] == 10); // NOLINT
    ASL_TEST_EXPECT(arr.data()[1] == 11); // NOLINT
    ASL_TEST_EXPECT(arr.data()[2] == 12); // NOLINT
    ASL_TEST_EXPECT(arr.data()[3] == 13); // NOLINT
}

static_assert(asl::convertible_to<asl::array<int, 4>, asl::span<int, 4>>);
static_assert(asl::convertible_to<asl::array<int, 4>, asl::span<int>>);

ASL_TEST(sized_span)
{
    asl::array<int, 4> arr{10, 11, 12, 13};
    const asl::span<int, 4> s1 = arr;

    ASL_TEST_EXPECT(s1.size() == 4);
    ASL_TEST_EXPECT(s1.data() == arr.data());
    ASL_TEST_EXPECT(s1[0] == 10);
    ASL_TEST_EXPECT(s1[1] == 11);
    ASL_TEST_EXPECT(s1[2] == 12);
    ASL_TEST_EXPECT(s1[3] == 13);
}

ASL_TEST(unsized_span)
{
    asl::array<int, 4> arr{10, 11, 12, 13};
    const asl::span<int> s2 = arr;

    ASL_TEST_EXPECT(s2.size() == 4);
    ASL_TEST_EXPECT(s2[0] == 10);
    ASL_TEST_EXPECT(s2[1] == 11);
    ASL_TEST_EXPECT(s2[2] == 12);
    ASL_TEST_EXPECT(s2[3] == 13);
}

ASL_TEST(iterator)
{
    const asl::array<int, 4> arr{10, 11, 12, 13};

    auto it = arr.begin();
    auto end = arr.end();

    ASL_TEST_ASSERT(it != end);
    ASL_TEST_EXPECT(*it == 10);
    it++;

    ASL_TEST_ASSERT(it != end);
    ASL_TEST_EXPECT(*it == 11);
    it++;

    ASL_TEST_ASSERT(it != end);
    ASL_TEST_EXPECT(*it == 12);
    it++;

    ASL_TEST_ASSERT(it != end);
    ASL_TEST_EXPECT(*it == 13);
    it++;

    ASL_TEST_ASSERT(it == end);
}

