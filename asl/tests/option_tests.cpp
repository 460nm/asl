#include "asl/option.hpp"
#include "asl/tests/test_types.hpp"
#include "asl/testing/testing.hpp"

static_assert(asl::trivially_destructible<asl::option<TriviallyDestructible>>);
static_assert(!asl::trivially_destructible<asl::option<HasDestructor>>);

static_assert(asl::copy_constructible<asl::option<int>>);
static_assert(asl::copy_constructible<asl::option<CopyConstructible>>);
static_assert(!asl::copy_constructible<asl::option<MoveConstructible>>);
static_assert(!asl::copy_constructible<asl::option<NonMoveConstructible>>);

static_assert(asl::move_constructible<asl::option<int>>);
static_assert(asl::move_constructible<asl::option<CopyConstructible>>);
static_assert(asl::move_constructible<asl::option<MoveConstructible>>);
static_assert(!asl::move_constructible<asl::option<NonMoveConstructible>>);

static_assert(asl::copy_assignable<asl::option<int>>);
static_assert(asl::copy_assignable<asl::option<CopyAssignable>>);
static_assert(!asl::copy_assignable<asl::option<MoveAssignable>>);
static_assert(!asl::copy_assignable<asl::option<NonMoveAssignable>>);

static_assert(asl::move_assignable<asl::option<int>>);
static_assert(asl::move_assignable<asl::option<CopyAssignable>>);
static_assert(!asl::move_assignable<asl::option<NonMoveAssignable>>);

ASL_TEST(Option_cheese)
{
    asl::option<int> a;
    asl::option<int> b;

    a = ASL_MOVE(b);
}

ASL_TEST(Option_cheese2)
{
    asl::option<int> a;
    asl::option<int> b;

    a = ASL_MOVE(b);
}

ASL_TEST(Option_cheese3)
{
    asl::option<int> a;
    asl::option<int> b;

    a = ASL_MOVE(b);
    asl::testing::report_failure("OH NO", __FILE__, __LINE__);
}

ASL_TEST(Option_cheese4)
{
    asl::option<int> a;
    asl::option<int> b;

    a = ASL_MOVE(b);
}
