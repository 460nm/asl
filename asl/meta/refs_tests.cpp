#include "asl/meta/refs.hpp"
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

static_assert(!is_ref<int>);
static_assert(!is_rref<int>);
static_assert(!is_any_ref<int>);

static_assert(is_ref<int&>);
static_assert(!is_rref<int&>);
static_assert(is_any_ref<int&>);

static_assert(!is_ref<int&&>);
static_assert(is_rref<int&&>);
static_assert(is_any_ref<int&&>);

static_assert(!is_any_ref<void()>);
static_assert(!is_any_ref<void() const>);
static_assert(!is_any_ref<void() &>);

int main() { return 0; }
