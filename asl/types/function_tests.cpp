#include "asl/testing/testing.hpp"
#include "asl/types/function.hpp"

static_assert(asl::function_detail::can_be_stored_inline<int(*)(int, int, int)>);
static_assert(asl::function_detail::can_be_stored_inline<decltype([](){})>);
static_assert(asl::function_detail::can_be_stored_inline<decltype([]() static {})>);
static_assert(asl::function_detail::can_be_stored_inline<decltype([a = 1ULL, b = 2ULL](){ return a + b; })>); // NOLINT
static_assert(asl::function_detail::can_be_stored_inline<decltype([a = 1ULL, b = 2ULL]() mutable { return a = b++; })>); // NOLINT
static_assert(!asl::function_detail::can_be_stored_inline<decltype([a = 1ULL, b = 2ULL, c = 3ULL](){ return a + b + c; })>); // NOLINT

static int add(int x, int y)
{
    return x + y;
}

ASL_TEST(function_pointer)
{
    const asl::function<int(int, int)> fn = add;
    ASL_TEST_EXPECT(fn(4, 5) == 9);
    ASL_TEST_EXPECT(fn(40, 50) == 90);
}

ASL_TEST(lambda)
{
    const asl::function<int(int, int)> fn = [](int a, int b) {
        return a + b;
    };
    ASL_TEST_EXPECT(fn(4, 5) == 9);
}

ASL_TEST(lambda_static)
{
    const asl::function<int(int, int)> fn = [](int a, int b) static {
        return a + b;
    };
    ASL_TEST_EXPECT(fn(4, 5) == 9);
}

ASL_TEST(lambda_static_state)
{
    const asl::function<int(int)> fn = [state = 0](int b) mutable {
        state += b;
        return state;
    };

    ASL_TEST_EXPECT(fn(1) == 1);
    ASL_TEST_EXPECT(fn(2) == 3);
    ASL_TEST_EXPECT(fn(3) == 6);
    ASL_TEST_EXPECT(fn(4) == 10);
}

ASL_TEST(lambda_state)
{
    int state = 0;
    const asl::function<void(int)> fn = [&state](int x) {
        state += x;
    };

    ASL_TEST_EXPECT(state == 0);

    fn(5);
    ASL_TEST_EXPECT(state == 5);

    fn(4);
    ASL_TEST_EXPECT(state == 9);
}

ASL_TEST(lambda_big_state)
{
    int s0 = 0;
    int s1 = 0;
    int s2 = 0;
    int s3 = 0;

    const asl::function<void(int)> fn = [&](int x) {
        s0 += x;
        s1 += x + 1;
        s2 += x + 2;
        s3 += x + 3;
    };

    ASL_TEST_EXPECT(s0 == 0);
    ASL_TEST_EXPECT(s1 == 0);
    ASL_TEST_EXPECT(s2 == 0);
    ASL_TEST_EXPECT(s3 == 0);

    fn(5);
    ASL_TEST_EXPECT(s0 == 5);
    ASL_TEST_EXPECT(s1 == 6);
    ASL_TEST_EXPECT(s2 == 7);
    ASL_TEST_EXPECT(s3 == 8);

    fn(4);
    ASL_TEST_EXPECT(s0 == 9);
    ASL_TEST_EXPECT(s1 == 11);
    ASL_TEST_EXPECT(s2 == 13);
    ASL_TEST_EXPECT(s3 == 15);
}

struct Functor
{
    int state{};

    int operator()(int x)
    {
        state += x;
        return state;
    }
};

ASL_TEST(functor)
{
    const asl::function<int(int)> fn = Functor{};

    ASL_TEST_EXPECT(fn(1) == 1);
    ASL_TEST_EXPECT(fn(2) == 3);
    ASL_TEST_EXPECT(fn(3) == 6);
    ASL_TEST_EXPECT(fn(4) == 10);
}

ASL_TEST(copy_move_construct_small)
{
    asl::function<int(int, int)> fn = [x = 0](int a, int b) mutable { return x++ + a + b; };
    ASL_TEST_EXPECT(fn(1, 3) == 4);

    asl::function<int(int, int)> fn2 = fn;
    ASL_TEST_EXPECT(fn(1, 3) == 5);
    ASL_TEST_EXPECT(fn2(5, 3) == 9);

    asl::function<int(int, int)> fn3 = std::move(fn2);
    ASL_TEST_EXPECT(fn(1, 3) == 6);
    ASL_TEST_EXPECT(fn3(5, 3) == 10);

    fn2 = fn;
    ASL_TEST_EXPECT(fn(1, 3) == 7);
    ASL_TEST_EXPECT(fn2(5, 3) == 11);
    ASL_TEST_EXPECT(fn3(5, 3) == 11);

    fn3 = std::move(fn);
    ASL_TEST_EXPECT(fn2(5, 3) == 12);
    ASL_TEST_EXPECT(fn3(5, 3) == 12);
}

ASL_TEST(copy_move_construct_big)
{
    const int64_t v1 = 1;
    const int64_t v2 = 2;
    const int64_t v3 = 3;
    const int64_t v4 = 4;

    asl::function<int64_t(int)> fn = [=](int x) { return v1 + v2 + v3 + v4 + x; };
    ASL_TEST_EXPECT(fn(1) == 11);

    asl::function<int64_t(int)> fn2 = fn;
    ASL_TEST_EXPECT(fn(3) == 13);
    ASL_TEST_EXPECT(fn2(5) == 15);

    asl::function<int64_t(int)> fn3 = std::move(fn2);
    ASL_TEST_EXPECT(fn(1) == 11);
    ASL_TEST_EXPECT(fn3(3) == 13);

    fn2 = fn;
    ASL_TEST_EXPECT(fn(1) == 11);
    ASL_TEST_EXPECT(fn2(5) == 15);
    ASL_TEST_EXPECT(fn3(3) == 13);

    fn3 = std::move(fn);
    ASL_TEST_EXPECT(fn2(5) == 15);
    ASL_TEST_EXPECT(fn3(3) == 13);
}

ASL_TEST(replace)
{
    asl::function<int(int)> fn = [](int x) { return x + 1; };
    ASL_TEST_EXPECT(fn(5) == 6);

    fn = [](int x) { return x + 3; };
    ASL_TEST_EXPECT(fn(5) == 8);
}

static int foo(const asl::function<int(int, int)>& fn)
{
    return fn(5, 5);
}

ASL_TEST(function_parameter)
{
    ASL_TEST_EXPECT(foo(add) == 10);
    ASL_TEST_EXPECT(foo([](int a, int b) { return a + b; }) == 10);
}

