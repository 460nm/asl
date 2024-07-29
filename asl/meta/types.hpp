#pragma once

namespace asl {

template<typename T> using void_t = void;

template<typename T, typename = void>
inline constexpr bool referenceable = false;

template<typename T>
inline constexpr bool referenceable<T, void_t<T&>> = true;

} // namespace asl

