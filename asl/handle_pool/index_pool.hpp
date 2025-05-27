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
    int kIndexBits_,
    int kGenBits_,
    typename UserType_ = empty,
    int kUserBits_ = 0
>
requires (kIndexBits_ > 0 && kGenBits_ > 0 && kUserBits_ >= 0)
struct index_pool_config
{
    static constexpr bool kHasUser = !same_as<UserType_, empty>;

    using UserType          = UserType_;
    using PrimitiveUserType = smallest_unsigned_integer_type_for_width<size_of<UserType> * 8>;

    static_assert(trivially_copy_constructible<UserType>);
    static_assert(size_of<UserType> == size_of<PrimitiveUserType>, "UserType should be of size 1, 2 or 4");

    static constexpr int kUserBits = []() static -> int {
        if constexpr (!kHasUser) { return 0; };
        return kUserBits_ == 0 ? size_of<UserType> * 8 : kUserBits_;
    }();

    static_assert(kUserBits <= size_of<UserType> * 8);

    static constexpr int kIndexBits = kIndexBits_;
    static constexpr int kGenBits   = kGenBits_;

    static_assert(kIndexBits + kGenBits + kUserBits <= 63);

    using HandleType = smallest_unsigned_integer_type_for_width<kIndexBits + kGenBits + kUserBits + 1>;

    static constexpr int kGenShift  = kIndexBits;
    static constexpr int kUserShift = kIndexBits + kGenBits;

    static constexpr HandleType kValidMask = HandleType{1} << (size_of<HandleType> * 8 - 1);
    static constexpr HandleType kIndexMask = (HandleType{1} << kIndexBits) - 1;
    static constexpr HandleType kGenMask   = ((HandleType{1} << kGenBits) - 1) << kGenShift;
    static constexpr HandleType kUserMask  = ((HandleType{1} << kUserBits) - 1) << kUserShift;
};

} // namespace asl

