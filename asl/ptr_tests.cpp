#include "asl/ptr.hpp"

static_assert(sizeof(asl::ptr<int>) == sizeof(int*));
static_assert(sizeof(asl::ptr<void>) == sizeof(void*));
static_assert(sizeof(asl::ptr<int[]>) == 2 * sizeof(int*));
static_assert(sizeof(asl::ptr<int[25]>) == sizeof(int*));
static_assert(sizeof(asl::ptr<int*>) == sizeof(int**));
static_assert(sizeof(asl::ptr<void(*)()>) == sizeof(void*));
static_assert(sizeof(asl::ptr<void()>) == sizeof(void*));
static_assert(sizeof(asl::ptr<int&>) == sizeof(int*));

int main() { return 0; }
