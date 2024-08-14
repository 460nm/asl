#include "asl/object.hpp"

static_assert(asl::is_object<asl::object<void>>);
static_assert(asl::is_object<asl::object<int>>);
static_assert(asl::is_object<asl::object<int&>>);
static_assert(asl::is_object<asl::object<int()>>);

static_assert(asl::is_empty<asl::object<void>>);
static_assert(sizeof(asl::object<int>) == 4);
static_assert(sizeof(asl::object<int*>) == sizeof(void*));
static_assert(sizeof(asl::object<int&>) == sizeof(void*));
static_assert(sizeof(asl::object<int()>) == sizeof(void*));

int main() { return 0; }
