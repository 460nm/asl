#include "asl/functional.hpp"
#include "asl/testing/testing.hpp"

struct HasFunction
{
    void do_something(int, float) {}
};

struct HasMember
{
    int member{};
    int member_array[4]{};
    void (*member_fn)(){};
};

struct Functor
{
    int64_t operator()() { return 35; }
    int operator()(int x) { return x; }
};

static int some_func0() { return 1; }
static int some_func1(int x) { return x + 1; }
static float some_func1(float x) { return x + 1; }
static int some_func2(int x, int b) { return x + b; }

static_assert(asl::is_same<asl::result_of_t<Functor()>, int64_t>);
static_assert(asl::is_same<asl::result_of_t<Functor(int)>, int>);
static_assert(asl::is_same<asl::result_of_t<decltype(static_cast<float(*)(float)>(some_func1))(float)>, float>);
static_assert(asl::is_same<asl::result_of_t<decltype(&HasFunction::do_something)(HasFunction, int, float)>, void>);
static_assert(asl::is_same<asl::result_of_t<decltype(&HasMember::member)(HasMember)>, int>);

ASL_TEST(invoke_member_function)
{
    HasFunction c;
    asl::invoke(&HasFunction::do_something, c, 5, 5.0F);
    asl::invoke(&HasFunction::do_something, &c, 5, 5.0F);
}

ASL_TEST(invoke_member_data)
{
    HasMember c;
    
    asl::invoke(&HasMember::member, c);
    asl::invoke(&HasMember::member_array, c);
    asl::invoke(&HasMember::member_fn, c);
    asl::invoke(&HasMember::member, &c);
    asl::invoke(&HasMember::member_array, &c);
    asl::invoke(&HasMember::member_fn, &c);
}

ASL_TEST(invoke_fn)
{
    ASL_TEST_EXPECT(asl::invoke(some_func0) == 1);
    ASL_TEST_EXPECT(asl::invoke(static_cast<int(*)(int)>(some_func1), 8) == 9);
    ASL_TEST_EXPECT(asl::invoke(some_func2, 4, 8) == 12);
    ASL_TEST_EXPECT(asl::invoke(&some_func0) == 1);
    ASL_TEST_EXPECT(asl::invoke(static_cast<int(*)(int)>(&some_func1), 8) == 9);
    ASL_TEST_EXPECT(asl::invoke(&some_func2, 4, 8) == 12);
}

ASL_TEST(invoke_operator_call)
{
    Functor f;
    ASL_TEST_EXPECT(asl::invoke(f) == 35);
    ASL_TEST_EXPECT(asl::invoke(f, 8) == 8);
}

ASL_TEST(invoke_lambda)
{
    ASL_TEST_EXPECT(asl::invoke([](){ return 35; }) == 35);
    ASL_TEST_EXPECT(asl::invoke([](int x){ return x + 2; }, 6) == 8);
}
