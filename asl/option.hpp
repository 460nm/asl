#pragma once

#include "asl/meta.hpp"
#include "asl/maybe_uninit.hpp"

namespace asl
{

struct nullopt_t {};
static constexpr nullopt_t nullopt{};

template<is_object T>
class option
{
    maybe_uninit<T> m_payload;
    bool            m_has_value = false;

public:
    constexpr option() = default;
    constexpr option(nullopt_t) {} // NOLINT(*-explicit-conversions)

    constexpr ~option() = default;
    constexpr ~option() requires (!trivially_destructible<T>)
    {
        if (m_has_value) { m_payload.uninit_unsafe(); }
    }
    
};

} // namespace asl
