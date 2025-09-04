// Copyright 2025 Steven Le Rouzic
//
// SPDX-License-Identifier: BSD-3-Clause

#include "asl/types/box.hpp"
#include "asl/types/option.hpp"

#include "asl/testing/testing.hpp"
#include "asl/tests/types.hpp"

static_assert(sizeof(asl::box<int>) == sizeof(int*));
static_assert(!asl::copyable<asl::box<int>>);
static_assert(asl::movable<asl::box<int>>);
static_assert(asl::has_niche<asl::box<int>>);
static_assert(sizeof(asl::option<asl::box<int>>) == sizeof(int*));

ASL_TEST(destructor)
{
    bool d = false;

    {
        auto box = asl::make_box<DestructorObserver>(&d);
        ASL_TEST_ASSERT(!d);


        auto box3 = std::move(box);
        ASL_TEST_ASSERT(!d);
    }

    ASL_TEST_ASSERT(d);
}

ASL_TEST(value)
{
    auto b = asl::make_box<int>(24);
    ASL_TEST_EXPECT(*b == 24);

    auto b2 = std::move(b);
    ASL_TEST_EXPECT(*b2 == 24);
}

ASL_TEST(ptr)
{
    auto b = asl::make_box<int>(24);
    auto* ptr1 = b.get();

    auto b2 = std::move(b);
    auto* ptr2 = b2.get();
    ASL_TEST_EXPECT(ptr1 == ptr2);
}

struct Struct { int a; };

ASL_TEST(arrow)
{
    auto b = asl::make_box<Struct>(45);
    ASL_TEST_EXPECT(b->a == 45);
}

ASL_TEST(niche)
{
    static_assert(sizeof(asl::box<int>) == sizeof(asl::option<asl::box<int>>));

    asl::option<asl::box<int>> opt;
    ASL_TEST_EXPECT(!opt.has_value());

    opt = asl::make_box<int>(66);
    ASL_TEST_EXPECT(opt.has_value());
    ASL_TEST_EXPECT(*opt.value() == 66);

    opt = asl::nullopt;
    ASL_TEST_EXPECT(!opt.has_value());

    bool destroyed = false;
    asl::option opt2 = asl::make_box<DestructorObserver>(&destroyed);
    ASL_TEST_EXPECT(opt2.has_value());
    ASL_TEST_EXPECT(!destroyed);

    opt2.reset();
    ASL_TEST_EXPECT(!opt2.has_value());
    ASL_TEST_EXPECT(destroyed);
}

class Base // NOLINT
{
public:
    virtual ~Base() = default;
    virtual int number() { return 1; }
};

class Derived : public Base
{
public:
    int number() override { return 2; }
};

static_assert(asl::convertible_to<asl::box<Derived>, asl::box<Base>>);
static_assert(asl::convertible_to<asl::box<Base>, asl::box<Base>>);
static_assert(!asl::convertible_to<asl::box<Base>, asl::box<Derived>>);
static_assert(!asl::convertible_to<asl::box<float>, asl::box<int>>);

ASL_TEST(derived)
{
    asl::box<Base> obj = asl::make_box<Derived>();
    ASL_TEST_ASSERT(obj->number() == 2);
}

static_assert(asl::derefs_as<asl::box<int>, int>);
static_assert(asl::derefs_as<asl::box<Derived>, Base>);
static_assert(asl::derefs_as<asl::box<Derived>, Derived>);
static_assert(asl::derefs_as<asl::box<Base>, Base>);

static void wants_int(int) {}
static void wants_base(const Base&) {}
static void wants_base_ptr(Base*) {}

ASL_TEST(deref)
{
    auto b = asl::make_box<int>(5);

    wants_int(asl::deref<int>(b));

    auto d = asl::make_box<Derived>();

    wants_base(asl::deref<Base>(d));
    wants_base_ptr(&asl::deref<Base>(d));
    wants_base(asl::deref<Base>(std::move(d)));
}

