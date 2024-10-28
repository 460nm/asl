#pragma once

#include "asl/utility.hpp"

struct DefaultConstructible { DefaultConstructible() {} };
struct TriviallyDefaultConstructible { TriviallyDefaultConstructible() = default; };
struct NonDefaultConstructible { NonDefaultConstructible() = delete; };

struct CopyConstructible { CopyConstructible(const CopyConstructible&) {} };
struct TriviallyCopyConstructible { TriviallyCopyConstructible(const TriviallyCopyConstructible&) = default; };
struct NonCopyConstructible { NonCopyConstructible(const NonCopyConstructible&) = delete; };

struct MoveConstructible { MoveConstructible(MoveConstructible&&) {} };
struct TriviallyMoveConstructible { TriviallyMoveConstructible(TriviallyMoveConstructible&&) = default; };
struct NonMoveConstructible { NonMoveConstructible(NonMoveConstructible&&) = delete; };

struct CopyAssignable { CopyAssignable& operator=(const CopyAssignable&) { return *this; } };
struct TriviallyCopyAssignable { TriviallyCopyAssignable& operator=(const TriviallyCopyAssignable&) = default; };
struct NonCopyAssignable { NonCopyAssignable& operator=(const NonCopyAssignable&) = delete; };

struct MoveAssignable { MoveAssignable& operator=(MoveAssignable&&) { return *this; } };
struct TriviallyMoveAssignable { TriviallyMoveAssignable& operator=(TriviallyMoveAssignable&&) = default; };
struct NonMoveAssignable { NonMoveAssignable& operator=(NonMoveAssignable&&) = delete; };

struct TriviallyDestructible { ~TriviallyDestructible() = default; };
struct HasDestructor { ~HasDestructor() {} };

struct DestructorObserver
{
    bool* destroyed;

    explicit DestructorObserver(bool* destroyed_) : destroyed{destroyed_} {}

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

