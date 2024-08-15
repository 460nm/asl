#pragma once

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
struct NoCopyAssignable { NoCopyAssignable& operator=(const NoCopyAssignable&) = delete; };

struct MoveAssignable { MoveAssignable& operator=(MoveAssignable&&) { return *this; } };
struct TriviallyMoveAssignable { TriviallyMoveAssignable& operator=(TriviallyMoveAssignable&&) = default; };
struct NoMoveAssignable { NoMoveAssignable& operator=(NoMoveAssignable&&) = delete; };

struct TriviallyDestructible { ~TriviallyDestructible() = default; };
struct HasDestructor { ~HasDestructor() {} };

