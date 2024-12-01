#include <asl/buffer.hpp>

#include <asl/testing/testing.hpp>

static_assert(asl::buffer<int32_t>::kInlineCapacity == 5);
static_assert(asl::buffer<int64_t>::kInlineCapacity == 2);
static_assert(asl::buffer<char>::kInlineCapacity == 23);

