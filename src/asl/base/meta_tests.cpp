// Copyright 2025 Steven Le Rouzic
//
// SPDX-License-Identifier: BSD-3-Clause

#include "asl/base/meta.hpp"
#include "asl/base/support.hpp"

struct C {};
enum E {}; // NOLINT
enum class Ec : int {};
union U {};

static_assert(asl::is_same<int, int>);
static_assert(!asl::is_same<int, const int>);
static_assert(asl::is_same<C, C>);
static_assert(!asl::is_same<C, C*>);
static_assert(asl::is_same<void, void>);
static_assert(!asl::is_same<E, Ec>);
static_assert(!asl::is_same<Ec, int>);
static_assert(asl::is_same<E, E>);
static_assert(asl::is_same<Ec, Ec>);

static_assert(asl::is_same<asl::remove_const_t<int>, int>);
static_assert(asl::is_same<asl::remove_const_t<const volatile int>, volatile int>);
static_assert(asl::is_same<asl::remove_const_t<const int*>, const int*>);
static_assert(asl::is_same<asl::remove_const_t<const int* const>, const int*>);

static_assert(asl::is_same<asl::remove_volatile_t<int>, int>);
static_assert(asl::is_same<asl::remove_volatile_t<const volatile int>, const int>);
static_assert(asl::is_same<asl::remove_volatile_t<volatile int*>, volatile int*>);
static_assert(asl::is_same<asl::remove_volatile_t<volatile int* volatile>, volatile int*>);

static_assert(asl::is_same<asl::remove_cv_t<int>, int>);
static_assert(asl::is_same<asl::remove_cv_t<const volatile int>, int>);
static_assert(asl::is_same<asl::remove_cv_t<const volatile int* const>, const volatile int*>);
static_assert(asl::is_same<asl::remove_cv_t<volatile int* const volatile>, volatile int*>);

static_assert(asl::is_void<void>);
static_assert(asl::is_void<const void>);
static_assert(asl::is_void<volatile void>);
static_assert(asl::is_void<const volatile void>);
static_assert(!asl::is_void<nullptr_t>);
static_assert(!asl::is_void<int>);
static_assert(!asl::is_void<float>);
static_assert(!asl::is_void<int[5]>);
static_assert(!asl::is_void<int[]>);
static_assert(!asl::is_void<int*>);
static_assert(!asl::is_void<int (*)(int)>);
static_assert(!asl::is_void<int&>);
static_assert(!asl::is_void<int&&>);
static_assert(!asl::is_void<int C::*>);
static_assert(!asl::is_void<int (C::*)(int)>);
static_assert(!asl::is_void<E>);
static_assert(!asl::is_void<Ec>);
static_assert(!asl::is_void<U>);
static_assert(!asl::is_void<C>);
static_assert(!asl::is_void<int (int)>);

static_assert(!asl::is_null_pointer<void>);
static_assert(asl::is_null_pointer<nullptr_t>);
static_assert(asl::is_null_pointer<const nullptr_t>);
static_assert(asl::is_null_pointer<volatile nullptr_t>);
static_assert(asl::is_null_pointer<const volatile nullptr_t>);
static_assert(!asl::is_null_pointer<int>);
static_assert(!asl::is_null_pointer<float>);
static_assert(!asl::is_null_pointer<int[5]>);
static_assert(!asl::is_null_pointer<int[]>);
static_assert(!asl::is_null_pointer<int*>);
static_assert(!asl::is_null_pointer<int (*)(int)>);
static_assert(!asl::is_null_pointer<int&>);
static_assert(!asl::is_null_pointer<int&&>);
static_assert(!asl::is_null_pointer<int C::*>);
static_assert(!asl::is_null_pointer<int (C::*)(int)>);
static_assert(!asl::is_null_pointer<E>);
static_assert(!asl::is_null_pointer<Ec>);
static_assert(!asl::is_null_pointer<U>);
static_assert(!asl::is_null_pointer<C>);
static_assert(!asl::is_null_pointer<int (int)>);

static_assert(!asl::is_integral<void>);
static_assert(!asl::is_integral<nullptr_t>);
static_assert(asl::is_integral<bool>);
static_assert(asl::is_integral<char>);
static_assert(asl::is_integral<char32_t>);
static_assert(asl::is_integral<char8_t>);
static_assert(asl::is_integral<wchar_t>);
static_assert(asl::is_integral<signed char>);
static_assert(asl::is_integral<signed short int>);
static_assert(asl::is_integral<signed int>);
static_assert(asl::is_integral<signed long int>);
static_assert(asl::is_integral<signed long long int>);
static_assert(asl::is_integral<unsigned char>);
static_assert(asl::is_integral<unsigned short int>);
static_assert(asl::is_integral<unsigned int>);
static_assert(asl::is_integral<unsigned long int>);
static_assert(asl::is_integral<unsigned long long int>);
static_assert(asl::is_integral<const int>);
static_assert(asl::is_integral<volatile int>);
static_assert(asl::is_integral<const volatile int>);
static_assert(!asl::is_integral<float>);
static_assert(!asl::is_integral<int[5]>);
static_assert(!asl::is_integral<int[]>);
static_assert(!asl::is_integral<int*>);
static_assert(!asl::is_integral<int (*)(int)>);
static_assert(!asl::is_integral<int&>);
static_assert(!asl::is_integral<int&&>);
static_assert(!asl::is_integral<int C::*>);
static_assert(!asl::is_integral<int (C::*)(int)>);
static_assert(!asl::is_integral<E>);
static_assert(!asl::is_integral<Ec>);
static_assert(!asl::is_integral<U>);
static_assert(!asl::is_integral<C>);
static_assert(!asl::is_integral<int (int)>);

