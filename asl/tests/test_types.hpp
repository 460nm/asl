#pragma once

#include "asl/utility.hpp"

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
            *destroyed = true;
        }
    }
};

