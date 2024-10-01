#include "asl/maybe_uninit.hpp"
#include "asl/test_types.hpp"

static_assert(asl::layout::of<int>() == asl::layout::of<asl::maybe_uninit<int>>());
static_assert(asl::size_of<int> == asl::size_of<asl::maybe_uninit<int>>);
static_assert(asl::align_of<int> == asl::align_of<asl::maybe_uninit<int>>);

static_assert(asl::trivially_destructible<asl::maybe_uninit<TriviallyDestructible>>);
static_assert(!asl::trivially_destructible<asl::maybe_uninit<HasDestructor>>);

int main()
{
    return 0;
}