static_assert(!asl::is_floating_point<void>);
static_assert(!asl::is_floating_point<nullptr_t>);
static_assert(!asl::is_floating_point<int>);
static_assert(asl::is_floating_point<float>);
static_assert(asl::is_floating_point<const float>);
static_assert(asl::is_floating_point<const volatile float>);
static_assert(asl::is_floating_point<volatile float>);
static_assert(asl::is_floating_point<double>);
static_assert(!asl::is_floating_point<int[5]>);
static_assert(!asl::is_floating_point<int[]>);
static_assert(!asl::is_floating_point<int*>);
static_assert(!asl::is_floating_point<int (*)(int)>);
static_assert(!asl::is_floating_point<int&>);
static_assert(!asl::is_floating_point<int&&>);
static_assert(!asl::is_floating_point<int C::*>);
static_assert(!asl::is_floating_point<int (C::*)(int)>);
static_assert(!asl::is_floating_point<E>);
static_assert(!asl::is_floating_point<Ec>);
static_assert(!asl::is_floating_point<U>);
static_assert(!asl::is_floating_point<C>);
static_assert(!asl::is_floating_point<int (int)>);

static_assert(!asl::is_array<void>);
static_assert(!asl::is_array<nullptr_t>);
static_assert(!asl::is_array<int>);
static_assert(!asl::is_array<float>);
static_assert(asl::is_array<int[5]>);
static_assert(asl::is_array<const int[5]>);
static_assert(asl::is_array<volatile int[5]>);
static_assert(asl::is_array<int[]>);
static_assert(asl::is_array<int*[]>);
static_assert(asl::is_array<const volatile int[]>);
static_assert(!asl::is_array<int*>);
static_assert(!asl::is_array<int (*)(int)>);
static_assert(!asl::is_array<int&>);
static_assert(!asl::is_array<int&&>);
static_assert(!asl::is_array<int C::*>);
static_assert(!asl::is_array<int (C::*)(int)>);
static_assert(!asl::is_array<E>);
static_assert(!asl::is_array<Ec>);
static_assert(!asl::is_array<U>);
static_assert(!asl::is_array<C>);
static_assert(!asl::is_array<int (int)>);

static_assert(!asl::is_pointer<void>);
static_assert(!asl::is_pointer<nullptr_t>);
static_assert(!asl::is_pointer<int>);
static_assert(!asl::is_pointer<float>);
static_assert(!asl::is_pointer<int[5]>);
static_assert(!asl::is_pointer<int[]>);
static_assert(asl::is_pointer<int*>);
static_assert(asl::is_pointer<const int*>);
static_assert(asl::is_pointer<int* const>);
static_assert(asl::is_pointer<int* volatile>);
static_assert(asl::is_pointer<int* const volatile>);
static_assert(asl::is_pointer<int (*)(int)>);
static_assert(asl::is_pointer<int (* const)(int)>);
static_assert(!asl::is_pointer<int&>);
static_assert(!asl::is_pointer<int&&>);
static_assert(!asl::is_pointer<int C::*>);
static_assert(!asl::is_pointer<int (C::*)(int)>);
static_assert(!asl::is_pointer<E>);
static_assert(!asl::is_pointer<Ec>);
static_assert(!asl::is_pointer<U>);
static_assert(!asl::is_pointer<C>);
static_assert(!asl::is_pointer<int (int)>);

static_assert(!asl::is_lvalue_reference<void>);
static_assert(!asl::is_lvalue_reference<nullptr_t>);
static_assert(!asl::is_lvalue_reference<int>);
static_assert(!asl::is_lvalue_reference<float>);
static_assert(!asl::is_lvalue_reference<int[5]>);
static_assert(!asl::is_lvalue_reference<int[]>);
static_assert(!asl::is_lvalue_reference<int*>);
static_assert(!asl::is_lvalue_reference<int (*)(int)>);
static_assert(asl::is_lvalue_reference<int&>);
static_assert(asl::is_lvalue_reference<const int&>);
static_assert(asl::is_lvalue_reference<volatile int&>);
static_assert(asl::is_lvalue_reference<const volatile int&>);
static_assert(asl::is_lvalue_reference<int (&)(int)>);
static_assert(asl::is_lvalue_reference<int (&)(int) noexcept>);
static_assert(!asl::is_lvalue_reference<int&&>);
static_assert(!asl::is_lvalue_reference<int C::*>);
static_assert(!asl::is_lvalue_reference<int (C::*)(int)>);
static_assert(!asl::is_lvalue_reference<E>);
static_assert(!asl::is_lvalue_reference<Ec>);
static_assert(!asl::is_lvalue_reference<U>);
static_assert(!asl::is_lvalue_reference<C>);
static_assert(!asl::is_lvalue_reference<int (int)>);

