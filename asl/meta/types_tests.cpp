#include "asl/meta/types.hpp"

using namespace asl;

static_assert(referenceable<int>);
static_assert(referenceable<int&>);
static_assert(referenceable<int&&>);
static_assert(!referenceable<void>);
static_assert(referenceable<void*>);
static_assert(referenceable<void()>);
static_assert(referenceable<void(*)()>);
static_assert(!referenceable<void() const>);
static_assert(!referenceable<void() &>);
static_assert(!referenceable<void() const &&>);

int main() { return 0; }
