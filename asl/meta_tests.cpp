#include "asl/meta.hpp"
#include "asl/test_types.hpp"

struct Struct {};
union Union {};
enum Enum { EnumVariant = 0, };
enum class EnumClass { Variant = 0, };

static_assert(!asl::is_same<long, short>);
static_assert(asl::is_same<int, int>);

static_assert(asl::is_same<asl::select_t<false, int, float>, float>);
static_assert(asl::is_same<asl::select_t<true, int, float>, int>);

static_assert(asl::default_constructible<int>);
static_assert(asl::default_constructible<TriviallyDefaultConstructible>);
static_assert(asl::default_constructible<DefaultConstructible>);
static_assert(!asl::default_constructible<NonDefaultConstructible>);

static_assert(asl::trivially_default_constructible<int>);
static_assert(asl::trivially_default_constructible<TriviallyDefaultConstructible>);
static_assert(!asl::trivially_default_constructible<DefaultConstructible>);
static_assert(!asl::trivially_default_constructible<NonDefaultConstructible>);

static_assert(asl::copy_constructible<int>);
static_assert(asl::copy_constructible<TriviallyCopyConstructible>);
static_assert(asl::copy_constructible<CopyConstructible>);
static_assert(!asl::copy_constructible<NonCopyConstructible>);

static_assert(asl::trivially_copy_constructible<int>);
static_assert(asl::trivially_copy_constructible<TriviallyCopyConstructible>);
static_assert(!asl::trivially_copy_constructible<CopyConstructible>);
static_assert(!asl::trivially_copy_constructible<NonCopyConstructible>);

static_assert(asl::move_constructible<int>);
static_assert(asl::move_constructible<TriviallyMoveConstructible>);
static_assert(asl::move_constructible<MoveConstructible>);
static_assert(!asl::move_constructible<NonMoveConstructible>);

static_assert(asl::trivially_move_constructible<int>);
static_assert(asl::trivially_move_constructible<TriviallyMoveConstructible>);
static_assert(!asl::trivially_move_constructible<MoveConstructible>);
static_assert(!asl::trivially_move_constructible<NonMoveConstructible>);

static_assert(asl::copy_assignable<int>);
static_assert(asl::copy_assignable<CopyAssignable>);
static_assert(asl::copy_assignable<TriviallyCopyAssignable>);
static_assert(!asl::copy_assignable<NonCopyAssignable>);

static_assert(asl::trivially_copy_assignable<int>);
static_assert(!asl::trivially_copy_assignable<CopyAssignable>);
static_assert(asl::trivially_copy_assignable<TriviallyCopyAssignable>);
static_assert(!asl::trivially_copy_assignable<NonCopyAssignable>);

static_assert(asl::move_assignable<int>);
static_assert(asl::move_assignable<MoveAssignable>);
static_assert(asl::move_assignable<TriviallyMoveAssignable>);
static_assert(!asl::move_assignable<NonMoveAssignable>);

static_assert(asl::trivially_move_assignable<int>);
static_assert(!asl::trivially_move_assignable<MoveAssignable>);
static_assert(asl::trivially_move_assignable<TriviallyMoveAssignable>);
static_assert(!asl::trivially_move_assignable<NonMoveAssignable>);

static_assert(asl::trivially_destructible<int>);
static_assert(asl::trivially_destructible<TriviallyDestructible>);
static_assert(!asl::trivially_destructible<HasDestructor>);

static_assert(asl::is_same<int, asl::un_const_t<int>>);
static_assert(asl::is_same<int, asl::un_const_t<const int>>);
static_assert(asl::is_same<const int&, asl::un_const_t<const int&>>);

static_assert(asl::is_same<int, asl::un_volatile_t<int>>);
static_assert(asl::is_same<int, asl::un_volatile_t<volatile int>>);
static_assert(asl::is_same<volatile int&, asl::un_volatile_t<volatile int&>>);

static_assert(asl::is_same<int, asl::un_cv_t<int>>);
static_assert(asl::is_same<int, asl::un_cv_t<const int>>);
static_assert(asl::is_same<int, asl::un_cv_t<const volatile int>>);
static_assert(asl::is_same<int, asl::un_cv_t<volatile int>>);