static_assert(!asl::is_rvalue_reference<void>);
static_assert(!asl::is_rvalue_reference<nullptr_t>);
static_assert(!asl::is_rvalue_reference<int>);
static_assert(!asl::is_rvalue_reference<float>);
static_assert(!asl::is_rvalue_reference<int[5]>);
static_assert(!asl::is_rvalue_reference<int[]>);
static_assert(!asl::is_rvalue_reference<int*>);
static_assert(!asl::is_rvalue_reference<int (*)(int)>);
static_assert(!asl::is_rvalue_reference<int&>);
static_assert(asl::is_rvalue_reference<int&&>);
static_assert(asl::is_rvalue_reference<const int&&>);
static_assert(asl::is_rvalue_reference<volatile int&&>);
static_assert(asl::is_rvalue_reference<const volatile int&&>);
static_assert(asl::is_rvalue_reference<int (&&)(int)>);
static_assert(asl::is_rvalue_reference<int (&&)(int) noexcept>);
static_assert(!asl::is_rvalue_reference<int C::*>);
static_assert(!asl::is_rvalue_reference<int (C::*)(int)>);
static_assert(!asl::is_rvalue_reference<E>);
static_assert(!asl::is_rvalue_reference<Ec>);
static_assert(!asl::is_rvalue_reference<U>);
static_assert(!asl::is_rvalue_reference<C>);
static_assert(!asl::is_rvalue_reference<int (int)>);

static_assert(!asl::is_member_object_pointer<void>);
static_assert(!asl::is_member_object_pointer<nullptr_t>);
static_assert(!asl::is_member_object_pointer<int>);
static_assert(!asl::is_member_object_pointer<float>);
static_assert(!asl::is_member_object_pointer<int[5]>);
static_assert(!asl::is_member_object_pointer<int[]>);
static_assert(!asl::is_member_object_pointer<int*>);
static_assert(!asl::is_member_object_pointer<int (*)(int)>);
static_assert(!asl::is_member_object_pointer<int&>);
static_assert(!asl::is_member_object_pointer<int&&>);
static_assert(asl::is_member_object_pointer<int C::*>);
static_assert(asl::is_member_object_pointer<int C::* const>);
static_assert(asl::is_member_object_pointer<int C::* volatile>);
static_assert(asl::is_member_object_pointer<int C::* const volatile>);
static_assert(!asl::is_member_object_pointer<int (C::*)(int)>);
static_assert(!asl::is_member_object_pointer<E>);
static_assert(!asl::is_member_object_pointer<Ec>);
static_assert(!asl::is_member_object_pointer<U>);
static_assert(!asl::is_member_object_pointer<C>);
static_assert(!asl::is_member_object_pointer<int (int)>);

static_assert(!asl::is_member_function_pointer<void>);
static_assert(!asl::is_member_function_pointer<nullptr_t>);
static_assert(!asl::is_member_function_pointer<int>);
static_assert(!asl::is_member_function_pointer<float>);
static_assert(!asl::is_member_function_pointer<int[5]>);
static_assert(!asl::is_member_function_pointer<int[]>);
static_assert(!asl::is_member_function_pointer<int*>);
static_assert(!asl::is_member_function_pointer<int (*)(int)>);
static_assert(!asl::is_member_function_pointer<int&>);
static_assert(!asl::is_member_function_pointer<int&&>);
static_assert(!asl::is_member_function_pointer<int C::*>);
static_assert(asl::is_member_function_pointer<int (C::*)(int)>);
static_assert(asl::is_member_function_pointer<int (C::*)(int) &>);
static_assert(asl::is_member_function_pointer<int (C::*)(int) const volatile &&>);
static_assert(asl::is_member_function_pointer<int (C::* const)(int)>);
static_assert(asl::is_member_function_pointer<int (C::* volatile)(int)>);
static_assert(asl::is_member_function_pointer<int (C::* const volatile)(int)>);
static_assert(!asl::is_member_function_pointer<E>);
static_assert(!asl::is_member_function_pointer<Ec>);
static_assert(!asl::is_member_function_pointer<U>);
static_assert(!asl::is_member_function_pointer<C>);
static_assert(!asl::is_member_function_pointer<int (int)>);

static_assert(!asl::is_enum<void>);
static_assert(!asl::is_enum<nullptr_t>);
static_assert(!asl::is_enum<int>);
static_assert(!asl::is_enum<float>);
static_assert(!asl::is_enum<int[5]>);
static_assert(!asl::is_enum<int[]>);
static_assert(!asl::is_enum<int*>);
static_assert(!asl::is_enum<int (*)(int)>);
static_assert(!asl::is_enum<int&>);
static_assert(!asl::is_enum<int&&>);
static_assert(!asl::is_enum<int C::*>);
static_assert(!asl::is_enum<int (C::*)(int)>);
static_assert(asl::is_enum<E>);
static_assert(asl::is_enum<const E>);
static_assert(asl::is_enum<volatile const E>);
static_assert(asl::is_enum<volatile E>);
static_assert(asl::is_enum<Ec>);
static_assert(asl::is_enum<const Ec>);
static_assert(asl::is_enum<volatile const Ec>);
static_assert(asl::is_enum<volatile Ec>);
static_assert(!asl::is_enum<U>);
static_assert(!asl::is_enum<C>);
static_assert(!asl::is_enum<int (int)>);

static_assert(!asl::is_union<void>);
static_assert(!asl::is_union<nullptr_t>);
static_assert(!asl::is_union<int>);
static_assert(!asl::is_union<float>);
static_assert(!asl::is_union<int[5]>);
static_assert(!asl::is_union<int[]>);
static_assert(!asl::is_union<int*>);
static_assert(!asl::is_union<int (*)(int)>);
static_assert(!asl::is_union<int&>);
static_assert(!asl::is_union<int&&>);
static_assert(!asl::is_union<int C::*>);
static_assert(!asl::is_union<int (C::*)(int)>);
static_assert(!asl::is_union<E>);
static_assert(!asl::is_union<Ec>);
static_assert(asl::is_union<U>);
static_assert(asl::is_union<const U>);
static_assert(asl::is_union<volatile const U>);
static_assert(asl::is_union<volatile U>);
static_assert(!asl::is_union<C>);
static_assert(!asl::is_union<int (int)>);

