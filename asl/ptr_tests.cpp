#include "asl/ptr.hpp"

using namespace asl;

static_assert(ptr_metadata<ptr_internal::metadata<void>>);
static_assert(ptr_metadata<ptr_internal::metadata<int[]>>);
static_assert(ptr_metadata<ptr_internal::metadata<int[56]>>);
static_assert(ptr_metadata<ptr_internal::metadata<int>>);
static_assert(ptr_metadata<ptr_internal::metadata<int()>>);
static_assert(ptr_metadata<ptr_internal::metadata<int(int) const &>>);
static_assert(ptr_metadata<ptr_internal::metadata<const int&>>);
static_assert(ptr_metadata<ptr_internal::metadata<int&&>>);

int main() { return 0; }
