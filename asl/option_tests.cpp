#include "asl/option.hpp"
#include "asl/test_types.hpp"

static_assert(asl::trivially_destructible<asl::option<TriviallyDestructible>>);
static_assert(!asl::trivially_destructible<asl::option<HasDestructor>>);

int main()
{
    asl::option<HasDestructor> a;
    asl::option<TriviallyDestructible> b;
    return 0;
}
