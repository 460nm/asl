#include "asl/meta/funcs.hpp"

using namespace asl;

static_assert(!is_func<int>);
static_assert(!is_func<int&>);
static_assert(!is_func<const int>);
static_assert(!is_func<void>);
static_assert(is_func<void()>);
static_assert(is_func<void() const>);
static_assert(is_func<void() &&>);
static_assert(!is_func<void(*)()>);

int main() { return 0; }
