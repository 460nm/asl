#pragma once

#include "asl/integers.hpp"
#include "asl/meta.hpp"
#include "asl/span.hpp"

namespace asl::city_hash
{

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

namespace asl
{

template<typename T, typename H>
concept hashable_generic = requires(const T& value, H h)
{
    { AslHashValue(h, value) } -> same_as<H>;
};

struct HashState
{
    uint128_t state{};

    constexpr HashState() = default;
    explicit constexpr HashState(uint128_t s) : state{s} {}

    static HashState combine_bytes(HashState h, span<const byte> bytes)
    {
        auto hashed = city_hash::CityHash128WithSeed(
            reinterpret_cast<const char*>(bytes.data()),
            static_cast<size_t>(bytes.size()),
            h.state);
        return HashState{hashed};
    }

    static constexpr HashState combine(HashState h)
    {
        return h;
    }

    template<hashable_generic<HashState> Arg, hashable_generic<HashState>... Remaining>
    static constexpr HashState combine(HashState h, const Arg& arg, const Remaining&... remaining)
    {
        return combine(AslHashValue(h, arg), remaining...);
    }
};

template<typename T>
concept hashable = hashable_generic<T, HashState>;

template<typename H, uniquely_represented T>
constexpr H AslHashValue(H h, const T& value)
{
    return H::combine_bytes(h, as_bytes(span<const T>{&value, 1}));
}

template<typename H>
constexpr H AslHashValue(H h, bool value)
{
    return AslHashValue(h, value ? 1 : 0);
}

template<hashable T>
constexpr uint64_t hash_value(const T& value)
{
    auto result = AslHashValue(HashState{}, value).state;
    return city_hash::Hash128to64(result);
}

} // namespace asl

