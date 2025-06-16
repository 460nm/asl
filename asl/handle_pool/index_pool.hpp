// Copyright 2025 Steven Le Rouzic
//
// SPDX-License-Identifier: BSD-3-Clause

#include "asl/base/integers.hpp"
#include "asl/base/meta.hpp"
#include "asl/containers/chunked_buffer.hpp"
#include "asl/memory/allocator.hpp"
#include "asl/types/option.hpp"

namespace asl
{

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
    static_assert(trivially_destructible<UserType>);
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

    static constexpr int        kGenShift  = kIndexBits;
    static constexpr int        kUserShift = kIndexBits + kGenBits;

    static constexpr HandleType kValidMask = HandleType{1} << (size_of<HandleType> * 8 - 1);
    static constexpr HandleType kIndexMask = (HandleType{1} << kIndexBits) - 1;
    static constexpr HandleType kGenMask   = ((HandleType{1} << kGenBits) - 1) << kGenShift;
    static constexpr HandleType kUserMask  = ((HandleType{1} << kUserBits) - 1) << kUserShift;
    static constexpr HandleType kNiche     = static_cast<HandleType>(~uint64_t{kValidMask});

    static constexpr uint64_t   kMaxGen    = (uint64_t{1} << kGenBits) - 1;
    static constexpr uint64_t   kMaxIndex  = (uint64_t{1} << kIndexBits) - 1;
};

template<
    int kIndexBits_,
    int kGenBits_,
    typename UserType_ = empty,
    int kUserBits_ = 0
>
class index_pool_handle
{
public:
    using config = index_pool_config<kIndexBits_, kGenBits_, UserType_, kUserBits_>;

private:
    config::HandleType m_handle{};

public:
    constexpr index_pool_handle() = default;

    constexpr explicit index_pool_handle(niche_t)
        : m_handle{config::kNiche}
    {}

    constexpr index_pool_handle(uint64_t index, uint64_t gen)
        requires (!config::kHasUser)
        : m_handle{static_cast<config::HandleType>(
            config::kValidMask |
            (index & config::kIndexMask) |
            ((gen << config::kGenShift) & config::kGenMask))}
    {
        ASL_ASSERT((index & uint64_t{config::kIndexMask}) == index);
        ASL_ASSERT((gen & (uint64_t{config::kGenMask} >> config::kGenShift)) == gen);
    }

    constexpr index_pool_handle(uint64_t index, uint64_t gen, config::UserType user)
        requires config::kHasUser
        : m_handle{static_cast<config::HandleType>(
            config::kValidMask |
            (index & config::kIndexMask) |
            ((gen << config::kGenShift) & config::kGenMask) |
            ((static_cast<config::HandleType>(bit_cast<typename config::PrimitiveUserType>(user)) << config::kUserShift) & config::kUserMask))}
    {
        ASL_ASSERT((index & uint64_t{config::kIndexMask}) == index);
        ASL_ASSERT((gen & (uint64_t{config::kGenMask} >> config::kGenShift)) == gen);
        ASL_ASSERT((bit_cast<typename config::PrimitiveUserType>(user) & (uint64_t{config::kUserMask} >> config::kUserShift)) == bit_cast<typename config::PrimitiveUserType>(user));
    }

    constexpr bool is_null(this index_pool_handle self)
    {
        return !(self.m_handle & config::kValidMask);
    }

    constexpr uint64_t index(this index_pool_handle self)
    {
        return self.m_handle & config::kIndexMask;
    }

    constexpr uint64_t gen(this index_pool_handle self)
    {
        return (self.m_handle & config::kGenMask) >> config::kGenShift;
    }

    constexpr config::UserType user(this index_pool_handle self)
    {
        return bit_cast<typename config::UserType>(static_cast<config::PrimitiveUserType>(
            ((self.m_handle & config::kUserMask) >> config::kUserShift)));
    }

    constexpr bool operator==(this index_pool_handle self, index_pool_handle other) = default;

    constexpr bool operator==(this index_pool_handle self, niche_t)
    {
        return self.m_handle == config::kNiche;
    }
};

template<
    int kIndexBits_,
    int kGenBits_,
    typename UserType_,
    int kUserBits_
>
struct is_uniquely_represented<index_pool_handle<kIndexBits_, kGenBits_, UserType_, kUserBits_>> : true_type {};

template<
    int kIndexBits_,
    int kGenBits_,
    typename UserType_ = empty,
    int kUserBits_ = 0,
    typename Payload = empty,
    allocator Allocator = DefaultAllocator
>
class IndexPool
{
public:
    using handle = index_pool_handle<kIndexBits_, kGenBits_, UserType_, kUserBits_>;

private:
    using config = handle::config;

    using internal_handle = index_pool_handle<
        kIndexBits_,
        kGenBits_,
        typename config::PrimitiveUserType,
        kUserBits_>;

    static constexpr bool kHasPayload = !same_as<Payload, empty>;

    // @Todo Remove need for default constructible & trivially destructible for payload
    // Use maybe_uninit for it

    static_assert(default_constructible<Payload>);
    static_assert(copy_constructible<Payload>);
    static_assert(trivially_destructible<Payload>);

    struct Slot
    {
        bool is_end_of_list : 1;
        bool is_active      : 1;

        internal_handle handle;

