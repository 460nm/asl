#include "asl/testing/testing.hpp"
#include "asl/tests/types.hpp"
#include "asl/containers/chunked_buffer.hpp"

static_assert(asl::moveable<asl::chunked_buffer<int, 8>>);
static_assert(asl::moveable<asl::chunked_buffer<Copyable, 8>>);
static_assert(asl::moveable<asl::chunked_buffer<MoveableOnly, 8>>);
static_assert(asl::moveable<asl::chunked_buffer<Pinned, 8>>);

static_assert(asl::copyable<asl::chunked_buffer<int, 8>>);
static_assert(asl::copyable<asl::chunked_buffer<Copyable, 8>>);
static_assert(!asl::copyable<asl::chunked_buffer<MoveableOnly, 8>>);
static_assert(!asl::copyable<asl::chunked_buffer<Pinned, 8>>);