static_assert(!asl::is_class<void>);
static_assert(!asl::is_class<nullptr_t>);
static_assert(!asl::is_class<int>);
static_assert(!asl::is_class<float>);
static_assert(!asl::is_class<int[5]>);
static_assert(!asl::is_class<int[]>);
static_assert(!asl::is_class<int*>);
static_assert(!asl::is_class<int (*)(int)>);
static_assert(!asl::is_class<int&>);
static_assert(!asl::is_class<int&&>);
static_assert(!asl::is_class<int C::*>);
static_assert(!asl::is_class<int (C::*)(int)>);
static_assert(!asl::is_class<E>);
static_assert(!asl::is_class<Ec>);
static_assert(!asl::is_class<U>);
static_assert(asl::is_class<C>);
static_assert(asl::is_class<const C>);
static_assert(asl::is_class<volatile C>);
static_assert(asl::is_class<const volatile C>);
static_assert(!asl::is_class<int (int)>);

static_assert(!asl::is_function<void>);
static_assert(!asl::is_function<nullptr_t>);
static_assert(!asl::is_function<int>);
static_assert(!asl::is_function<float>);
static_assert(!asl::is_function<int[5]>);
static_assert(!asl::is_function<int[]>);
static_assert(!asl::is_function<int*>);
static_assert(!asl::is_function<int (*)(int)>);
static_assert(!asl::is_function<int&>);
static_assert(!asl::is_function<int&&>);
static_assert(!asl::is_function<int C::*>);
static_assert(!asl::is_function<int (C::*)(int)>);
static_assert(!asl::is_function<E>);
static_assert(!asl::is_function<Ec>);
static_assert(!asl::is_function<U>);
static_assert(!asl::is_function<C>);
static_assert(asl::is_function<int (int)>);
static_assert(asl::is_function<int (int) &>);
static_assert(asl::is_function<int (int) const &&>);
static_assert(asl::is_function<int (int) volatile noexcept>);

static_assert(!asl::is_reference<void>);
static_assert(!asl::is_reference<nullptr_t>);
static_assert(!asl::is_reference<int>);
static_assert(!asl::is_reference<float>);
static_assert(!asl::is_reference<int[5]>);
static_assert(!asl::is_reference<int[]>);
static_assert(!asl::is_reference<int*>);
static_assert(!asl::is_reference<int (*)(int)>);
static_assert(asl::is_reference<int&>);
static_assert(asl::is_reference<int&&>);
static_assert(!asl::is_reference<int C::*>);
static_assert(!asl::is_reference<int (C::*)(int)>);
static_assert(!asl::is_reference<E>);
static_assert(!asl::is_reference<Ec>);
static_assert(!asl::is_reference<U>);
static_assert(!asl::is_reference<C>);
static_assert(!asl::is_reference<int (int)>);

static_assert(!asl::is_arithmetic<void>);
static_assert(!asl::is_arithmetic<nullptr_t>);
static_assert(asl::is_arithmetic<int>);
static_assert(asl::is_arithmetic<float>);
static_assert(asl::is_arithmetic<char>);
static_assert(asl::is_arithmetic<bool>);
static_assert(!asl::is_arithmetic<int[5]>);
static_assert(!asl::is_arithmetic<int[]>);
static_assert(!asl::is_arithmetic<int*>);
static_assert(!asl::is_arithmetic<int (*)(int)>);
static_assert(!asl::is_arithmetic<int&>);
static_assert(!asl::is_arithmetic<int&&>);
static_assert(!asl::is_arithmetic<int C::*>);
static_assert(!asl::is_arithmetic<int (C::*)(int)>);
static_assert(!asl::is_arithmetic<E>);
static_assert(!asl::is_arithmetic<Ec>);
static_assert(!asl::is_arithmetic<U>);
static_assert(!asl::is_arithmetic<C>);
static_assert(!asl::is_arithmetic<int (int)>);

static_assert(asl::is_fundamental<void>);
static_assert(asl::is_fundamental<nullptr_t>);
static_assert(asl::is_fundamental<int>);
static_assert(asl::is_fundamental<float>);
static_assert(asl::is_fundamental<char>);
static_assert(asl::is_fundamental<bool>);
static_assert(!asl::is_fundamental<int[5]>);
static_assert(!asl::is_fundamental<int[]>);
static_assert(!asl::is_fundamental<int*>);
static_assert(!asl::is_fundamental<int (*)(int)>);
static_assert(!asl::is_fundamental<int&>);
static_assert(!asl::is_fundamental<int&&>);
static_assert(!asl::is_fundamental<int C::*>);
static_assert(!asl::is_fundamental<int (C::*)(int)>);
static_assert(!asl::is_fundamental<E>);
static_assert(!asl::is_fundamental<Ec>);
static_assert(!asl::is_fundamental<U>);
static_assert(!asl::is_fundamental<C>);
static_assert(!asl::is_fundamental<int (int)>);

static_assert(!asl::is_object<void>);
static_assert(asl::is_object<nullptr_t>);
static_assert(asl::is_object<int>);
static_assert(asl::is_object<float>);
static_assert(asl::is_object<char>);
static_assert(asl::is_object<bool>);
static_assert(asl::is_object<int[5]>);
static_assert(asl::is_object<int[]>);
static_assert(asl::is_object<int*>);
static_assert(asl::is_object<int (*)(int)>);
static_assert(!asl::is_object<int&>);
static_assert(!asl::is_object<int&&>);
static_assert(asl::is_object<int C::*>);
static_assert(asl::is_object<int (C::*)(int)>);
static_assert(asl::is_object<E>);
static_assert(asl::is_object<Ec>);
static_assert(asl::is_object<U>);
static_assert(asl::is_object<C>);
static_assert(!asl::is_object<int (int)>);

