#include "asl/meta/quals.hpp"
#include "asl/meta/types.hpp"

using namespace asl;

static_assert(!is_const<int>);
static_assert(is_const<const int>);
static_assert(!is_const<volatile int>);
static_assert(is_const<const volatile int>);

static_assert(!is_volatile<int>);
static_assert(!is_volatile<const int>);
static_assert(is_volatile<volatile int>);
static_assert(is_volatile<const volatile int>);

static_assert(!is_const<const int*>);
static_assert(is_const<const int* const>);

static_assert(is_const<const int&>);
static_assert(is_const<int&>);

static_assert(is_const<void()>);
static_assert(is_const<void() const>);
static_assert(is_const<void() const &>);

static_assert(!is_volatile<int&>);
static_assert(!is_volatile<void()>);

static_assert(same<as_const_t<int>, const int>);
static_assert(same<as_const_t<const int>, const int>);
static_assert(same<un_const_t<int>, int>);
static_assert(same<un_const_t<const int>, int>);

static_assert(same<as_const_t<int[]>, const int[]>);
static_assert(same<as_const_t<int[5]>, const int[5]>);

static_assert(same<as_volatile_t<int>, volatile int>);
static_assert(same<as_volatile_t<volatile int>, volatile int>);
static_assert(same<un_volatile_t<int>, int>);
static_assert(same<un_volatile_t<volatile int>, int>);

static_assert(same<as_volatile_t<int[]>, volatile int[]>);
static_assert(same<as_volatile_t<int[5]>, volatile int[5]>);

static_assert(same<un_qual_t<int>, int>);
static_assert(same<un_qual_t<const int>, int>);
static_assert(same<un_qual_t<volatile int>, int>);
static_assert(same<un_qual_t<const volatile int>, int>);
static_assert(same<un_qual_t<volatile const int>, int>);

int main() { return 0; }
