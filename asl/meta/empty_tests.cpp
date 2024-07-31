#include "asl/meta/empty.hpp"

using namespace asl;

struct Empty {};

static_assert(is_void<void>);
static_assert(is_void<const void>);
static_assert(is_void<volatile void>);
static_assert(is_void<const void>);
static_assert(!is_void<Empty>);
static_assert(!is_void<int>);
static_assert(!is_void<int&>);
static_assert(!is_void<int()>);


int main() { return 0; }
