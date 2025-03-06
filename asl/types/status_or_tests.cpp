// Copyright 2025 Steven Le Rouzic
//
// SPDX-License-Identifier: BSD-3-Clause

#include "asl/types/status_or.hpp"
#include "asl/testing/testing.hpp"
#include "asl/tests/types.hpp"

static_assert(asl::copyable<asl::status_or<TrivialType>>);
static_assert(asl::moveable<asl::status_or<TrivialType>>);

static_assert(asl::copyable<asl::status_or<Copyable>>);
static_assert(asl::moveable<asl::status_or<Copyable>>);

static_assert(!asl::copyable<asl::status_or<MoveableOnly>>);
static_assert(asl::moveable<asl::status_or<MoveableOnly>>);

static_assert(!asl::copyable<asl::status_or<Pinned>>);
static_assert(!asl::moveable<asl::status_or<Pinned>>);

ASL_TEST(ok)
{
    const  asl::status_or<int> s = 6;
    ASL_TEST_EXPECT(s.ok());
    ASL_TEST_EXPECT(s.code() == asl::status_code::ok);
}

ASL_TEST(from_status)
{
    const asl::status_or<char> s = asl::internal_error();
    ASL_TEST_EXPECT(!s.ok());
    ASL_TEST_EXPECT(s.code() == asl::status_code::internal);
    ASL_TEST_EXPECT(s.message() == ""_sv);

    const asl::status_or<int> s2 = asl::internal_error("oh no");
    ASL_TEST_EXPECT(!s2.ok());
    ASL_TEST_EXPECT(s2.code() == asl::status_code::internal);
    ASL_TEST_EXPECT(s2.message() == "oh no"_sv);

    const asl::status_or<int> s3 = asl::internal_error("{} {}", 1, 2);
    ASL_TEST_EXPECT(!s3.ok());
    ASL_TEST_EXPECT(s3.code() == asl::status_code::internal);
    ASL_TEST_EXPECT(s3.message() == "1 2"_sv);
}

ASL_TEST(destructor)
{
    bool d = false;

    {
        asl::status_or<DestructorObserver> s{&d};
        ASL_TEST_EXPECT(s.ok());
        ASL_TEST_EXPECT(!d);

        asl::status_or s2 = std::move(s);
        ASL_TEST_EXPECT(s2.ok());
        ASL_TEST_EXPECT(!d);

        s = std::move(s2);
        ASL_TEST_EXPECT(s.ok());
        ASL_TEST_EXPECT(!d);
    }

    ASL_TEST_EXPECT(d);
}

ASL_TEST(copy)
{
    asl::status_or<int> s = 7;
    asl::status_or s2 = s;
    s = s2;

    ASL_TEST_EXPECT(s.ok());
    ASL_TEST_EXPECT(s2.ok());

    ASL_TEST_EXPECT(s.value() == 7);
    ASL_TEST_EXPECT(s2.value() == 7);
}

ASL_TEST(value_or)
{
    const asl::status_or<int> s = 7;
    const asl::status_or<int> s2 = asl::internal_error();

    ASL_TEST_EXPECT(s.value_or(45) == 7);
    ASL_TEST_EXPECT(s2.value_or(45) == 45);
}

