#pragma once

#include "asl/meta.hpp"
#include "asl/annotations.hpp"
#include "asl/layout.hpp"

namespace asl
{

static constexpr int64_t dynamic_size = -1;

template<is_object T, int64_t kSize = dynamic_size>
class span
{
    static constexpr bool kIsDynamic = kSize < 0;

    using SizeType = select_t<kIsDynamic, int64_t, empty>;

    T* m_data{};
    ASL_NO_UNIQUE_ADDRESS SizeType m_size{};

public:
    constexpr span() = default;

    constexpr span(const span&) = default;
    constexpr span(span&&) = default;
    
    constexpr span& operator=(const span&) = default;
    constexpr span& operator=(span&&) = default;

    ~span() = default;

    constexpr int64_t size() const
    {
        if constexpr (kIsDynamic) { return m_size; }
        else { return kSize; }
    }

    constexpr int64_t size_bytes() const { return size() * size_of<T>; }

    constexpr bool is_empty() const { return size() == 0; }
};

} // namespace asl
