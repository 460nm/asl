#include "asl/testing/testing.hpp"
#include "asl/types/function_ref.hpp"

static int add(int a, int b)
{
    return a + b;
}

struct Functor
{
    int state = 0;

    int operator()(int x, int)
    {
        state += x;
        return state;
    }
};

static int invoke_fn_ref(asl::function_ref<int(int, int)> fn, int a, int b)
{
    return fn(a, b);
}

ASL_TEST(function_ref)
{
    const asl::function_ref<int(int, int)> fn(add);
    ASL_TEST_EXPECT(invoke_fn_ref(fn, 4, 5) == 9);

    ASL_TEST_EXPECT(invoke_fn_ref(add, 4, 5) == 9);

    ASL_TEST_EXPECT(invoke_fn_ref([](int a, int b) { return a * b; }, 4, 5) == 20);

    Functor fun;
    ASL_TEST_EXPECT(invoke_fn_ref(fun, 4, 5) == 4);
    ASL_TEST_EXPECT(invoke_fn_ref(fun, 4, 5) == 8);
    ASL_TEST_EXPECT(invoke_fn_ref(fun, 4, 5) == 12);

    asl::function_ref<int(int, int)> fn2 = fn;
    ASL_TEST_EXPECT(invoke_fn_ref(fn2, 4, 5) == 9);

    fn2 = [](int a, int b) { return a - b; };
    ASL_TEST_EXPECT(invoke_fn_ref(fn2, 4, 5) == -1);

    fn2 = fn;
    ASL_TEST_EXPECT(invoke_fn_ref(fn2, 4, 5) == 9);

    fn2 = add;
    ASL_TEST_EXPECT(invoke_fn_ref(fn2, 4, 5) == 9);
}
