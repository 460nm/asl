#include "asl/option.hpp"
#include "asl/tests/test_types.hpp"
#include "asl/testing/testing.hpp"

class Base {};
class Derived : public Base {};

static_assert(!asl::is_option<int>);
static_assert(asl::is_option<asl::option<int>>);
static_assert(asl::is_option<const asl::option<int>>);

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
static_assert(asl::move_assignable<asl::option<MoveAssignable>>);
static_assert(!asl::move_assignable<asl::option<NonMoveAssignable>>);

static_assert(asl::assignable<asl::option<Base*>&, asl::option<Derived*>>);
static_assert(!asl::assignable<asl::option<Derived*>&, asl::option<Base*>>);

static_assert(!asl::convertible<asl::option<Base*>, asl::option<Derived*>>);
static_assert(asl::convertible<asl::option<Derived*>, asl::option<Base*>>);

class ExplicitConversion { public: explicit ExplicitConversion(int) {} };
class ImplicitConversion { public: ImplicitConversion(int) {} }; // NOLINT

static_assert(!asl::convertible<int, ExplicitConversion>);
static_assert(asl::convertible<int, ImplicitConversion>);

static_assert(!asl::convertible<int, asl::option<ExplicitConversion>>);
static_assert(asl::convertible<int, asl::option<ImplicitConversion>>);

static_assert(!asl::convertible<asl::option<int>, asl::option<ExplicitConversion>>);
static_assert(asl::convertible<asl::option<int>, asl::option<ImplicitConversion>>);

static_assert(asl::trivially_copy_constructible<asl::option<int>>);
static_assert(!asl::trivially_copy_constructible<asl::option<CopyConstructible>>);

static_assert(asl::trivially_move_constructible<asl::option<int>>);
static_assert(!asl::trivially_move_constructible<asl::option<MoveConstructible>>);

static_assert(asl::trivially_copy_assignable<asl::option<int>>);
static_assert(!asl::trivially_copy_assignable<asl::option<CopyAssignable>>);

static_assert(asl::trivially_move_assignable<asl::option<int>>);
static_assert(!asl::trivially_move_assignable<asl::option<MoveAssignable>>);

ASL_TEST(make_null)
{
    asl::option<int> a;
    asl::option<int> b = asl::nullopt;

    ASL_TEST_EXPECT(!a.has_value());
    ASL_TEST_EXPECT(!b.has_value());
}

ASL_TEST(make_value)
{
    asl::option<int> a = 48;

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
        
        asl::option<DestructorObserver> opt(ASL_MOVE(obs));
        ASL_TEST_EXPECT(!destroyed);
    
        asl::option<DestructorObserver> opt2 = ASL_MOVE(opt);
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
        auto x = ASL_MOVE(opt).value();
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
    
    ASL_TEST_EXPECT(opt16.has_value());
    ASL_TEST_EXPECT(opt16.value() == 8);

    opt8 = uint8_t{10};
    ASL_TEST_EXPECT(opt8.has_value());
    ASL_TEST_EXPECT(opt8.value() == 10);
    
    opt16 = asl::nullopt;
    ASL_TEST_EXPECT(!opt16.has_value());
    
    opt16 = opt8;
    ASL_TEST_EXPECT(opt16.has_value());
    ASL_TEST_EXPECT(opt16.value() == 10);
}

ASL_TEST(convert_move)
{
    asl::option<uint8_t> opt8 = uint8_t{8};
    asl::option<uint16_t> opt16 = ASL_MOVE(opt8);
    
    ASL_TEST_EXPECT(opt16.has_value());
    ASL_TEST_EXPECT(opt16.value() == 8);

    opt8 = ASL_MOVE(uint8_t{10});
    ASL_TEST_EXPECT(opt8.has_value());
    ASL_TEST_EXPECT(opt8.value() == 10);
    
    opt16 = asl::nullopt;
    ASL_TEST_EXPECT(!opt16.has_value());
    
    opt16 = ASL_MOVE(opt8);
    ASL_TEST_EXPECT(opt16.has_value());
    ASL_TEST_EXPECT(opt16.value() == 10);
}
