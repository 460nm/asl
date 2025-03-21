// Copyright 2025 Steven Le Rouzic
//
// SPDX-License-Identifier: BSD-3-Clause

#include "asl/types/option.hpp"
#include "asl/tests/types.hpp"
#include "asl/testing/testing.hpp"

class Base {};
class Derived : public Base {};

struct NonZero
{
    int value;

    constexpr explicit NonZero(int x) : value(x)
    {
        ASL_ASSERT(x != 0);
    }

    constexpr explicit NonZero(asl::niche_t) : value(0) {}

    constexpr bool operator==(asl::niche_t) const { return value == 0; }
};
static_assert(asl::has_niche<NonZero>);
static_assert(!asl::has_niche<int>);

static_assert(sizeof(asl::option<int>) >= sizeof(int));
static_assert(sizeof(asl::option<NonZero>) == sizeof(NonZero));

static_assert(!asl::is_option<int>);
static_assert(asl::is_option<asl::option<int>>);
static_assert(asl::is_option<const asl::option<int>>);

static_assert(asl::trivially_destructible<asl::option<TrivialType>>);
static_assert(!asl::trivially_destructible<asl::option<WithDestructor>>);

static_assert(asl::trivially_copy_constructible<asl::option<int>>);
static_assert(asl::trivially_copy_constructible<asl::option<NonZero>>);
static_assert(asl::copy_constructible<asl::option<Copyable>>);
static_assert(!asl::copy_constructible<asl::option<MoveableOnly>>);
static_assert(!asl::copy_constructible<asl::option<Pinned>>);

static_assert(asl::trivially_move_constructible<asl::option<int>>);
static_assert(asl::trivially_move_constructible<asl::option<NonZero>>);
static_assert(asl::move_constructible<asl::option<Copyable>>);
static_assert(asl::move_constructible<asl::option<MoveableOnly>>);
static_assert(!asl::move_constructible<asl::option<Pinned>>);

static_assert(asl::trivially_copy_assignable<asl::option<int>>);
static_assert(asl::trivially_copy_assignable<asl::option<NonZero>>);
static_assert(asl::copy_assignable<asl::option<Copyable>>);
static_assert(!asl::copy_assignable<asl::option<MoveableOnly>>);
static_assert(!asl::copy_assignable<asl::option<Pinned>>);

static_assert(asl::trivially_move_assignable<asl::option<int>>);
static_assert(asl::trivially_move_assignable<asl::option<NonZero>>);
static_assert(asl::move_assignable<asl::option<Copyable>>);
static_assert(asl::move_assignable<asl::option<MoveableOnly>>);
static_assert(!asl::move_assignable<asl::option<Pinned>>);

static_assert(asl::assignable_from<asl::option<Base*>&, asl::option<Derived*>>);
static_assert(!asl::assignable_from<asl::option<Derived*>&, asl::option<Base*>>);

static_assert(asl::convertible_to<asl::option<Derived*>, asl::option<Base*>>);
static_assert(!asl::convertible_to<asl::option<Base*>, asl::option<Derived*>>);

class ExplicitConversion { public: explicit ExplicitConversion(int) {} };
class ImplicitConversion { public: ImplicitConversion(int) {} }; // NOLINT

static_assert(!asl::convertible_to<int, ExplicitConversion>);
static_assert(asl::convertible_to<int, ImplicitConversion>);

static_assert(!asl::convertible_to<int, asl::option<ExplicitConversion>>);
static_assert(asl::convertible_to<int, asl::option<ImplicitConversion>>);

static_assert(!asl::convertible_to<asl::option<int>, asl::option<ExplicitConversion>>);
static_assert(asl::convertible_to<asl::option<int>, asl::option<ImplicitConversion>>);

static_assert(asl::trivially_copy_constructible<asl::option<int>>);
static_assert(asl::trivially_copy_constructible<asl::option<TrivialType>>);
static_assert(asl::trivially_copy_constructible<asl::option<TrivialTypeDefaultValue>>);
static_assert(!asl::trivially_copy_constructible<asl::option<Copyable>>);

