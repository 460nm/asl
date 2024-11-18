#include "asl/box.hpp"

#include "asl/testing/testing.hpp"

static_assert(sizeof(asl::box<int>) == sizeof(int*));