static_assert(!asl::is_scalar<void>);
static_assert(asl::is_scalar<nullptr_t>);
static_assert(asl::is_scalar<int>);
static_assert(asl::is_scalar<float>);
static_assert(asl::is_scalar<char>);
static_assert(asl::is_scalar<bool>);
static_assert(!asl::is_scalar<int[5]>);
static_assert(!asl::is_scalar<int[]>);
static_assert(asl::is_scalar<int*>);
static_assert(asl::is_scalar<int (*)(int)>);
static_assert(!asl::is_scalar<int&>);
static_assert(!asl::is_scalar<int&&>);
static_assert(asl::is_scalar<int C::*>);
static_assert(asl::is_scalar<int (C::*)(int)>);
static_assert(asl::is_scalar<E>);
static_assert(asl::is_scalar<Ec>);
static_assert(!asl::is_scalar<U>);
static_assert(!asl::is_scalar<C>);
static_assert(!asl::is_scalar<int (int)>);

static_assert(!asl::is_member_pointer<void>);
static_assert(!asl::is_member_pointer<nullptr_t>);
static_assert(!asl::is_member_pointer<int>);
static_assert(!asl::is_member_pointer<float>);
static_assert(!asl::is_member_pointer<char>);
static_assert(!asl::is_member_pointer<bool>);
static_assert(!asl::is_member_pointer<int[5]>);
static_assert(!asl::is_member_pointer<int[]>);
static_assert(!asl::is_member_pointer<int*>);
static_assert(!asl::is_member_pointer<int (*)(int)>);
static_assert(!asl::is_member_pointer<int&>);
static_assert(!asl::is_member_pointer<int&&>);
static_assert(asl::is_member_pointer<int C::*>);
static_assert(asl::is_member_pointer<int (C::*)(int) &>);
static_assert(!asl::is_member_pointer<E>);
static_assert(!asl::is_member_pointer<Ec>);
static_assert(!asl::is_member_pointer<U>);
static_assert(!asl::is_member_pointer<C>);
static_assert(!asl::is_member_pointer<int (int)>);

static_assert(!asl::is_compound<void>);
static_assert(!asl::is_compound<nullptr_t>);
static_assert(!asl::is_compound<int>);
static_assert(!asl::is_compound<float>);
static_assert(!asl::is_compound<char>);
static_assert(!asl::is_compound<bool>);
static_assert(asl::is_compound<int[5]>);
static_assert(asl::is_compound<int[]>);
static_assert(asl::is_compound<int*>);
static_assert(asl::is_compound<int (*)(int)>);
static_assert(asl::is_compound<int&>);
static_assert(asl::is_compound<int&&>);
static_assert(asl::is_compound<int C::*>);
static_assert(asl::is_compound<int (C::*)(int)>);
static_assert(asl::is_compound<E>);
static_assert(asl::is_compound<Ec>);
static_assert(asl::is_compound<U>);
static_assert(asl::is_compound<C>);
static_assert(asl::is_compound<int (int)>);

static_assert(asl::is_const<const volatile int>);
static_assert(asl::is_const<const int* const>);
static_assert(!asl::is_const<const int*>);
static_assert(!asl::is_const<const int&>);
static_assert(!asl::is_const<int[3]>);
static_assert(asl::is_const<const int[3]>);

static_assert(asl::is_volatile<const volatile int>);
static_assert(asl::is_volatile<const int* volatile>);
static_assert(!asl::is_volatile<volatile int*>);
static_assert(!asl::is_volatile<volatile int&>);
static_assert(!asl::is_volatile<int[3]>);
static_assert(asl::is_volatile<volatile int[3]>);

static_assert(asl::is_bounded_array<int[3]>);
static_assert(asl::is_bounded_array<int[3][4]>);
static_assert(!asl::is_bounded_array<int[][3]>);
static_assert(!asl::is_bounded_array<int[]>);
static_assert(!asl::is_bounded_array<int>);

static_assert(!asl::is_unbounded_array<int[3]>);
static_assert(!asl::is_unbounded_array<int[3][4]>);
static_assert(asl::is_unbounded_array<int[][3]>);
static_assert(asl::is_unbounded_array<int[]>);
static_assert(!asl::is_unbounded_array<int>);

static_assert(asl::is_same<asl::remove_extent_t<int>, int>);
static_assert(asl::is_same<asl::remove_extent_t<int[]>, int>);
static_assert(asl::is_same<asl::remove_extent_t<int[45]>, int>);
static_assert(asl::is_same<asl::remove_extent_t<int[2][3]>, int[3]>);
static_assert(asl::is_same<asl::remove_extent_t<int[][45]>, int[45]>);

static_assert(asl::is_same<asl::remove_all_extents_t<int>, int>);
static_assert(asl::is_same<asl::remove_all_extents_t<int[]>, int>);
static_assert(asl::is_same<asl::remove_all_extents_t<int[45]>, int>);
static_assert(asl::is_same<asl::remove_all_extents_t<int[2][3]>, int>);
static_assert(asl::is_same<asl::remove_all_extents_t<int[][45]>, int>);

// NOLINTBEGIN