static_assert(asl::trivially_move_constructible<asl::option<int>>);
static_assert(asl::trivially_move_constructible<asl::option<TrivialType>>);
static_assert(asl::trivially_move_constructible<asl::option<TrivialTypeDefaultValue>>);
static_assert(!asl::trivially_move_constructible<asl::option<MoveableOnly>>);

static_assert(asl::trivially_copy_assignable<asl::option<int>>);
static_assert(asl::trivially_copy_assignable<asl::option<TrivialType>>);
static_assert(asl::trivially_copy_assignable<asl::option<TrivialTypeDefaultValue>>);
static_assert(!asl::trivially_copy_assignable<asl::option<Copyable>>);

static_assert(asl::trivially_move_assignable<asl::option<int>>);
static_assert(asl::trivially_move_assignable<asl::option<TrivialType>>);
static_assert(asl::trivially_move_assignable<asl::option<TrivialTypeDefaultValue>>);
static_assert(!asl::trivially_move_assignable<asl::option<MoveableOnly>>);

ASL_TEST(make_null)
{
    const asl::option<int> a;
    const asl::option<int> b = asl::nullopt;

    ASL_TEST_EXPECT(!a.has_value());
    ASL_TEST_EXPECT(!b.has_value());
}

ASL_TEST(make_value)
{
    const asl::option<int> a = 48;

    ASL_TEST_EXPECT(a.has_value());
}

ASL_TEST(reset)
{
    asl::option<int> b = 48;
    ASL_TEST_EXPECT(b.has_value());

    b.reset();
    ASL_TEST_EXPECT(!b.has_value());
}

ASL_TEST(call_destructor)
{
    bool destroyed = false;

    {
        DestructorObserver obs(&destroyed);

        asl::option<DestructorObserver> opt(std::move(obs));
        ASL_TEST_EXPECT(!destroyed);

        const asl::option<DestructorObserver> opt2 = std::move(opt);
        ASL_TEST_EXPECT(!destroyed);
    }

    ASL_TEST_EXPECT(destroyed);
}

ASL_TEST(call_destructor_on_reset)
{
    bool destroyed = false;

    asl::option<DestructorObserver> opt(&destroyed);
    ASL_TEST_EXPECT(!destroyed);

    opt.reset();
    ASL_TEST_EXPECT(destroyed);
}

ASL_TEST(value)
{
    asl::option<int> a = 1;
    asl::option<int> b = 2;
    asl::option<int> c = a;

    ASL_TEST_EXPECT(a.value() == 1);
    ASL_TEST_EXPECT(b.value() == 2);
    ASL_TEST_EXPECT(c.value() == 1);

    c = b;
    ASL_TEST_EXPECT(c.value() == 2);
}

ASL_TEST(value_move)
{
    bool destroyed = false;

    asl::option<DestructorObserver> opt(&destroyed);
    ASL_TEST_EXPECT(!destroyed);

    {
        auto x = std::move(opt).value();
        ASL_TEST_EXPECT(!destroyed);
    }

    ASL_TEST_EXPECT(destroyed);
}

ASL_TEST(deduction_guide)
{
    asl::option opt(45);
    ASL_TEST_EXPECT(opt.value() == 45);
}

ASL_TEST(convert_copy)
{
    asl::option<uint8_t> opt8 = uint8_t{8};
    asl::option<uint16_t> opt16 = opt8;

    ASL_TEST_ASSERT(opt16.has_value());
    ASL_TEST_EXPECT(opt16.value() == 8);

    opt8 = uint8_t{10};
    ASL_TEST_ASSERT(opt8.has_value());
    ASL_TEST_EXPECT(opt8.value() == 10);

    opt16 = asl::nullopt;
    ASL_TEST_EXPECT(!opt16.has_value());

    opt16 = opt8;
    ASL_TEST_ASSERT(opt16.has_value());
    ASL_TEST_EXPECT(opt16.value() == 10);
}

