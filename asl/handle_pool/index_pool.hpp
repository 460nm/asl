// Copyright 2025 Steven Le Rouzic
//
// SPDX-License-Identifier: BSD-3-Clause

#include "asl/base/integers.hpp"
#include "asl/base/meta.hpp"
#include "asl/containers/chunked_buffer.hpp"
#include "asl/memory/allocator.hpp"

namespace asl
{

// @Todo Uniquely represented for the handle?

template<
    int kIndexSize_,
    int kGenSize_,
    int kUserSize_ = 0
>
requires (
    kUserSize_ >= 0
    && kGenSize_ > 0
    && kIndexSize_ > 0
    && (kUserSize_ + kGenSize_ + kIndexSize_ <= 63)
)
struct index_pool_config
{
    static constexpr int kUserSize  = kUserSize_;
    static constexpr int kGenSize   = kGenSize_;
    static constexpr int kIndexSize = kIndexSize_;

    static constexpr bool kHasUser  = kUserSize > 0;

    using handle_type = smallest_unsigned_integer_type_for_width<kGenSize + kIndexSize + kUserSize>;
    using user_type   = select_t<kHasUser, smallest_unsigned_integer_type_for_width<kUserSize>, empty>;
};


template<
    int kIndexSize_,
    int kGenSize_,
    typename UserType = empty,
    int kUserSize_ = 0
>
requires (
    same_as<UserType, empty> || (kUserSize_ <= size_of<UserType> * 8 && trivially_copy_constructible<UserType>)
)
class index_pool_handle
{
    static constexpr int kUserSizeComputed =
        same_as<UserType, empty> ? 0 : (kUserSize_ == 0 ? size_of<UserType> * 8 : kUserSize_); // NOLINT

public:
    using config             = index_pool_config<kIndexSize_, kGenSize_, kUserSizeComputed>;
    using user_type_external = UserType;

private:
    using handle_type        = config::handle_type;
    using user_type          = config::user_type;

    static constexpr handle_type kHasValueMask = ~(~handle_type{0} >> 1);

    static constexpr handle_type kIndexMask = (handle_type{1} << config::kIndexSize) - 1;

    static constexpr int         kGenShift = config::kIndexSize;
    static constexpr handle_type kGenMask  = ((handle_type{1} << config::kGenSize) - 1) << kGenShift;

    static constexpr int         kUserShift = config::kIndexSize + config::kGenSize;
    static constexpr handle_type kUserMask  = ((handle_type{1} << config::kUserSize) - 1) << kUserShift;
                                        
    handle_type m_handle{};

    [[nodiscard]] constexpr handle_type index(this index_pool_handle self)
    {
        return self.m_handle & kIndexMask;
    }

    [[nodiscard]] constexpr handle_type gen(this index_pool_handle self)
    {
        return (self.m_handle & kGenMask) >> kGenShift;
    }

public:
    index_pool_handle() = default;

    [[nodiscard]] constexpr bool has_value(this index_pool_handle self)
    {
        return self.m_handle & kHasValueMask;
    }

    [[nodiscard]] constexpr user_type_external user(this index_pool_handle self)
        requires config::kHasUser
    {
        return bit_cast<user_type_external>(
            static_cast<user_type>((self.m_handle & kUserMask) >> kUserShift)
        );
    }

    constexpr bool operator==(this index_pool_handle self, index_pool_handle other)
    {
        return self.m_handle == other.m_handle;
    }
};

template<
    typename H,
    typename Payload = empty,
    allocator Allocator = DefaultAllocator>
class IndexPool
{
    using config = H::config;

    struct Slot
    {
        H handle;
        ASL_NO_UNIQUE_ADDRESS Payload payload;
    };

    asl::chunked_buffer<Slot, 1024, Allocator> m_slots;

public:
    IndexPool() requires default_constructible<Allocator> = default;

    explicit IndexPool(Allocator allocator) : m_slots{allocator} {}

    H acquire() requires (!H::kHasUser)
    {
    }

    H acquire(H::user_type_external
};

} // namespace asl

