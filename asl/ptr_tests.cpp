#include "asl/ptr.hpp"

static_assert(asl::ptr_metadata<asl::ptr_internal::metadata<void>>);
static_assert(asl::ptr_metadata<asl::ptr_internal::metadata<int[]>>);
static_assert(asl::ptr_metadata<asl::ptr_internal::metadata<int[56]>>);
static_assert(asl::ptr_metadata<asl::ptr_internal::metadata<int>>);
static_assert(asl::ptr_metadata<asl::ptr_internal::metadata<int()>>);
static_assert(asl::ptr_metadata<asl::ptr_internal::metadata<int(int) const &>>);
static_assert(asl::ptr_metadata<asl::ptr_internal::metadata<const int&>>);
static_assert(asl::ptr_metadata<asl::ptr_internal::metadata<int&&>>);

static_assert(sizeof(asl::ptr<int>) == sizeof(int*));
static_assert(sizeof(asl::ptr<int[]>) == sizeof(int*) * 2);
static_assert(sizeof(asl::ptr<int[67]>) == sizeof(int*));

int main() { return 0; }