struct Trivial {};
struct HasNonTrivialDestructor { ~HasNonTrivialDestructor() {} };
struct HasTrivialDestructor { ~HasTrivialDestructor() = default; };

struct HasNonTrivialDefault { HasNonTrivialDefault() {} };
struct HasTrivialDefault { HasTrivialDefault() = default; };

struct HasNonTrivialCopyConstruct { HasNonTrivialCopyConstruct(const HasNonTrivialCopyConstruct&) {} };
struct HasTrivialCopyConstruct { HasTrivialCopyConstruct(const HasTrivialCopyConstruct&) = default; };

struct HasNonTrivialMoveConstruct { HasNonTrivialMoveConstruct(HasNonTrivialMoveConstruct&&) {} };
struct HasTrivialMoveConstruct { HasTrivialMoveConstruct(HasTrivialMoveConstruct&&) = default; };

struct HasNonTrivialCopyAssign { HasNonTrivialCopyAssign& operator=(const HasNonTrivialCopyAssign&) { return *this; } };
struct HasTrivialCopyAssign { HasTrivialCopyAssign& operator=(const HasTrivialCopyAssign&) = default; };

struct HasNonTrivialMoveAssign { HasNonTrivialMoveAssign& operator=(HasNonTrivialMoveAssign&&) { return *this; } };
struct HasTrivialMoveAssign { HasTrivialMoveAssign& operator=(HasTrivialMoveAssign&&) = default; };

struct NonTrivial
{
    NonTrivial() {}
    ~NonTrivial() {}
    NonTrivial(const NonTrivial&) {}
    NonTrivial(NonTrivial&&) {}
    NonTrivial& operator=(const NonTrivial&) { return *this; }
    NonTrivial& operator=(NonTrivial&&) { return *this; }
};

struct Problematic
{
    Problematic() = default;
    Problematic(const Problematic&) = default;
    Problematic(Problematic&&) = default;
    Problematic& operator=(const Problematic&) { return *this; }
    Problematic& operator=(Problematic&&) = default;
};

// NOLINTEND

static_assert(asl::is_trivially_default_constructible<int>);
static_assert(asl::is_trivially_default_constructible<void*>);
static_assert(asl::is_trivially_default_constructible<E>);
static_assert(asl::is_trivially_default_constructible<Ec>);
static_assert(asl::is_trivially_default_constructible<int[5]>);
static_assert(!asl::is_trivially_default_constructible<int[]>);

static_assert(asl::is_trivially_default_constructible<Trivial>);
static_assert(!asl::is_trivially_default_constructible<NonTrivial>);
static_assert(!asl::is_trivially_default_constructible<HasNonTrivialDestructor>);
static_assert(asl::is_trivially_default_constructible<HasTrivialDestructor>);
static_assert(!asl::is_trivially_default_constructible<HasNonTrivialDefault>);
static_assert(asl::is_trivially_default_constructible<HasTrivialDefault>);
static_assert(!asl::is_trivially_default_constructible<HasNonTrivialCopyConstruct>);
static_assert(!asl::is_trivially_default_constructible<HasTrivialCopyConstruct>);
static_assert(!asl::is_trivially_default_constructible<HasNonTrivialMoveConstruct>);
static_assert(!asl::is_trivially_default_constructible<HasTrivialMoveConstruct>);
static_assert(asl::is_trivially_default_constructible<HasNonTrivialCopyAssign>);
static_assert(asl::is_trivially_default_constructible<HasTrivialCopyAssign>);
static_assert(asl::is_trivially_default_constructible<HasNonTrivialMoveAssign>);
static_assert(asl::is_trivially_default_constructible<HasTrivialMoveAssign>);

static_assert(asl::is_trivially_copy_constructible<Trivial>);
static_assert(!asl::is_trivially_copy_constructible<NonTrivial>);
static_assert(!asl::is_trivially_copy_constructible<HasNonTrivialDestructor>);
static_assert(asl::is_trivially_copy_constructible<HasTrivialDestructor>);
static_assert(asl::is_trivially_copy_constructible<HasNonTrivialDefault>);
static_assert(asl::is_trivially_copy_constructible<HasTrivialDefault>);
static_assert(!asl::is_trivially_copy_constructible<HasNonTrivialCopyConstruct>);
static_assert(asl::is_trivially_copy_constructible<HasTrivialCopyConstruct>);
static_assert(!asl::is_trivially_copy_constructible<HasNonTrivialMoveConstruct>);
static_assert(!asl::is_trivially_copy_constructible<HasTrivialMoveConstruct>);
static_assert(asl::is_trivially_copy_constructible<HasNonTrivialCopyAssign>);
static_assert(asl::is_trivially_copy_constructible<HasTrivialCopyAssign>);
static_assert(!asl::is_trivially_copy_constructible<HasNonTrivialMoveAssign>);
static_assert(!asl::is_trivially_copy_constructible<HasTrivialMoveAssign>);

static_assert(asl::is_trivially_move_constructible<Trivial>);
static_assert(!asl::is_trivially_move_constructible<NonTrivial>);
static_assert(!asl::is_trivially_move_constructible<HasNonTrivialDestructor>);
static_assert(asl::is_trivially_move_constructible<HasTrivialDestructor>);
static_assert(asl::is_trivially_move_constructible<HasNonTrivialDefault>);
static_assert(asl::is_trivially_move_constructible<HasTrivialDefault>);
static_assert(!asl::is_trivially_move_constructible<HasNonTrivialCopyConstruct>);
static_assert(asl::is_trivially_move_constructible<HasTrivialCopyConstruct>);
static_assert(!asl::is_trivially_move_constructible<HasNonTrivialMoveConstruct>);
static_assert(asl::is_trivially_move_constructible<HasTrivialMoveConstruct>);
static_assert(asl::is_trivially_move_constructible<HasNonTrivialCopyAssign>);
static_assert(asl::is_trivially_move_constructible<HasTrivialCopyAssign>);
static_assert(!asl::is_trivially_move_constructible<HasNonTrivialMoveAssign>);
static_assert(!asl::is_trivially_move_constructible<HasTrivialMoveAssign>);

