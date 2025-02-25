#include "asl/types/maybe_uninit.hpp"
#include "asl/tests/types.hpp"

static_assert(asl::layout::of<int>() == asl::layout::of<asl::maybe_uninit<int>>());
static_assert(asl::size_of<int> == asl::size_of<asl::maybe_uninit<int>>);
static_assert(asl::align_of<int> == asl::align_of<asl::maybe_uninit<int>>);

#define TEST_TYPE_PROPERTIES(PRP) \
    static_assert(asl::PRP<asl::maybe_uninit<TrivialType>> == asl::PRP<TrivialType>); \
    static_assert(asl::PRP<asl::maybe_uninit<TrivialTypeDefaultValue>> == asl::PRP<TrivialTypeDefaultValue>); \
    static_assert(asl::PRP<asl::maybe_uninit<WithDestructor>> == asl::PRP<WithDestructor>); \
    static_assert(asl::PRP<asl::maybe_uninit<Copyable>> == asl::PRP<Copyable>); \
    static_assert(asl::PRP<asl::maybe_uninit<MoveableOnly>> == asl::PRP<MoveableOnly>); \
    static_assert(asl::PRP<asl::maybe_uninit<Pinned>> == asl::PRP<Pinned>);

TEST_TYPE_PROPERTIES(trivially_default_constructible);
TEST_TYPE_PROPERTIES(trivially_copy_constructible);
TEST_TYPE_PROPERTIES(trivially_move_constructible);
TEST_TYPE_PROPERTIES(trivially_copy_assignable);
TEST_TYPE_PROPERTIES(trivially_move_assignable);
TEST_TYPE_PROPERTIES(trivially_destructible);

static_assert(asl::same_as<int&, decltype(asl::declval<asl::maybe_uninit<int>&>().as_init_unsafe())>);
static_assert(asl::same_as<int&&, decltype(asl::declval<asl::maybe_uninit<int>&&>().as_init_unsafe())>);
static_assert(asl::same_as<const int&, decltype(asl::declval<const asl::maybe_uninit<int>&>().as_init_unsafe())>);
static_assert(asl::same_as<const int&&, decltype(asl::declval<const asl::maybe_uninit<int>>().as_init_unsafe())>);
static_assert(asl::same_as<int&&, decltype(asl::declval<asl::maybe_uninit<int>>().as_init_unsafe())>);
