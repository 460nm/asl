#pragma once

#include "asl/utility.hpp"
#include "asl/io.hpp"
#include "asl/allocator.hpp"
#include "asl/string_view.hpp"

struct TrivialType
{
    int x;
    TrivialType() = default;
    TrivialType(const TrivialType&) = default;
    TrivialType(TrivialType&&) = default;
    TrivialType& operator=(const TrivialType&) = default;
    TrivialType& operator=(TrivialType&&) = default;
    ~TrivialType() = default;
};

struct TrivialTypeDefaultValue
{
    int x{};
    TrivialTypeDefaultValue() = default;
    TrivialTypeDefaultValue(const TrivialTypeDefaultValue&) = default;
    TrivialTypeDefaultValue(TrivialTypeDefaultValue&&) = default;
    TrivialTypeDefaultValue& operator=(const TrivialTypeDefaultValue&) = default;
    TrivialTypeDefaultValue& operator=(TrivialTypeDefaultValue&&) = default;
    ~TrivialTypeDefaultValue() = default;
};

struct WithDestructor
{
    WithDestructor() = default;
    WithDestructor(const WithDestructor&) = default;
    WithDestructor(WithDestructor&&) = default;
    WithDestructor& operator=(const WithDestructor&) = default;
    WithDestructor& operator=(WithDestructor&&) = default;
    ~WithDestructor() {} // NOLINT
};

struct Copyable // NOLINT
{
    Copyable(const Copyable&) {} // NOLINT
    Copyable& operator=(const Copyable&); // NOLINT
};

struct MoveableOnly // NOLINT
{
    MoveableOnly(const MoveableOnly&) = delete;
    MoveableOnly& operator=(const MoveableOnly&) = delete;
    MoveableOnly(MoveableOnly&&);
    MoveableOnly& operator=(MoveableOnly&&); // NOLINT
};

struct Pinned // NOLINT
{
    Pinned(const Pinned&) = delete;
    Pinned& operator=(const Pinned&) = delete;
    Pinned(Pinned&&) = delete;
    Pinned& operator=(Pinned&&) = delete;
};

struct DestructorObserver
{
    bool* destroyed;

    explicit DestructorObserver(bool* destroyed_) : destroyed{destroyed_} {}

    DestructorObserver(const DestructorObserver&) = delete;
    DestructorObserver& operator=(const DestructorObserver&) = delete;

    DestructorObserver(DestructorObserver&& other)
        : destroyed{asl::exchange(other.destroyed, nullptr)}
    {}

    DestructorObserver& operator=(DestructorObserver&& other)
    {
        if (this != &other)
        {
            destroyed = asl::exchange(other.destroyed, nullptr);
        }
        return *this;
    }

    ~DestructorObserver()
    {
        if (destroyed != nullptr)
        {
            ASL_ASSERT_RELEASE(*destroyed == false);
            *destroyed = true;
        }
    }
};

class StringSink : public asl::Writer
{
    // @Todo Use string, once we have it, or a buffer
    isize_t m_current_len{};
    char*   m_data{};
    
public:
    ~StringSink() override
    {
        reset();
    }
    
    void write(asl::span<const asl::byte> str) override
    {
        m_data = reinterpret_cast<char*>(asl::GlobalHeap::realloc(
            m_data,
            asl::layout::array<char>(m_current_len),
            asl::layout::array<char>(m_current_len + str.size())));
        
        asl::memcpy(m_data + m_current_len, str.data(), str.size()); // NOLINT
        
        m_current_len += str.size();
    }

    constexpr asl::string_view str() const { return {m_data, m_current_len}; }

    void reset()
    {
        if (m_data != nullptr)
        {
            m_current_len = 0;
            asl::GlobalHeap::dealloc(m_data, asl::layout::array<char>(m_current_len));
            m_data = nullptr;
        }
    }
};
