#include "asl/meta.hpp"
#include "asl/tests/test_types.hpp"
#include "asl/testing/testing.hpp"
#include "asl/box.hpp"

struct Struct {};
union Union {};
enum Enum : uint8_t { EnumVariant = 0, };
enum class EnumClass : uint8_t { Variant = 0, };

static_assert(!asl::same_as<long, short>);
static_assert(asl::same_as<int, int>);

static_assert(asl::same_as<asl::select_t<false, int, float>, float>);
static_assert(asl::same_as<asl::select_t<true, int, float>, int>);

static_assert(asl::default_constructible<int>);
static_assert(asl::default_constructible<TrivialType>);
static_assert(asl::default_constructible<TrivialTypeDefaultValue>);

static_assert(asl::trivially_default_constructible<int>);
static_assert(asl::trivially_default_constructible<TrivialType>);
static_assert(!asl::trivially_default_constructible<TrivialTypeDefaultValue>);

static_assert(asl::copy_constructible<int>);
static_assert(asl::copy_constructible<TrivialType>);
static_assert(asl::copy_constructible<Copyable>);
static_assert(!asl::copy_constructible<MoveableOnly>);
static_assert(!asl::copy_constructible<Pinned>);

static_assert(asl::trivially_copy_constructible<int>);
static_assert(asl::trivially_copy_constructible<TrivialType>);
static_assert(asl::trivially_copy_constructible<TrivialTypeDefaultValue>);
static_assert(!asl::trivially_copy_constructible<WithDestructor>);
static_assert(!asl::trivially_copy_constructible<Copyable>);
static_assert(!asl::trivially_copy_constructible<MoveableOnly>);
static_assert(!asl::trivially_copy_constructible<Pinned>);

static_assert(asl::move_constructible<int>);
static_assert(asl::move_constructible<TrivialType>);
static_assert(asl::move_constructible<Copyable>);
static_assert(asl::move_constructible<MoveableOnly>);
static_assert(!asl::move_constructible<Pinned>);

static_assert(asl::trivially_move_constructible<int>);
static_assert(asl::trivially_move_constructible<TrivialType>);
static_assert(asl::trivially_move_constructible<TrivialTypeDefaultValue>);
static_assert(!asl::trivially_move_constructible<WithDestructor>);
static_assert(!asl::trivially_move_constructible<Copyable>);
static_assert(!asl::trivially_move_constructible<MoveableOnly>);
static_assert(!asl::trivially_move_constructible<Pinned>);

static_assert(asl::copy_assignable<int>);
static_assert(asl::copy_assignable<TrivialType>);
static_assert(asl::copy_assignable<Copyable>);
static_assert(!asl::copy_assignable<MoveableOnly>);
static_assert(!asl::copy_assignable<Pinned>);

static_assert(asl::trivially_copy_assignable<int>);
static_assert(asl::trivially_copy_assignable<TrivialType>);
static_assert(asl::trivially_copy_assignable<TrivialTypeDefaultValue>);
static_assert(asl::trivially_copy_assignable<WithDestructor>);
static_assert(!asl::trivially_copy_assignable<Copyable>);
static_assert(!asl::trivially_copy_assignable<MoveableOnly>);
static_assert(!asl::trivially_copy_assignable<Pinned>);

static_assert(asl::copyable<int>);
static_assert(asl::copyable<TrivialType>);
static_assert(asl::copyable<Copyable>);
static_assert(!asl::copyable<MoveableOnly>);
static_assert(!asl::copyable<Pinned>);

static_assert(asl::moveable<int>);
static_assert(asl::moveable<TrivialType>);
static_assert(asl::moveable<Copyable>);
static_assert(asl::moveable<MoveableOnly>);
static_assert(!asl::moveable<Pinned>);

static_assert(asl::move_assignable<int>);
static_assert(asl::move_assignable<TrivialType>);
static_assert(asl::move_assignable<Copyable>);
static_assert(asl::move_assignable<MoveableOnly>);
static_assert(!asl::move_assignable<Pinned>);

static_assert(asl::trivially_move_assignable<int>);
static_assert(asl::trivially_move_assignable<TrivialType>);
static_assert(asl::trivially_move_assignable<TrivialTypeDefaultValue>);
static_assert(asl::trivially_move_assignable<WithDestructor>);
static_assert(!asl::trivially_move_assignable<Copyable>);
static_assert(!asl::trivially_move_assignable<MoveableOnly>);
static_assert(!asl::trivially_move_assignable<Pinned>);

static_assert(asl::trivially_destructible<int>);
static_assert(asl::trivially_destructible<TrivialType>);
static_assert(asl::trivially_destructible<TrivialTypeDefaultValue>);
static_assert(!asl::trivially_destructible<WithDestructor>);
static_assert(asl::trivially_destructible<Copyable>);
static_assert(asl::trivially_destructible<MoveableOnly>);
static_assert(asl::trivially_destructible<Pinned>);

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

static_assert(asl::uniquely_represented<int>);
static_assert(asl::uniquely_represented<uint128_t>);
static_assert(!asl::uniquely_represented<bool>);

enum Enum1 {};
enum class Enum2 {};

static_assert(asl::uniquely_represented<Enum1>);
static_assert(asl::uniquely_represented<Enum2>);

static_assert(!asl::is_enum<int>);
static_assert(asl::is_enum<Enum1>);
static_assert(asl::is_enum<Enum2>);

static_assert(asl::derefs_as<int, int>);
static_assert(asl::derefs_as<int*, int>);
static_assert(asl::derefs_as<int&, int>);
static_assert(asl::derefs_as<asl::box<int>, int>);

static_assert(asl::derefs_as<Derived, Base>);
static_assert(asl::derefs_as<Derived*, Base>);
static_assert(asl::derefs_as<Derived&, Base>);
static_assert(asl::derefs_as<asl::box<Derived>, Base>);

static void wants_int(int) {}
static void wants_base(Base&) {}
static void wants_base_ptr(Base*) {}

ASL_TEST(deref)
{
    int a = 4;
    auto b = asl::make_box<int>(5);
    
    wants_int(asl::deref<int>(5));
    wants_int(asl::deref<int>(a));
    wants_int(asl::deref<int>(&a));
    wants_int(asl::deref<int>(b));

    Derived c{};
    auto d = asl::make_box<Derived>();

    wants_base(asl::deref<Base>(Derived{}));
    wants_base(asl::deref<Base>(c));
    wants_base(asl::deref<Base>(&c));
    wants_base(asl::deref<Base>(d));

    wants_base_ptr(&asl::deref<Base>(Derived{}));
    wants_base_ptr(&asl::deref<Base>(c));
    wants_base_ptr(&asl::deref<Base>(&c));
    wants_base_ptr(&asl::deref<Base>(d));
}

