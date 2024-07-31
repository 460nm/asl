#include "asl/meta/types.hpp"

using namespace asl;

static_assert(same<int, int>);
static_assert(same<void, void>);
static_assert(!same<const float, float>);
static_assert(!same<int, float>);

int main() { return 0; }
