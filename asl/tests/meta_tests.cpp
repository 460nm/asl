#include "asl/meta.hpp"
#include "asl/tests/test_types.hpp"

struct Struct {};
union Union {};
enum Enum : uint8_t { EnumVariant = 0, };
enum class EnumClass : uint8_t { Variant = 0, };

static_assert(!asl::same_as<long, short>);
static_assert(asl::same_as<int, int>);

static_assert(asl::same_as<asl::select_t<false, int, float>, float>);
static_assert(asl::same_as<asl::select_t<true, int, float>, int>);

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
static_assert(asl::move_constructible<CopyConstructible>);
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

static_assert(asl::same_as<int, asl::un_const_t<int>>);
static_assert(asl::same_as<int, asl::un_const_t<const int>>);
static_assert(asl::same_as<const int&, asl::un_const_t<const int&>>);

static_assert(asl::same_as<int, asl::un_volatile_t<int>>);
static_assert(asl::same_as<int, asl::un_volatile_t<volatile int>>);
static_assert(asl::same_as<volatile int&, asl::un_volatile_t<volatile int&>>);

static_assert(asl::same_as<int, asl::un_cv_t<int>>);
static_assert(asl::same_as<int, asl::un_cv_t<const int>>);
static_assert(asl::same_as<int, asl::un_cv_t<const volatile int>>);
static_assert(asl::same_as<int, asl::un_cv_t<volatile int>>);

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

static_assert(asl::same_as<int, asl::tame_t<int>>);
static_assert(asl::same_as<int(), asl::tame_t<int()>>);
static_assert(asl::same_as<int(float), asl::tame_t<int(float)>>);
static_assert(asl::same_as<int(float), asl::tame_t<int(float) &>>);
static_assert(asl::same_as<int(float), asl::tame_t<int(float) const &&>>);
static_assert(asl::same_as<int(float), asl::tame_t<int(float) volatile noexcept>>);
static_assert(asl::same_as<int(float), asl::tame_t<int(float) && noexcept>>);
static_assert(asl::same_as<int(float), asl::tame_t<int(float) const>>);

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

static_assert(asl::same_as<int, asl::un_ref_t<int>>);
static_assert(asl::same_as<int, asl::un_ref_t<int&>>);
static_assert(asl::same_as<int, asl::un_ref_t<int&&>>);
static_assert(asl::same_as<int() &, asl::un_ref_t<int() &>>);

static_assert(asl::types_count<int, float> == 2);
static_assert(asl::types_count<int, int> == 2);
static_assert(asl::types_count<int> == 1);
static_assert(asl::types_count<> == 0);

static_assert(asl::trivially_copyable<int>);
static_assert(!asl::trivially_copyable<HasDestructor>);
static_assert(!asl::trivially_copyable<CopyAssignable>);
static_assert(asl::trivially_copyable<DefaultConstructible>);
static_assert(asl::trivially_copyable<TriviallyDefaultConstructible>);

class Base {};
class Derived : public Base {};
class C {};
class D { public: operator C() { return c; } C c; }; // NOLINT
class E { public: template<class T> E(T&&) {} }; // NOLINT

static_assert(asl::convertible_from<Base*, Derived*>);
static_assert(!asl::convertible_from<Derived*, Base*>);
static_assert(asl::convertible_from<C, D>);
static_assert(!asl::convertible_from<C*, Derived*>);
static_assert(asl::convertible_from<E, Base>);

static_assert(!asl::convertible_from<int16_t(&)[], int32_t(&)[]>);
static_assert(asl::convertible_from<const int16_t(&)[], int16_t(&)[]>);
static_assert(asl::convertible_from<const int16_t(&)[], const int16_t(&)[]>);
static_assert(asl::convertible_from<int16_t(&)[], int16_t(&)[]>);
static_assert(!asl::convertible_from<int32_t(&)[], int16_t(&)[]>);
static_assert(!asl::convertible_from<int16_t(&)[], const int16_t(&)[]>);
static_assert(!asl::convertible_from<C(&)[], D(&)[]>);

static_assert(asl::derived_from<Derived, Base>);
static_assert(!asl::derived_from<Base, Derived>);
static_assert(!asl::derived_from<D, C>);
static_assert(!asl::derived_from<C, D>);
static_assert(!asl::derived_from<uint8_t, uint16_t>);
static_assert(!asl::derived_from<uint16_t, uint8_t>);
static_assert(!asl::derived_from<int, int>);

static_assert(!asl::is_const<int>);
static_assert(asl::is_const<const int>);
static_assert(!asl::is_const<const int*>);
static_assert(asl::is_const<int* const>);

static_assert(asl::is_floating_point<float>);
static_assert(asl::is_floating_point<const float>);
static_assert(asl::is_floating_point<volatile double>);
static_assert(!asl::is_floating_point<const float&>);
static_assert(!asl::is_floating_point<int>);
static_assert(!asl::is_floating_point<C>);