        ASL_NO_UNIQUE_ADDRESS Payload payload;
    };

    chunked_buffer<Slot, 256, Allocator> m_slots;

    // We only use the index, this is essentially the head of the linked
    // list to the first available slot.
    // Then the index of each slot points to the next available one.
    internal_handle m_first_available;

    void allocate_new_slot()
    {
        const auto new_index = static_cast<uint64_t>(m_slots.size());
        if (new_index > config::kMaxIndex) { return; }

        const internal_handle new_handle = internal_handle(new_index, 0, 0);

        m_slots.push(Slot{
            .is_end_of_list = true,
            .is_active      = false,
            .handle         = new_handle,
            .payload        = Payload{}
        });

        m_first_available = new_handle;
    }

    option<internal_handle> acquire_handle(const Payload& payload)
    {
        if (m_first_available.is_null())
        {
            allocate_new_slot();
        }

        if (m_first_available.is_null())
        {
            return nullopt;
        }

        auto index = m_first_available.index();

        Slot& slot = m_slots[static_cast<isize_t>(index)];
        ASL_ASSERT(!slot.is_active);

        m_first_available = slot.is_end_of_list ? internal_handle{} : slot.handle;

        slot.is_active = true;
        slot.payload   = payload;

        return internal_handle(index, slot.handle.gen(), 0);
    }

    auto get_slot_if_valid(this auto&& self, handle h)
        -> copy_const_t<decltype(self), Slot>*
    {
        if (h.is_null()) { return nullptr; }

        auto index = static_cast<isize_t>(h.index());
        if (index < 0 || index >= self.m_slots.size()) { return nullptr; }

        auto& slot = self.m_slots[index];
        if (!slot.is_active || slot.handle.gen() != h.gen())
        {
            return nullptr;
        }

        return &slot;
    }

public:
    IndexPool() requires default_constructible<Allocator> = default;

    explicit IndexPool(Allocator allocator) : m_slots{std::move(allocator)} {}

    [[nodiscard]] bool is_full() const
    {
        return m_first_available.is_null() && m_slots.size() > config::kMaxIndex;
    }

    option<handle> acquire()
        requires (!kHasPayload && !config::kHasUser)
    {
        return acquire_handle({}).transform([](internal_handle h)
        {
            return handle(h.index(), h.gen());
        });
    }

    handle acquire_ensure()
        requires (!kHasPayload && !config::kHasUser)
    {
        auto opt = acquire();
        ASL_ASSERT_RELEASE(opt.has_value());
        return opt.value();
    }

    option<handle> acquire(const Payload& payload)
        requires (kHasPayload && !config::kHasUser)
    {
        return acquire_handle(payload).transform([](internal_handle h)
        {
            return handle(h.index(), h.gen());
        });
    }

    handle acquire_ensure(const Payload& payload)
        requires (kHasPayload && !config::kHasUser)
    {
        auto opt = acquire(payload);
        ASL_ASSERT_RELEASE(opt.has_value());
        return opt.value();
    }

    option<handle> acquire(config::UserType user)
        requires (!kHasPayload && config::kHasUser)
    {
        return acquire_handle({}).transform([user](internal_handle h)
        {
            return handle(h.index(), h.gen(), user);
        });
    }

    handle acquire_ensure(config::UserType user)
        requires (!kHasPayload && config::kHasUser)
    {
        auto opt = acquire(user);
        ASL_ASSERT_RELEASE(opt.has_value());
        return opt.value();
    }

    option<handle> acquire(config::UserType user, const Payload& payload)
        requires (kHasPayload && config::kHasUser)
    {
        return acquire_handle(payload).transform([user](internal_handle h)
        {
            return handle(h.index(), h.gen(), user);
        });
    }

    handle acquire_ensure(config::UserType user, const Payload& payload)
        requires (kHasPayload && config::kHasUser)
    {
        auto opt = acquire(user, payload);
        ASL_ASSERT_RELEASE(opt.has_value());
        return opt.value();
    }

    // @Todo Add a policy to abandon slots that reached max generation
    void release(handle h)
    {
        if (Slot* slot = get_slot_if_valid(h); slot != nullptr)
        {
            const uint64_t next_gen = h.gen() == config::kMaxGen ? 0 : h.gen() + 1;

            slot->is_active = false;

            if (m_first_available.is_null())
            {
                slot->is_end_of_list = true;
                slot->handle = internal_handle(h.index(), next_gen, 0);
            }
            else
            {
                slot->is_end_of_list = false;
                slot->handle = internal_handle(m_first_available.index(), next_gen, 0);
            }

            m_first_available = internal_handle(h.index(), 0, 0);
        }
    }

    bool is_valid(handle h) const
    {
        return get_slot_if_valid(h) != nullptr;
    }

    const Payload* get_payload(handle h) const
        requires kHasPayload
    {
        if (const Slot* slot = get_slot_if_valid(h); slot != nullptr)
        {
            return &slot->payload;
        }
        return nullptr;
    }

    option<Payload> exchange_payload(handle h, Payload new_payload)
        requires kHasPayload
    {
        if (Slot* slot = get_slot_if_valid(h); slot != nullptr)
        {
            return asl::exchange(slot->payload, new_payload);
        }
        return nullopt;
    }
};

} // namespace asl