ASL_TEST(convert_move)
{
    asl::option<uint8_t> opt8 = uint8_t{8};
    asl::option<uint16_t> opt16 = std::move(opt8);

    ASL_TEST_ASSERT(opt16.has_value());
    ASL_TEST_EXPECT(opt16.value() == 8);

    opt8 = uint8_t{10};
    ASL_TEST_ASSERT(opt8.has_value());
    ASL_TEST_EXPECT(opt8.value() == 10);

    opt16 = asl::nullopt;
    ASL_TEST_EXPECT(!opt16.has_value());

    opt16 = std::move(opt8);
    ASL_TEST_ASSERT(opt16.has_value());
    ASL_TEST_EXPECT(opt16.value() == 10);
}

ASL_TEST(value_or)
{
    const asl::option<int> a = asl::nullopt;
    const asl::option<int> b = 2;

    ASL_TEST_EXPECT(a.value_or(5) == 5);
    ASL_TEST_EXPECT(b.value_or(5) == 2);
}

ASL_TEST(emplace)
{
    asl::option<int> a = asl::nullopt;

    a.emplace(42);
    ASL_TEST_ASSERT(a.has_value());
    ASL_TEST_EXPECT(a.value() == 42);
}

ASL_TEST(emplace_destroys_previous)
{
    bool b1 = false;
    bool b2 = false;

    {
        asl::option<DestructorObserver> a{&b1};
        ASL_TEST_EXPECT(!b1);

        a.emplace(&b2);
        ASL_TEST_EXPECT(b1);
        ASL_TEST_EXPECT(!b2);
    }

    ASL_TEST_EXPECT(b2);
}

ASL_TEST(and_then)
{
    asl::option<int> a = 5;
    asl::option<int> b;

    auto fn = [](int x) -> asl::option<float> { return static_cast<float>(x) + 0.5F; };

    auto a2 = a.and_then(fn);
    static_assert(asl::same_as<decltype(a2), asl::option<float>>);
    ASL_TEST_ASSERT(a2.has_value());
    ASL_TEST_EXPECT(a2.value() == 5.5F);

    auto b2 = b.and_then(fn);
    static_assert(asl::same_as<decltype(b2), asl::option<float>>);
    ASL_TEST_ASSERT(!b2.has_value());
}

ASL_TEST(transform)
{
    asl::option<int> a = 5;
    asl::option<int> b;

    auto fn = [](int x) -> float { return static_cast<float>(x) + 0.5F; };

    auto a2 = a.transform(fn);
    static_assert(asl::same_as<decltype(a2), asl::option<float>>);
    ASL_TEST_ASSERT(a2.has_value());
    ASL_TEST_EXPECT(a2.value() == 5.5F);

    auto b2 = b.transform(fn);
    static_assert(asl::same_as<decltype(b2), asl::option<float>>);
    ASL_TEST_ASSERT(!b2.has_value());
}

ASL_TEST(or_else)
{
    const asl::option<int> a = 5;
    const asl::option<int> b;

    auto fn = []() -> asl::option<int> { return 12; };

    auto a2 = a.or_else(fn);
    static_assert(asl::same_as<decltype(a2), asl::option<int>>);
    ASL_TEST_ASSERT(a2.has_value());
    ASL_TEST_EXPECT(a2.value() == 5);

    auto b2 = b.or_else(fn);
    static_assert(asl::same_as<decltype(b2), asl::option<int>>);
    ASL_TEST_ASSERT(b2.has_value());
    ASL_TEST_EXPECT(b2.value() == 12);
}

ASL_TEST(niche)
{
    asl::option<NonZero> opt;
    ASL_TEST_EXPECT(!opt.has_value());

    asl::option<NonZero> opt2(2);
    ASL_TEST_EXPECT(opt2.has_value());
    ASL_TEST_EXPECT(opt2.value().value == 2);

    opt = opt2;
    ASL_TEST_EXPECT(opt2.has_value());
    ASL_TEST_EXPECT(opt2.value().value == 2);

    opt.reset();
    ASL_TEST_EXPECT(!opt.has_value());

    opt = NonZero(87);
    ASL_TEST_EXPECT(opt.has_value());
    ASL_TEST_EXPECT(opt.value().value == 87);
}
