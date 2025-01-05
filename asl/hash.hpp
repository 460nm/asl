#pragma once

#include "asl/integers.hpp"

namespace asl::city_hash
{

// All CityHash stuff below this point

// Hash function for a byte array.
uint64_t CityHash64(const char *s, size_t len);

// Hash function for a byte array.  For convenience, a 64-bit seed is also
// hashed into the result.
uint64_t CityHash64WithSeed(const char *s, size_t len, uint64_t seed);

// Hash function for a byte array.  For convenience, two seeds are also
// hashed into the result.
uint64_t CityHash64WithSeeds(const char *s, size_t len,
                           uint64_t seed0, uint64_t seed1);

// Hash function for a byte array.
uint128_t CityHash128(const char *s, size_t len);

// Hash function for a byte array.  For convenience, a 128-bit seed is also
// hashed into the result.
uint128_t CityHash128WithSeed(const char *s, size_t len, uint128_t seed);

// Hash function for a byte array.  Most useful in 32-bit binaries.
uint32_t CityHash32(const char *s, size_t len);

// Hash 128 input bits down to 64 bits of output.
// This is intended to be a reasonably good hash function.
constexpr uint64_t Hash128to64(uint64_t high, uint64_t low)
{
  // Murmur-inspired hashing.
  const uint64_t kMul = 0x9ddfea08eb382d69ULL;
  uint64_t a = (low ^ high) * kMul;
  a ^= (a >> 47);
  uint64_t b = (high ^ a) * kMul;
  b ^= (b >> 47);
  b *= kMul;
  return b;
}

// Hash 128 input bits down to 64 bits of output.
// This is intended to be a reasonably good hash function.
constexpr uint64_t Hash128to64(const uint128_t& x)
{
  return Hash128to64(x.high, x.low);
}

} // namespace asl::city_hash
