#include "asl/object.hpp"
#include "asl/test_types.hpp"

static_assert(asl::is_object<asl::object<void>>);
static_assert(asl::is_object<asl::object<int>>);
static_assert(asl::is_object<asl::object<int&>>);
static_assert(asl::is_object<asl::object<int()>>);

static_assert(asl::is_empty<asl::object<void>>);
static_assert(asl::is_empty<asl::object<asl::empty>>);
static_assert(sizeof(asl::object<int>) == 4);
static_assert(sizeof(asl::object<int*>) == sizeof(void*));
static_assert(sizeof(asl::object<int&>) == sizeof(void*));
static_assert(sizeof(asl::object<int()>) == sizeof(void*));

static_assert(asl::default_constructible<asl::object<int>>);
static_assert(asl::default_constructible<asl::object<TriviallyDefaultConstructible>>);
static_assert(asl::default_constructible<asl::object<DefaultConstructible>>);
static_assert(!asl::default_constructible<asl::object<NonDefaultConstructible>>);

static_assert(asl::trivially_default_constructible<asl::object<int>>);
static_assert(asl::trivially_default_constructible<asl::object<TriviallyDefaultConstructible>>);
static_assert(!asl::trivially_default_constructible<asl::object<DefaultConstructible>>);
static_assert(!asl::trivially_default_constructible<asl::object<NonDefaultConstructible>>);

static_assert(asl::copy_constructible<asl::object<int>>);
static_assert(asl::copy_constructible<asl::object<TriviallyCopyConstructible>>);
static_assert(asl::copy_constructible<asl::object<CopyConstructible>>);
static_assert(!asl::copy_constructible<asl::object<NonCopyConstructible>>);

static_assert(asl::trivially_copy_constructible<asl::object<int>>);
static_assert(asl::trivially_copy_constructible<asl::object<TriviallyCopyConstructible>>);
static_assert(!asl::trivially_copy_constructible<asl::object<CopyConstructible>>);
static_assert(!asl::trivially_copy_constructible<asl::object<NonCopyConstructible>>);

static_assert(asl::move_constructible<asl::object<int>>);
static_assert(asl::move_constructible<asl::object<TriviallyMoveConstructible>>);
static_assert(asl::move_constructible<asl::object<MoveConstructible>>);
static_assert(!asl::move_constructible<asl::object<NonMoveConstructible>>);

static_assert(asl::trivially_move_constructible<asl::object<int>>);
static_assert(asl::trivially_move_constructible<asl::object<TriviallyMoveConstructible>>);
static_assert(!asl::trivially_move_constructible<asl::object<MoveConstructible>>);
static_assert(!asl::trivially_move_constructible<asl::object<NonMoveConstructible>>);

static_assert(asl::copy_assignable<asl::object<int>>);
static_assert(asl::copy_assignable<asl::object<CopyAssignable>>);
static_assert(asl::copy_assignable<asl::object<TriviallyCopyAssignable>>);
static_assert(!asl::copy_assignable<asl::object<NoCopyAssignable>>);

static_assert(asl::trivially_copy_assignable<asl::object<int>>);
static_assert(!asl::trivially_copy_assignable<asl::object<CopyAssignable>>);
static_assert(asl::trivially_copy_assignable<asl::object<TriviallyCopyAssignable>>);
static_assert(!asl::trivially_copy_assignable<asl::object<NoCopyAssignable>>);

static_assert(asl::move_assignable<asl::object<int>>);
static_assert(asl::move_assignable<asl::object<MoveAssignable>>);
static_assert(asl::move_assignable<asl::object<TriviallyMoveAssignable>>);
static_assert(!asl::move_assignable<asl::object<NoMoveAssignable>>);

static_assert(asl::trivially_move_assignable<asl::object<int>>);
static_assert(!asl::trivially_move_assignable<asl::object<MoveAssignable>>);
static_assert(asl::trivially_move_assignable<asl::object<TriviallyMoveAssignable>>);
static_assert(!asl::trivially_move_assignable<asl::object<NoMoveAssignable>>);

static_assert(asl::trivially_destructible<asl::object<int>>);
static_assert(asl::trivially_destructible<asl::object<TriviallyDestructible>>);
static_assert(!asl::trivially_destructible<asl::object<HasDestructor>>);

int main() { return 0; }