static_assert(asl::is_trivially_copy_assignable<Trivial>);
static_assert(!asl::is_trivially_copy_assignable<NonTrivial>);
static_assert(asl::is_trivially_copy_assignable<HasNonTrivialDestructor>);
static_assert(asl::is_trivially_copy_assignable<HasTrivialDestructor>);
static_assert(asl::is_trivially_copy_assignable<HasNonTrivialDefault>);
static_assert(asl::is_trivially_copy_assignable<HasTrivialDefault>);
static_assert(asl::is_trivially_copy_assignable<HasNonTrivialCopyConstruct>);
static_assert(asl::is_trivially_copy_assignable<HasTrivialCopyConstruct>);
static_assert(!asl::is_trivially_copy_assignable<HasNonTrivialMoveConstruct>);
static_assert(!asl::is_trivially_copy_assignable<HasTrivialMoveConstruct>);
static_assert(!asl::is_trivially_copy_assignable<HasNonTrivialCopyAssign>);
static_assert(asl::is_trivially_copy_assignable<HasTrivialCopyAssign>);
static_assert(!asl::is_trivially_copy_assignable<HasNonTrivialMoveAssign>);
static_assert(!asl::is_trivially_copy_assignable<HasTrivialMoveAssign>);

static_assert(asl::is_trivially_move_assignable<Trivial>);
static_assert(!asl::is_trivially_move_assignable<NonTrivial>);
static_assert(asl::is_trivially_move_assignable<HasNonTrivialDestructor>);
static_assert(asl::is_trivially_move_assignable<HasTrivialDestructor>);
static_assert(asl::is_trivially_move_assignable<HasNonTrivialDefault>);
static_assert(asl::is_trivially_move_assignable<HasTrivialDefault>);
static_assert(asl::is_trivially_move_assignable<HasNonTrivialCopyConstruct>);
static_assert(asl::is_trivially_move_assignable<HasTrivialCopyConstruct>);
static_assert(!asl::is_trivially_move_assignable<HasNonTrivialMoveConstruct>);
static_assert(!asl::is_trivially_move_assignable<HasTrivialMoveConstruct>);
static_assert(!asl::is_trivially_move_assignable<HasNonTrivialCopyAssign>);
static_assert(asl::is_trivially_move_assignable<HasTrivialCopyAssign>);
static_assert(!asl::is_trivially_move_assignable<HasNonTrivialMoveAssign>);
static_assert(asl::is_trivially_move_assignable<HasTrivialMoveAssign>);

static_assert(asl::is_trivially_destructible<Trivial>);
static_assert(!asl::is_trivially_destructible<NonTrivial>);
static_assert(!asl::is_trivially_destructible<HasNonTrivialDestructor>);
static_assert(asl::is_trivially_destructible<HasTrivialDestructor>);
static_assert(asl::is_trivially_destructible<HasNonTrivialDefault>);
static_assert(asl::is_trivially_destructible<HasTrivialDefault>);
static_assert(asl::is_trivially_destructible<HasNonTrivialCopyConstruct>);
static_assert(asl::is_trivially_destructible<HasTrivialCopyConstruct>);
static_assert(asl::is_trivially_destructible<HasNonTrivialMoveConstruct>);
static_assert(asl::is_trivially_destructible<HasTrivialMoveConstruct>);
static_assert(asl::is_trivially_destructible<HasNonTrivialCopyAssign>);
static_assert(asl::is_trivially_destructible<HasTrivialCopyAssign>);
static_assert(asl::is_trivially_destructible<HasNonTrivialMoveAssign>);
static_assert(asl::is_trivially_destructible<HasTrivialMoveAssign>);

static_assert(asl::is_trivially_copyable<int>);
static_assert(asl::is_trivially_copyable<void*>);
static_assert(asl::is_trivially_copyable<E>);
static_assert(asl::is_trivially_copyable<Ec>);
static_assert(asl::is_trivially_copyable<int[5]>);
static_assert(asl::is_trivially_copyable<int[]>);
static_assert(asl::is_trivially_copyable<int[]>);
static_assert(asl::is_trivially_copyable<int[][5]>);
static_assert(asl::is_trivially_copyable<int[8][5]>);
static_assert(asl::is_trivially_copyable<Trivial>);
static_assert(!asl::is_trivially_copyable<NonTrivial>);
static_assert(asl::is_trivially_copyable<Trivial[18]>);
static_assert(asl::is_trivially_copyable<Trivial[][18]>);
static_assert(asl::is_trivially_copyable<Trivial[89][18]>);
static_assert(!asl::is_trivially_copyable<NonTrivial[23]>);
static_assert(!asl::is_trivially_copyable<NonTrivial[][23]>);
static_assert(!asl::is_trivially_copyable<NonTrivial[12][23]>);
static_assert(!asl::is_trivially_copyable<HasNonTrivialDestructor>);
static_assert(asl::is_trivially_copyable<HasTrivialDestructor>);
static_assert(asl::is_trivially_copyable<HasNonTrivialDefault>);
static_assert(asl::is_trivially_copyable<HasTrivialDefault>);
static_assert(!asl::is_trivially_copyable<HasNonTrivialCopyConstruct>);
static_assert(asl::is_trivially_copyable<HasTrivialCopyConstruct>);
static_assert(!asl::is_trivially_copyable<HasNonTrivialMoveConstruct>);
static_assert(asl::is_trivially_copyable<HasTrivialMoveConstruct>);
static_assert(!asl::is_trivially_copyable<HasNonTrivialCopyAssign>);
static_assert(asl::is_trivially_copyable<HasTrivialCopyAssign>);
static_assert(!asl::is_trivially_copyable<HasNonTrivialMoveAssign>);
static_assert(asl::is_trivially_copyable<HasTrivialMoveAssign>);
static_assert(!asl::is_trivially_copyable<Problematic>);