static_assert(asl::is_void<void>);
static_assert(asl::is_void<const void>);
static_assert(asl::is_void<const volatile void>);
static_assert(asl::is_void<volatile void>);
static_assert(!asl::is_void<int>);
static_assert(!asl::is_void<Struct>);
static_assert(!asl::is_void<int&>);
static_assert(!asl::is_void<int&&>);
static_assert(!asl::is_void<void()>);
static_assert(!asl::is_void<void() const &&>);

static_assert(asl::is_ref<int&>);
static_assert(asl::is_ref<const int&>);
static_assert(asl::is_ref<const volatile int&>);
static_assert(asl::is_ref<int&&>);
static_assert(!asl::is_ref<int>);
static_assert(!asl::is_ref<void>);
static_assert(!asl::is_ref<void()>);
static_assert(!asl::is_ref<void() const &&>);

static_assert(asl::is_ptr<int*>);
static_assert(asl::is_ptr<const int* const>);
static_assert(asl::is_ptr<const volatile int*>);
static_assert(!asl::is_ptr<int>);
static_assert(!asl::is_ptr<void>);
static_assert(!asl::is_ptr<void()>);
static_assert(!asl::is_ptr<void() const &&>);

static_assert(asl::is_same<int, asl::tame_t<int>>);
static_assert(asl::is_same<int(), asl::tame_t<int()>>);
static_assert(asl::is_same<int(float), asl::tame_t<int(float)>>);
static_assert(asl::is_same<int(float), asl::tame_t<int(float) &>>);
static_assert(asl::is_same<int(float), asl::tame_t<int(float) const &&>>);
static_assert(asl::is_same<int(float), asl::tame_t<int(float) volatile noexcept>>);
static_assert(asl::is_same<int(float), asl::tame_t<int(float) && noexcept>>);
static_assert(asl::is_same<int(float), asl::tame_t<int(float) const>>);

static_assert(asl::is_func<void()>);
static_assert(asl::is_func<void(int)>);
static_assert(asl::is_func<void(int, float)>);
static_assert(asl::is_func<void() &>);
static_assert(asl::is_func<void() const &&>);
static_assert(asl::is_func<void() volatile noexcept>);
static_assert(!asl::is_func<void(*)()>);
static_assert(!asl::is_func<int>);
static_assert(!asl::is_func<int&>);
static_assert(!asl::is_func<void>);

static_assert(asl::is_object<Struct>);
static_assert(asl::is_object<int>);
static_assert(asl::is_object<int*>);
static_assert(asl::is_object<int Struct::*>);
static_assert(asl::is_object<int (Struct::*)(float)>);
static_assert(asl::is_object<int[]>);
static_assert(asl::is_object<int[45]>);
static_assert(asl::is_object<Enum>);
static_assert(!asl::is_object<int&>);
static_assert(!asl::is_object<void>);
static_assert(!asl::is_object<void(int)>);
static_assert(!asl::is_object<int(float) const && noexcept>);

static_assert(!asl::is_array<Struct>);
static_assert(!asl::is_array<int>);
static_assert(!asl::is_array<int*>);
static_assert(!asl::is_array<int Struct::*>);
static_assert(!asl::is_array<int (Struct::*)(float)>);
static_assert(asl::is_array<int[]>);
static_assert(asl::is_array<int[45]>);
static_assert(!asl::is_array<Enum>);
static_assert(!asl::is_array<int&>);
static_assert(!asl::is_array<void>);
static_assert(!asl::is_array<void(int)>);
static_assert(!asl::is_array<int(float) const && noexcept>);

static_assert(asl::is_same<int, asl::un_ref_t<int>>);
static_assert(asl::is_same<int, asl::un_ref_t<int&>>);
static_assert(asl::is_same<int, asl::un_ref_t<int&&>>);
static_assert(asl::is_same<int() &, asl::un_ref_t<int() &>>);

static_assert(asl::types_count<int, float> == 2);
static_assert(asl::types_count<int, int> == 2);
static_assert(asl::types_count<int> == 1);
static_assert(asl::types_count<> == 0);

int main() { return 0; }
