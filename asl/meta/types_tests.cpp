#include "asl/meta/types.hpp"

using namespace asl;

static_assert(is_referenceable<int>);
static_assert(is_referenceable<int&>);
static_assert(is_referenceable<int&&>);
static_assert(!is_referenceable<void>);
static_assert(is_referenceable<void*>);
static_assert(is_referenceable<void()>);
static_assert(is_referenceable<void(*)()>);
static_assert(!is_referenceable<void() const>);
static_assert(!is_referenceable<void() &>);
static_assert(!is_referenceable<void() const &&>);

int main() { return 0; }