// This is a fairly complex trait so I'm more comfortable testing against the
// builtin. Of course I could have just use the builtin, but you know...
static_assert(asl::is_trivially_copyable<int> == __is_trivially_copyable(int));
static_assert(asl::is_trivially_copyable<void*> == __is_trivially_copyable(void*));
static_assert(asl::is_trivially_copyable<E> == __is_trivially_copyable(E));
static_assert(asl::is_trivially_copyable<Ec> == __is_trivially_copyable(Ec));
static_assert(asl::is_trivially_copyable<int[5]> == __is_trivially_copyable(int[5]));
static_assert(asl::is_trivially_copyable<int[]> == __is_trivially_copyable(int[]));
static_assert(asl::is_trivially_copyable<int[]> == __is_trivially_copyable(int[]));
static_assert(asl::is_trivially_copyable<int[][5]> == __is_trivially_copyable(int[][5]));
static_assert(asl::is_trivially_copyable<int[8][5]> == __is_trivially_copyable(int[8][5]));
static_assert(asl::is_trivially_copyable<Trivial> == __is_trivially_copyable(Trivial));
static_assert(asl::is_trivially_copyable<NonTrivial> == __is_trivially_copyable(NonTrivial));
static_assert(asl::is_trivially_copyable<Trivial[18]> == __is_trivially_copyable(Trivial[18]));
static_assert(asl::is_trivially_copyable<Trivial[][18]> == __is_trivially_copyable(Trivial[][18]));
static_assert(asl::is_trivially_copyable<Trivial[89][18]> == __is_trivially_copyable(Trivial[89][18]));
static_assert(asl::is_trivially_copyable<NonTrivial[23]> == __is_trivially_copyable(NonTrivial[23]));
static_assert(asl::is_trivially_copyable<NonTrivial[][23]> == __is_trivially_copyable(NonTrivial[][23]));
static_assert(asl::is_trivially_copyable<NonTrivial[12][23]> == __is_trivially_copyable(NonTrivial[12][23]));
static_assert(asl::is_trivially_copyable<HasNonTrivialDestructor> == __is_trivially_copyable(HasNonTrivialDestructor));
static_assert(asl::is_trivially_copyable<HasTrivialDestructor> == __is_trivially_copyable(HasTrivialDestructor));
static_assert(asl::is_trivially_copyable<HasNonTrivialDefault> == __is_trivially_copyable(HasNonTrivialDefault));
static_assert(asl::is_trivially_copyable<HasTrivialDefault> == __is_trivially_copyable(HasTrivialDefault));
static_assert(asl::is_trivially_copyable<HasNonTrivialCopyConstruct> == __is_trivially_copyable(HasNonTrivialCopyConstruct));
static_assert(asl::is_trivially_copyable<HasTrivialCopyConstruct> == __is_trivially_copyable(HasTrivialCopyConstruct));
static_assert(asl::is_trivially_copyable<HasNonTrivialMoveConstruct> == __is_trivially_copyable(HasNonTrivialMoveConstruct));
static_assert(asl::is_trivially_copyable<HasTrivialMoveConstruct> == __is_trivially_copyable(HasTrivialMoveConstruct));
static_assert(asl::is_trivially_copyable<HasNonTrivialCopyAssign> == __is_trivially_copyable(HasNonTrivialCopyAssign));
static_assert(asl::is_trivially_copyable<HasTrivialCopyAssign> == __is_trivially_copyable(HasTrivialCopyAssign));
static_assert(asl::is_trivially_copyable<HasNonTrivialMoveAssign> == __is_trivially_copyable(HasNonTrivialMoveAssign));
static_assert(asl::is_trivially_copyable<HasTrivialMoveAssign> == __is_trivially_copyable(HasTrivialMoveAssign));
static_assert(asl::is_trivially_copyable<Problematic> == __is_trivially_copyable(Problematic));

static_assert(!asl::is_scoped_enum<E>);
static_assert(asl::is_scoped_enum<Ec>);
static_assert(!asl::is_scoped_enum<int>);

static_assert(asl::is_signed<int>);
static_assert(asl::is_signed<float>);
static_assert(!asl::is_signed<unsigned int>);
static_assert(!asl::is_signed<E>);
static_assert(!asl::is_signed<Ec>);
static_assert(!asl::is_signed<C>);

static_assert(!asl::is_unsigned<int>);
static_assert(!asl::is_unsigned<float>);
static_assert(asl::is_unsigned<unsigned int>);
static_assert(!asl::is_unsigned<E>);
static_assert(!asl::is_unsigned<Ec>);
static_assert(!asl::is_unsigned<C>);

int main()
{
    return 0;
}
