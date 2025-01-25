#include "asl/box.hpp"
#include "asl/option.hpp"

#include "asl/testing/testing.hpp"
#include "asl/tests/test_types.hpp"

static_assert(sizeof(asl::box<int>) == sizeof(int*));
static_assert(!asl::copyable<asl::box<int>>);
static_assert(asl::moveable<asl::box<int>>);
static_assert(asl::has_niche<asl::box<int>>);
static_assert(sizeof(asl::option<asl::box<int>>) == sizeof(int*));

ASL_TEST(destructor)
{
    bool d = false;

    {
        auto box = asl::make_box<DestructorObserver>(&d);
        ASL_TEST_ASSERT(!d);


        auto box3 = ASL_MOVE(box);
        ASL_TEST_ASSERT(!d);
    }

    ASL_TEST_ASSERT(d);
}

ASL_TEST(value)
{
    auto b = asl::make_box<int>(24);
    ASL_TEST_EXPECT(*b == 24);

    auto b2 = ASL_MOVE(b);
    ASL_TEST_EXPECT(*b2 == 24);
}

ASL_TEST(ptr)
{
    auto b = asl::make_box<int>(24);
    auto* ptr1 = b.get();

    auto b2 = ASL_MOVE(b);
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
