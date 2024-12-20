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
struct NonCopyAssignable { NonCopyAssignable& operator=(const NonCopyAssignable&) = delete; };

struct MoveAssignable { MoveAssignable& operator=(MoveAssignable&&) { return *this; } };
struct TriviallyMoveAssignable { TriviallyMoveAssignable& operator=(TriviallyMoveAssignable&&) = default; };
struct NonMoveAssignable { NonMoveAssignable& operator=(NonMoveAssignable&&) = delete; };

struct TriviallyDestructible { ~TriviallyDestructible() = default; };
struct HasDestructor { ~HasDestructor() {} };

