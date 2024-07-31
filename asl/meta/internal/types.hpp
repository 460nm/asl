#pragma once

namespace asl::internal {

template<typename U, typename V> inline constexpr bool is_same = false;
template<typename T> inline constexpr bool is_same<T, T> = true;

} // namespace asl::internal

