#include "asl/box.hpp"

#include "asl/testing/testing.hpp"
#include "asl/tests/test_types.hpp"

static_assert(sizeof(asl::box<int>) == sizeof(int*));
static_assert(asl::default_constructible<asl::box<int>>);
static_assert(!asl::copyable<asl::box<int>>);
static_assert(asl::moveable<asl::box<int>>);
static_assert(asl::default_constructible<asl::box<NonMoveConstructible>>);

ASL_TEST(destructor)
{
    bool d = false;

    {
        asl::box<DestructorObserver> box2;
    
        {
            auto box = asl::make_box<DestructorObserver>(&d);
            ASL_TEST_ASSERT(!d);
    

            auto box3 = ASL_MOVE(box);
            ASL_TEST_ASSERT(!d);

            box2 = ASL_MOVE(box3);
            ASL_TEST_ASSERT(!d);
        }
    
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
