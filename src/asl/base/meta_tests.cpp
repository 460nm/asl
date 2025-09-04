// Copyright 2025 Steven Le Rouzic
//
// SPDX-License-Identifier: BSD-3-Clause

#include "asl/base/meta.hpp"
#include "asl/base/support.hpp"
#include "asl/base/integers.hpp"

#define test_assert(...) if (__VA_ARGS__) {} else { \
        __builtin_debugtrap(); \
    }

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

static_assert(asl::is_same<asl::conditional_t<true, int, short>, int>);
static_assert(asl::is_same<asl::conditional_t<false, int, short>, short>);

static_assert(asl::is_same<asl::remove_const_t<int>, int>);
static_assert(asl::is_same<asl::remove_const_t<const int[5]>, int[5]>);
static_assert(asl::is_same<asl::remove_const_t<const int[]>, int[]>);
static_assert(asl::is_same<asl::remove_const_t<int&>, int&>);
static_assert(asl::is_same<asl::remove_const_t<const int&>, const int&>);
static_assert(asl::is_same<asl::remove_const_t<int (int)>, int (int)>);
static_assert(asl::is_same<asl::remove_const_t<int (*)(int)>, int (*)(int)>);
static_assert(asl::is_same<asl::remove_const_t<int (* const)(int)>, int (*)(int)>);
static_assert(asl::is_same<asl::remove_const_t<int (int) const>, int (int) const>);
static_assert(asl::is_same<asl::remove_const_t<const volatile int>, volatile int>);
static_assert(asl::is_same<asl::remove_const_t<const int*>, const int*>);
static_assert(asl::is_same<asl::remove_const_t<const int* const>, const int*>);

static_assert(asl::is_same<asl::remove_volatile_t<int>, int>);
static_assert(asl::is_same<asl::remove_volatile_t<volatile int[5]>, int[5]>);
static_assert(asl::is_same<asl::remove_volatile_t<volatile int[]>, int[]>);
static_assert(asl::is_same<asl::remove_volatile_t<int&>, int&>);
static_assert(asl::is_same<asl::remove_volatile_t<volatile int&>, volatile int&>);
static_assert(asl::is_same<asl::remove_volatile_t<int (int)>, int (int)>);
static_assert(asl::is_same<asl::remove_volatile_t<int (*)(int)>, int (*)(int)>);
static_assert(asl::is_same<asl::remove_volatile_t<int (* volatile)(int)>, int (*)(int)>);
static_assert(asl::is_same<asl::remove_volatile_t<int (int) volatile>, int (int) volatile>);
static_assert(asl::is_same<asl::remove_volatile_t<const volatile int>, const int>);
static_assert(asl::is_same<asl::remove_volatile_t<volatile int*>, volatile int*>);
static_assert(asl::is_same<asl::remove_volatile_t<volatile int* volatile>, volatile int*>);

static_assert(asl::is_same<asl::remove_cv_t<int>, int>);
static_assert(asl::is_same<asl::remove_cv_t<const volatile int[5]>, int[5]>);
static_assert(asl::is_same<asl::remove_cv_t<const volatile int[]>, int[]>);
static_assert(asl::is_same<asl::remove_cv_t<int&>, int&>);
static_assert(asl::is_same<asl::remove_cv_t<const volatile int&>, const volatile int&>);
static_assert(asl::is_same<asl::remove_cv_t<int (int)>, int (int)>);
static_assert(asl::is_same<asl::remove_cv_t<int (*)(int)>, int (*)(int)>);
static_assert(asl::is_same<asl::remove_cv_t<int (* const volatile)(int)>, int (*)(int)>);
static_assert(asl::is_same<asl::remove_cv_t<int (int) const volatile>, int (int) const volatile>);
static_assert(asl::is_same<asl::remove_cv_t<const volatile int>, int>);
static_assert(asl::is_same<asl::remove_cv_t<const volatile int* const>, const volatile int*>);
static_assert(asl::is_same<asl::remove_cv_t<volatile int* const volatile>, volatile int*>);

static_assert(asl::is_same<asl::add_const_t<int>, const int>);
static_assert(asl::is_same<asl::add_const_t<int[18]>, const int[18]>);
static_assert(asl::is_same<asl::add_const_t<int*>, int* const>);
static_assert(asl::is_same<asl::add_const_t<const int*>, const int* const>);
static_assert(asl::is_same<asl::add_const_t<int>, const int>);
static_assert(asl::is_same<asl::add_const_t<int&>, int&>);
static_assert(asl::is_same<asl::add_const_t<const int&>, const int&>);
static_assert(asl::is_same<asl::add_const_t<const int>, const int>);
static_assert(asl::is_same<asl::add_const_t<volatile int>, const volatile int>);
static_assert(asl::is_same<asl::add_const_t<int (int)>, int (int)>);
static_assert(asl::is_same<asl::add_const_t<int (*)(int)>, int (* const)(int)>);

static_assert(asl::is_same<asl::add_volatile_t<int>, volatile int>);
static_assert(asl::is_same<asl::add_volatile_t<int[18]>, volatile int[18]>);
static_assert(asl::is_same<asl::add_volatile_t<int*>, int* volatile>);
static_assert(asl::is_same<asl::add_volatile_t<volatile int*>, volatile int* volatile>);
static_assert(asl::is_same<asl::add_volatile_t<int>, volatile int>);
static_assert(asl::is_same<asl::add_volatile_t<int&>, int&>);
static_assert(asl::is_same<asl::add_volatile_t<volatile int&>, volatile int&>);
static_assert(asl::is_same<asl::add_volatile_t<volatile int>, volatile int>);
static_assert(asl::is_same<asl::add_volatile_t<int (int)>, int (int)>);
static_assert(asl::is_same<asl::add_volatile_t<int (*)(int)>, int (* volatile)(int)>);
static_assert(asl::is_same<asl::add_volatile_t<const int>, const volatile int>);

static_assert(asl::is_same<asl::add_cv_t<int>, const volatile int>);
static_assert(asl::is_same<asl::add_cv_t<int[18]>, const volatile int[18]>);
static_assert(asl::is_same<asl::add_cv_t<int*>, int* const volatile>);
static_assert(asl::is_same<asl::add_cv_t<const volatile int*>, const volatile int* const volatile>);
static_assert(asl::is_same<asl::add_cv_t<int>, const volatile int>);
static_assert(asl::is_same<asl::add_cv_t<int&>, int&>);
static_assert(asl::is_same<asl::add_cv_t<const volatile int&>, const volatile int&>);
static_assert(asl::is_same<asl::add_cv_t<const volatile int>, const volatile int>);
static_assert(asl::is_same<asl::add_cv_t<int (int)>, int (int)>);
static_assert(asl::is_same<asl::add_cv_t<int (*)(int)>, int (* const volatile)(int)>);
static_assert(asl::is_same<asl::add_cv_t<const int>, const volatile int>);
static_assert(asl::is_same<asl::add_cv_t<volatile int>, const volatile int>);

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

static_assert(!asl::is_nullptr<void>);
static_assert(asl::is_nullptr<nullptr_t>);
static_assert(asl::is_nullptr<const nullptr_t>);
static_assert(asl::is_nullptr<volatile nullptr_t>);
static_assert(asl::is_nullptr<const volatile nullptr_t>);
static_assert(!asl::is_nullptr<int>);
static_assert(!asl::is_nullptr<float>);
static_assert(!asl::is_nullptr<int[5]>);
static_assert(!asl::is_nullptr<int[]>);
static_assert(!asl::is_nullptr<int*>);
static_assert(!asl::is_nullptr<int (*)(int)>);
static_assert(!asl::is_nullptr<int&>);
static_assert(!asl::is_nullptr<int&&>);
static_assert(!asl::is_nullptr<int C::*>);
static_assert(!asl::is_nullptr<int (C::*)(int)>);
static_assert(!asl::is_nullptr<E>);
static_assert(!asl::is_nullptr<Ec>);
static_assert(!asl::is_nullptr<U>);
static_assert(!asl::is_nullptr<C>);
static_assert(!asl::is_nullptr<int (int)>);

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

static_assert(!asl::is_ptr<void>);
static_assert(!asl::is_ptr<nullptr_t>);
static_assert(!asl::is_ptr<int>);
static_assert(!asl::is_ptr<float>);
static_assert(!asl::is_ptr<int[5]>);
static_assert(!asl::is_ptr<int[]>);
static_assert(asl::is_ptr<int*>);
static_assert(asl::is_ptr<const int*>);
static_assert(asl::is_ptr<int* const>);
static_assert(asl::is_ptr<int* volatile>);
static_assert(asl::is_ptr<int* const volatile>);
static_assert(asl::is_ptr<int (*)(int)>);
static_assert(asl::is_ptr<int (* const)(int)>);
static_assert(!asl::is_ptr<int&>);
static_assert(!asl::is_ptr<int&&>);
static_assert(!asl::is_ptr<int C::*>);
static_assert(!asl::is_ptr<int (C::*)(int)>);
static_assert(!asl::is_ptr<E>);
static_assert(!asl::is_ptr<Ec>);
static_assert(!asl::is_ptr<U>);
static_assert(!asl::is_ptr<C>);
static_assert(!asl::is_ptr<int (int)>);

static_assert(!asl::is_lref<void>);
static_assert(!asl::is_lref<nullptr_t>);
static_assert(!asl::is_lref<int>);
static_assert(!asl::is_lref<float>);
static_assert(!asl::is_lref<int[5]>);
static_assert(!asl::is_lref<int[]>);
static_assert(!asl::is_lref<int*>);
static_assert(!asl::is_lref<int (*)(int)>);
static_assert(asl::is_lref<int&>);
static_assert(asl::is_lref<const int&>);
static_assert(asl::is_lref<volatile int&>);
static_assert(asl::is_lref<const volatile int&>);
static_assert(asl::is_lref<int (&)(int)>);
static_assert(asl::is_lref<int (&)(int) noexcept>);
static_assert(!asl::is_lref<int&&>);
static_assert(!asl::is_lref<int C::*>);
static_assert(!asl::is_lref<int (C::*)(int)>);
static_assert(!asl::is_lref<E>);
static_assert(!asl::is_lref<Ec>);
static_assert(!asl::is_lref<U>);
static_assert(!asl::is_lref<C>);
static_assert(!asl::is_lref<int (int)>);

static_assert(!asl::is_rref<void>);
static_assert(!asl::is_rref<nullptr_t>);
static_assert(!asl::is_rref<int>);
static_assert(!asl::is_rref<float>);
static_assert(!asl::is_rref<int[5]>);
static_assert(!asl::is_rref<int[]>);
static_assert(!asl::is_rref<int*>);
static_assert(!asl::is_rref<int (*)(int)>);
static_assert(!asl::is_rref<int&>);
static_assert(asl::is_rref<int&&>);
static_assert(asl::is_rref<const int&&>);
static_assert(asl::is_rref<volatile int&&>);
static_assert(asl::is_rref<const volatile int&&>);
static_assert(asl::is_rref<int (&&)(int)>);
static_assert(asl::is_rref<int (&&)(int) noexcept>);
static_assert(!asl::is_rref<int C::*>);
static_assert(!asl::is_rref<int (C::*)(int)>);
static_assert(!asl::is_rref<E>);
static_assert(!asl::is_rref<Ec>);
static_assert(!asl::is_rref<U>);
static_assert(!asl::is_rref<C>);
static_assert(!asl::is_rref<int (int)>);

static_assert(!asl::is_member_object_ptr<void>);
static_assert(!asl::is_member_object_ptr<nullptr_t>);
static_assert(!asl::is_member_object_ptr<int>);
static_assert(!asl::is_member_object_ptr<float>);
static_assert(!asl::is_member_object_ptr<int[5]>);
static_assert(!asl::is_member_object_ptr<int[]>);
static_assert(!asl::is_member_object_ptr<int*>);
static_assert(!asl::is_member_object_ptr<int (*)(int)>);
static_assert(!asl::is_member_object_ptr<int&>);
static_assert(!asl::is_member_object_ptr<int&&>);
static_assert(asl::is_member_object_ptr<int C::*>);
static_assert(asl::is_member_object_ptr<int C::* const>);
static_assert(asl::is_member_object_ptr<int C::* volatile>);
static_assert(asl::is_member_object_ptr<int C::* const volatile>);
static_assert(!asl::is_member_object_ptr<int (C::*)(int)>);
static_assert(!asl::is_member_object_ptr<E>);
static_assert(!asl::is_member_object_ptr<Ec>);
static_assert(!asl::is_member_object_ptr<U>);
static_assert(!asl::is_member_object_ptr<C>);
static_assert(!asl::is_member_object_ptr<int (int)>);

static_assert(!asl::is_member_func_ptr<void>);
static_assert(!asl::is_member_func_ptr<nullptr_t>);
static_assert(!asl::is_member_func_ptr<int>);
static_assert(!asl::is_member_func_ptr<float>);
static_assert(!asl::is_member_func_ptr<int[5]>);
static_assert(!asl::is_member_func_ptr<int[]>);
static_assert(!asl::is_member_func_ptr<int*>);
static_assert(!asl::is_member_func_ptr<int (*)(int)>);
static_assert(!asl::is_member_func_ptr<int&>);
static_assert(!asl::is_member_func_ptr<int&&>);
static_assert(!asl::is_member_func_ptr<int C::*>);
static_assert(asl::is_member_func_ptr<int (C::*)(int)>);
static_assert(asl::is_member_func_ptr<int (C::*)(int) &>);
static_assert(asl::is_member_func_ptr<int (C::*)(int) const volatile &&>);
static_assert(asl::is_member_func_ptr<int (C::* const)(int)>);
static_assert(asl::is_member_func_ptr<int (C::* volatile)(int)>);
static_assert(asl::is_member_func_ptr<int (C::* const volatile)(int)>);
static_assert(!asl::is_member_func_ptr<E>);
static_assert(!asl::is_member_func_ptr<Ec>);
static_assert(!asl::is_member_func_ptr<U>);
static_assert(!asl::is_member_func_ptr<C>);
static_assert(!asl::is_member_func_ptr<int (int)>);

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

static_assert(!asl::is_func<void>);
static_assert(!asl::is_func<nullptr_t>);
static_assert(!asl::is_func<int>);
static_assert(!asl::is_func<float>);
static_assert(!asl::is_func<int[5]>);
static_assert(!asl::is_func<int[]>);
static_assert(!asl::is_func<int*>);
static_assert(!asl::is_func<int (*)(int)>);
static_assert(!asl::is_func<int&>);
static_assert(!asl::is_func<int&&>);
static_assert(!asl::is_func<int C::*>);
static_assert(!asl::is_func<int (C::*)(int)>);
static_assert(!asl::is_func<E>);
static_assert(!asl::is_func<Ec>);
static_assert(!asl::is_func<U>);
static_assert(!asl::is_func<C>);
static_assert(asl::is_func<int (int)>);
static_assert(asl::is_func<int (int) &>);
static_assert(asl::is_func<int (int) const &&>);
static_assert(asl::is_func<int (int) volatile noexcept>);

static_assert(!asl::is_ref<void>);
static_assert(!asl::is_ref<nullptr_t>);
static_assert(!asl::is_ref<int>);
static_assert(!asl::is_ref<float>);
static_assert(!asl::is_ref<int[5]>);
static_assert(!asl::is_ref<int[]>);
static_assert(!asl::is_ref<int*>);
static_assert(!asl::is_ref<int (*)(int)>);
static_assert(asl::is_ref<int&>);
static_assert(asl::is_ref<int&&>);
static_assert(!asl::is_ref<int C::*>);
static_assert(!asl::is_ref<int (C::*)(int)>);
static_assert(!asl::is_ref<E>);
static_assert(!asl::is_ref<Ec>);
static_assert(!asl::is_ref<U>);
static_assert(!asl::is_ref<C>);
static_assert(!asl::is_ref<int (int)>);

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

static_assert(!asl::is_member_ptr<void>);
static_assert(!asl::is_member_ptr<nullptr_t>);
static_assert(!asl::is_member_ptr<int>);
static_assert(!asl::is_member_ptr<float>);
static_assert(!asl::is_member_ptr<char>);
static_assert(!asl::is_member_ptr<bool>);
static_assert(!asl::is_member_ptr<int[5]>);
static_assert(!asl::is_member_ptr<int[]>);
static_assert(!asl::is_member_ptr<int*>);
static_assert(!asl::is_member_ptr<int (*)(int)>);
static_assert(!asl::is_member_ptr<int&>);
static_assert(!asl::is_member_ptr<int&&>);
static_assert(asl::is_member_ptr<int C::*>);
static_assert(asl::is_member_ptr<int (C::*)(int) &>);
static_assert(!asl::is_member_ptr<E>);
static_assert(!asl::is_member_ptr<Ec>);
static_assert(!asl::is_member_ptr<U>);
static_assert(!asl::is_member_ptr<C>);
static_assert(!asl::is_member_ptr<int (int)>);

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
static_assert(asl::is_unsigned<bool>);
static_assert(!asl::is_unsigned<float>);
static_assert(asl::is_unsigned<unsigned int>);
static_assert(!asl::is_unsigned<E>);
static_assert(!asl::is_unsigned<Ec>);
static_assert(!asl::is_unsigned<C>);

static_assert(asl::is_same<asl::remove_ref_t<int>, int>);
static_assert(asl::is_same<asl::remove_ref_t<int&>, int>);
static_assert(asl::is_same<asl::remove_ref_t<int&&>, int>);
static_assert(asl::is_same<asl::remove_ref_t<const int&>, const int>);
static_assert(asl::is_same<asl::remove_ref_t<int*>, int*>);
static_assert(asl::is_same<asl::remove_ref_t<int (int)>, int (int)>);
static_assert(asl::is_same<asl::remove_ref_t<int (int) &>, int (int) &>);
static_assert(asl::is_same<asl::remove_ref_t<int (&)(int)>, int (int)>);
static_assert(asl::is_same<asl::remove_ref_t<int[]>, int[]>);
static_assert(asl::is_same<asl::remove_ref_t<int (&&)[]>, int[]>);
static_assert(asl::is_same<asl::remove_ref_t<int (&)[5]>, int[5]>);

static_assert(asl::is_same<asl::remove_cvref_t<int>, int>);
static_assert(asl::is_same<asl::remove_cvref_t<const int>, int>);
static_assert(asl::is_same<asl::remove_cvref_t<int&>, int>);
static_assert(asl::is_same<asl::remove_cvref_t<volatile int&>, int>);
static_assert(asl::is_same<asl::remove_cvref_t<const volatile int&&>, int>);

static_assert(asl::is_same<asl::add_lref_t<int>, int&>);
static_assert(asl::is_same<asl::add_lref_t<int&>, int&>);
static_assert(asl::is_same<asl::add_lref_t<int&&>, int&>);
static_assert(asl::is_same<asl::add_lref_t<void>, void>);
static_assert(asl::is_same<asl::add_lref_t<int[]>, int (&)[]>);
static_assert(asl::is_same<asl::add_lref_t<int[5]>, int (&)[5]>);
static_assert(asl::is_same<asl::add_lref_t<int (int)>, int (&)(int)>);
static_assert(asl::is_same<asl::add_lref_t<int (&)(int)>, int (&)(int)>);
static_assert(asl::is_same<asl::add_lref_t<int (*)(int)>, int (*&)(int)>);

static_assert(asl::is_same<asl::add_rref_t<int>, int&&>);
static_assert(asl::is_same<asl::add_rref_t<int&>, int&>);
static_assert(asl::is_same<asl::add_rref_t<int&&>, int&&>);
static_assert(asl::is_same<asl::add_rref_t<void>, void>);
static_assert(asl::is_same<asl::add_rref_t<int[]>, int (&&)[]>);
static_assert(asl::is_same<asl::add_rref_t<int[5]>, int (&&)[5]>);
static_assert(asl::is_same<asl::add_rref_t<int (int)>, int (&&)(int)>);
static_assert(asl::is_same<asl::add_rref_t<int (&)(int)>, int (&)(int)>);
static_assert(asl::is_same<asl::add_rref_t<int (*)(int)>, int (*&&)(int)>);

static_assert(asl::is_same<asl::remove_ptr_t<int>, int>);
static_assert(asl::is_same<asl::remove_ptr_t<const int>, const int>);
static_assert(asl::is_same<asl::remove_ptr_t<int&>, int&>);
static_assert(asl::is_same<asl::remove_ptr_t<int&&>, int&&>);
static_assert(asl::is_same<asl::remove_ptr_t<int*&>, int*&>);
static_assert(asl::is_same<asl::remove_ptr_t<int*>, int>);
static_assert(asl::is_same<asl::remove_ptr_t<int**>, int*>);
static_assert(asl::is_same<asl::remove_ptr_t<const int*>, const int>);
static_assert(asl::is_same<asl::remove_ptr_t<int* const>, int>);
static_assert(asl::is_same<asl::remove_ptr_t<const int&>, const int&>);
static_assert(asl::is_same<asl::remove_ptr_t<int (int)>, int (int)>);
static_assert(asl::is_same<asl::remove_ptr_t<int (*)(int)>, int (int)>);

static_assert(asl::is_same<asl::add_ptr_t<int>, int*>);
static_assert(asl::is_same<asl::add_ptr_t<const int*>, const int**>);
static_assert(asl::is_same<asl::add_ptr_t<int&>, int*>);
static_assert(asl::is_same<asl::add_ptr_t<int&&>, int*>);
static_assert(asl::is_same<asl::add_ptr_t<int*&>, int**>);
static_assert(asl::is_same<asl::add_ptr_t<int* const>, int* const*>);
static_assert(asl::is_same<asl::add_ptr_t<int (int)>, int (*)(int)>);
static_assert(asl::is_same<asl::add_ptr_t<int (*)(int)>, int (**)(int)>);

enum UintEnum : unsigned int {}; // NOLINT
enum class Int64Enum : int64_t {};

static_assert(asl::is_same<asl::make_unsigned_t<int>, unsigned int>);
static_assert(asl::is_same<asl::make_unsigned_t<const volatile int>, const volatile unsigned int>);
static_assert(asl::is_same<asl::make_unsigned_t<short int>, unsigned short int>);
static_assert(asl::is_same<asl::make_unsigned_t<volatile short int>, volatile unsigned short int>);
static_assert(asl::is_same<asl::make_unsigned_t<unsigned long>, unsigned long>);
static_assert(asl::is_same<asl::make_unsigned_t<const unsigned long>, const unsigned long>);
static_assert(asl::is_same<asl::make_unsigned_t<unsigned char>, unsigned char>);

static_assert(asl::is_unsigned<asl::make_unsigned_t<UintEnum>>);
static_assert(asl::is_unsigned<asl::make_unsigned_t<Int64Enum>>);
static_assert(sizeof(asl::make_unsigned_t<UintEnum>) == sizeof(UintEnum));
static_assert(sizeof(asl::make_unsigned_t<Int64Enum>) == sizeof(Int64Enum));

static_assert(asl::is_same<asl::make_unsigned_t<signed char>, unsigned char>);
static_assert(asl::is_same<asl::make_unsigned_t<signed short int>, unsigned short int>);
static_assert(asl::is_same<asl::make_unsigned_t<signed int>, unsigned int>);
static_assert(asl::is_same<asl::make_unsigned_t<signed long int>, unsigned long int>);
static_assert(asl::is_same<asl::make_unsigned_t<signed long long int>, unsigned long long int>);

static_assert(asl::is_same<asl::make_unsigned_t<char>, unsigned char>);
static_assert(sizeof(asl::make_unsigned_t<char>) == sizeof(char));
static_assert(sizeof(asl::make_unsigned_t<wchar_t>) == sizeof(wchar_t));
static_assert(sizeof(asl::make_unsigned_t<char8_t>) == sizeof(char8_t));
static_assert(sizeof(asl::make_unsigned_t<char16_t>) == sizeof(char16_t));
static_assert(sizeof(asl::make_unsigned_t<char32_t>) == sizeof(char32_t));
static_assert(sizeof(asl::make_unsigned_t<UintEnum>) == sizeof(UintEnum));
static_assert(sizeof(asl::make_unsigned_t<Int64Enum>) == sizeof(Int64Enum));
static_assert(asl::is_same<asl::make_unsigned_t<volatile char32_t>, volatile unsigned int>);
static_assert(asl::is_same<asl::make_unsigned_t<const volatile char16_t>, const volatile unsigned short>);

static_assert(asl::is_same<asl::make_signed_t<unsigned int>, signed int>);
static_assert(asl::is_same<asl::make_signed_t<const volatile int>, const volatile int>);
static_assert(asl::is_same<asl::make_signed_t<unsigned short int>, short int>);
static_assert(asl::is_same<asl::make_signed_t<volatile unsigned short int>, volatile short int>);
static_assert(asl::is_same<asl::make_signed_t<unsigned long>, long>);
static_assert(asl::is_same<asl::make_signed_t<const unsigned long>, const long>);
static_assert(asl::is_same<asl::make_signed_t<unsigned char>, signed char>);

static_assert(asl::is_signed<asl::make_signed_t<UintEnum>>);
static_assert(asl::is_signed<asl::make_signed_t<Int64Enum>>);
static_assert(sizeof(asl::make_signed_t<UintEnum>) == sizeof(UintEnum));
static_assert(sizeof(asl::make_signed_t<Int64Enum>) == sizeof(Int64Enum));

static_assert(asl::is_same<asl::make_signed_t<unsigned char>, signed char>);
static_assert(asl::is_same<asl::make_signed_t<unsigned short int>, signed short int>);
static_assert(asl::is_same<asl::make_signed_t<unsigned int>, signed int>);
static_assert(asl::is_same<asl::make_signed_t<unsigned long int>, signed long int>);
static_assert(asl::is_same<asl::make_signed_t<unsigned long long int>, signed long long int>);

static_assert(asl::is_same<asl::make_signed_t<char>, signed char>);
static_assert(sizeof(asl::make_signed_t<char>) == sizeof(char));
static_assert(sizeof(asl::make_signed_t<wchar_t>) == sizeof(wchar_t));
static_assert(sizeof(asl::make_signed_t<char8_t>) == sizeof(char8_t));
static_assert(sizeof(asl::make_signed_t<char16_t>) == sizeof(char16_t));
static_assert(sizeof(asl::make_signed_t<char32_t>) == sizeof(char32_t));
static_assert(sizeof(asl::make_signed_t<UintEnum>) == sizeof(UintEnum));
static_assert(sizeof(asl::make_signed_t<Int64Enum>) == sizeof(Int64Enum));
static_assert(asl::is_same<asl::make_signed_t<volatile char32_t>, volatile signed int>);
static_assert(asl::is_same<asl::make_signed_t<const volatile char16_t>, const volatile signed short>);

static_assert(asl::rank_v<int> == 0);
static_assert(asl::rank_v<int[2]> == 1);
static_assert(asl::rank_v<int[][4]> == 2);

static_assert(asl::extent_v<int> == 0);
static_assert(asl::extent_v<int[]> == 0);
static_assert(asl::extent_v<int[2]> == 2);
static_assert(asl::extent_v<int[2], 0> == 2);
static_assert(asl::extent_v<int[2][4]> == 2);
static_assert(asl::extent_v<int[][4]> == 0);
static_assert(asl::extent_v<int, 1> == 0);
static_assert(asl::extent_v<int[2], 1> == 0);
static_assert(asl::extent_v<int[2][4], 1> == 4);
static_assert(asl::extent_v<int[2][4][6], 0> == 2);
static_assert(asl::extent_v<int[2][4][6], 1> == 4);
static_assert(asl::extent_v<int[2][4][6], 2> == 6);
static_assert(asl::extent_v<int[2][4][6], 3> == 0);
static_assert(asl::extent_v<int[][4], 1> == 4);

static_assert(asl::has_unique_object_representations_v<int>);
static_assert(asl::has_unique_object_representations_v<E>);
static_assert(asl::has_unique_object_representations_v<Ec>);
static_assert(asl::has_unique_object_representations_v<bool>);
static_assert(asl::has_unique_object_representations_v<char>);
static_assert(asl::has_unique_object_representations_v<void*>);
static_assert(asl::has_unique_object_representations_v<char32_t>);
static_assert(asl::has_unique_object_representations_v<int[5]>);
static_assert(asl::has_unique_object_representations_v<int[]>);
static_assert(asl::has_unique_object_representations_v<int (*)(int)>);
static_assert(!asl::has_unique_object_representations_v<void>);
static_assert(!asl::has_unique_object_representations_v<float>);
static_assert(!asl::has_unique_object_representations_v<int&>);
static_assert(!asl::has_unique_object_representations_v<int (int)>);

namespace base_of_tests
{

class Base {};
class NotBase {};
class ChildPublic : public Base {};
class ChildPrivate : private Base {};
class ChildChildPublic : public ChildPublic {};

static_assert(asl::is_base_of<const Base, Base>);
static_assert(asl::is_base_of<Base, const Base>);
static_assert(asl::is_base_of<Base, ChildPublic>);
static_assert(asl::is_base_of<Base, volatile ChildPrivate>);
static_assert(asl::is_base_of<const volatile Base, ChildChildPublic>);
static_assert(!asl::is_base_of<ChildPublic, const Base>);
static_assert(!asl::is_base_of<volatile NotBase, ChildPublic>);

struct B {};
struct B1 : B {};
struct B2 : B {};
struct D : private B1, private B2 {};

static_assert(asl::is_base_of<B, D>);
static_assert(asl::is_base_of<const B, D>);
static_assert(asl::is_base_of<B, const D>);
static_assert(asl::is_base_of<B, const B>);
static_assert(!asl::is_base_of<D, B>);
static_assert(!asl::is_base_of<B&, D&>);
static_assert(!asl::is_base_of<B[3], D[3]>);
static_assert(!asl::is_base_of<int, int>);

} // namespace base_of_tests

namespace convertible_tests
{

class A {};
class B : public A {};
class C {};
class D { public: operator C() { return c; } C c; }; // NOLINT
class E { public: template<class T> E(T&&) {} }; // NOLINT

static_assert(asl::is_convertible<B*, A*>);
static_assert(!asl::is_convertible<A*, B*>);
static_assert(asl::is_convertible<D, C>);
static_assert(!asl::is_convertible<B*, C*>);
static_assert(asl::is_convertible<A, E>);
static_assert(!asl::is_convertible<A, void>);
static_assert(!asl::is_convertible<void, A>);
static_assert(asl::is_convertible<void, void>);

} // namespace convertible_tests


static_assert(asl::is_same<asl::decay_t<int>, int>);
static_assert(!asl::is_same<asl::decay_t<int>, float>);
static_assert(asl::is_same<asl::decay_t<int&>, int>);
static_assert(asl::is_same<asl::decay_t<int&&>, int>);
static_assert(asl::is_same<asl::decay_t<const int&>, int>);
static_assert(asl::is_same<asl::decay_t<int[2]>, int*>);
static_assert(!asl::is_same<asl::decay_t<int[4][2]>, int*>);
static_assert(!asl::is_same<asl::decay_t<int[4][2]>, int**>);
static_assert(asl::is_same<asl::decay_t<int[4][2]>, int(*)[2]>);
static_assert(asl::is_same<asl::decay_t<int(int)>, int(*)(int)>);

namespace invoke_tests
{

struct HasFunction
{
    int x{};
    void do_something(int, float) const {}
    int& do_something2(int, float) & { return x; }
};

struct HasFunction2 : public HasFunction {};

struct HasMember
{
    int member{};
    int member_array[4]{};
    void (*member_fn)(){};
};

struct HasMember2 : public HasMember {};

struct Functor
{
    int64_t operator()() { return 35; }
    int operator()(int x) { return x; }
};

[[maybe_unused]] static int some_func0() { return 1; }
[[maybe_unused]] static int some_func1(int x) { return x + 1; } // NOLINT
[[maybe_unused]] static float some_func1(float x) { return x + 1; }
[[maybe_unused]] static int some_func2(int x, int b) { return x + b; }
[[maybe_unused]] static void some_func3() {} // NOLINT

static_assert(asl::is_invocable<decltype(&HasFunction::do_something), const HasFunction, int, float>);
static_assert(asl::is_invocable<decltype(&HasFunction::do_something), HasFunction&&, int, float>);
static_assert(!asl::is_invocable<decltype(&HasFunction::do_something2), const HasFunction&, int, float>);
static_assert(!asl::is_invocable<decltype(&HasFunction::do_something), const HasMember&, int, float>);
static_assert(asl::is_invocable<decltype(&HasFunction::do_something2), HasFunction&, int, float>);
static_assert(!asl::is_invocable<decltype(&HasFunction::do_something2), HasFunction&&, int, float>);
static_assert(asl::is_invocable<decltype(&HasFunction::do_something2), HasFunction&, int, float>);
static_assert(asl::is_invocable<decltype(&HasFunction::do_something2), HasFunction2&, int, float>);
static_assert(!asl::is_invocable<decltype(&HasFunction::do_something2), HasFunction&, int>);

static_assert(asl::is_invocable<decltype(&HasFunction::do_something), const HasFunction*, int, float>);
static_assert(asl::is_invocable<decltype(&HasFunction::do_something2), HasFunction*, int, float>);
static_assert(asl::is_invocable<decltype(&HasFunction::do_something2), HasFunction*, int, float>);
static_assert(asl::is_invocable<decltype(&HasFunction::do_something2), HasFunction2*, int, float>);
static_assert(!asl::is_invocable<decltype(&HasFunction::do_something2), HasMember*, int, float>);

static_assert(asl::is_invocable<decltype(&HasMember::member), HasMember>);
static_assert(asl::is_invocable<decltype(&HasMember::member), HasMember*>);
static_assert(!asl::is_invocable<decltype(&HasMember::member), HasFunction>);
static_assert(!asl::is_invocable<decltype(&HasMember::member), HasFunction*>);
static_assert(asl::is_invocable<decltype(&HasMember::member), const HasMember>);
static_assert(asl::is_invocable<decltype(&HasMember::member), HasMember&>);
static_assert(asl::is_invocable<decltype(&HasMember::member), HasMember2>);
static_assert(asl::is_invocable<decltype(&HasMember::member), const HasMember2>);
static_assert(asl::is_invocable<decltype(&HasMember::member_array), HasMember>);
static_assert(asl::is_invocable<decltype(&HasMember::member_fn), HasMember>);

static_assert(asl::is_invocable<Functor>);
static_assert(asl::is_invocable<Functor, int>);
static_assert(asl::is_invocable<Functor, short>);
static_assert(!asl::is_invocable<Functor, char*>);
static_assert(asl::is_invocable<decltype(some_func0)>);
static_assert(asl::is_invocable<decltype(&some_func0)>);
static_assert(asl::is_invocable<decltype(&some_func2), int, int>);
static_assert(!asl::is_invocable<decltype(&some_func2), int, void>);
static_assert(asl::is_invocable<decltype(some_func3)>);

static_assert(asl::is_same<void, asl::invoke_result_t<decltype(&HasFunction::do_something), const HasFunction, int, float>>);
static_assert(asl::is_same<void, asl::invoke_result_t<decltype(&HasFunction::do_something), HasFunction&&, int, float>>);
static_assert(asl::is_same<int&, asl::invoke_result_t<decltype(&HasFunction::do_something2), HasFunction&, int, float>>);
static_assert(asl::is_same<int&, asl::invoke_result_t<decltype(&HasFunction::do_something2), HasFunction&, int, float>>);
static_assert(asl::is_same<int&, asl::invoke_result_t<decltype(&HasFunction::do_something2), HasFunction2&, int, float>>);

static_assert(asl::is_same<void, asl::invoke_result_t<decltype(&HasFunction::do_something), const HasFunction*, int, float>>);
static_assert(asl::is_same<int&, asl::invoke_result_t<decltype(&HasFunction::do_something2), HasFunction*, int, float>>);
static_assert(asl::is_same<int&, asl::invoke_result_t<decltype(&HasFunction::do_something2), HasFunction*, int, float>>);
static_assert(asl::is_same<int&, asl::invoke_result_t<decltype(&HasFunction::do_something2), HasFunction2*, int, float>>);

static_assert(asl::is_same<int&&, asl::invoke_result_t<decltype(&HasMember::member), HasMember>>);
static_assert(asl::is_same<int&, asl::invoke_result_t<decltype(&HasMember::member), HasMember*>>);
static_assert(asl::is_same<const int&&, asl::invoke_result_t<decltype(&HasMember::member), const HasMember>>);
static_assert(asl::is_same<int&, asl::invoke_result_t<decltype(&HasMember::member), HasMember&>>);
static_assert(asl::is_same<int&&, asl::invoke_result_t<decltype(&HasMember::member), HasMember2>>);
static_assert(asl::is_same<const int&, asl::invoke_result_t<decltype(&HasMember::member), const HasMember2&>>);
static_assert(asl::is_same<int (&&)[4], asl::invoke_result_t<decltype(&HasMember::member_array), HasMember>>);
static_assert(asl::is_same<void (*&&)(), asl::invoke_result_t<decltype(&HasMember::member_fn), HasMember>>);

static_assert(asl::is_same<int64_t, asl::invoke_result_t<Functor>>);
static_assert(asl::is_same<int, asl::invoke_result_t<Functor, int>>);
static_assert(asl::is_same<int, asl::invoke_result_t<decltype(some_func0)>>);
static_assert(asl::is_same<int, asl::invoke_result_t<decltype(&some_func0)>>);
static_assert(asl::is_same<int, asl::invoke_result_t<decltype(&some_func2), int, int>>);
static_assert(asl::is_same<void, asl::invoke_result_t<decltype(some_func3)>>);

static_assert(asl::is_invocable_r<int, int (int), int>);
static_assert(asl::is_invocable_r<short, int (int), int>);
static_assert(asl::is_invocable_r<void, int (int), int>);
static_assert(asl::is_invocable_r<void, void (int), int>);
static_assert(!asl::is_invocable_r<int, void (int), int>);
static_assert(!asl::is_invocable_r<char*, int (int), int>);

static void tests()
{
    HasMember m{};
    m.member = 98;

    test_assert(asl::invoke(&HasMember::member, m) == 98);
    test_assert(asl::invoke_r<long long>(&HasMember::member, &m) == 98);

    test_assert(asl::invoke(Functor{}) == 35);
    test_assert(asl::invoke_r<int64_t>(Functor{}, 82) == 82);

    test_assert(asl::invoke(some_func0) == 1);
    test_assert(asl::invoke_r<void>(some_func0); true);
    test_assert(asl::invoke(some_func2, 5, 1) == 6);
    test_assert(asl::invoke(some_func3); true);
    test_assert(asl::invoke_r<void>(some_func3); true);

    test_assert(asl::invoke([&m](int a) { return m.member + a; }, 4) == 102);
}

} // namespace invoke_tests

static_assert(asl::is_same<int&&, decltype(std::forward_like<short>(asl::declval<int>()))>);
static_assert(asl::is_same<int&, decltype(std::forward_like<short&>(asl::declval<int>()))>);
static_assert(asl::is_same<const int&, decltype(std::forward_like<const short&>(asl::declval<int>()))>);
static_assert(asl::is_same<int&&, decltype(std::forward_like<short&&>(asl::declval<int>()))>);

struct CA {};
struct CB {};
struct CC {};

template<>
struct asl::common_type<CA, CB> : type_identity<CC> {};

namespace common_type_tests
{

class Base {};
class Derived : public Base {};

struct ConvertInt { operator const int(); }; // NOLINT
struct ConvertConstInt { operator int(); }; // NOLINT

struct ConvertIntPtr { operator const int*(); }; // NOLINT
struct ConvertConstIntPtr { operator int*(); }; // NOLINT

static_assert(asl::is_same<int, asl::common_type_t<int>>);
static_assert(asl::is_same<int, asl::common_type_t<const int>>);
static_assert(asl::is_same<int, asl::common_type_t<int, int>>);
static_assert(asl::is_same<int, asl::common_type_t<int&, int>>);
static_assert(asl::is_same<int, asl::common_type_t<int&&, int&>>);
static_assert(asl::is_same<int, asl::common_type_t<int, int&&>>);
static_assert(asl::is_same<int, asl::common_type_t<const int, int>>);
static_assert(asl::is_same<E, asl::common_type_t<E, E>>);
static_assert(asl::is_same<int, asl::common_type_t<E, E, int>>);
static_assert(asl::is_same<Ec, asl::common_type_t<Ec, Ec, Ec>>);
static_assert(asl::is_same<int, asl::common_type_t<ConvertInt, ConvertConstInt>>);
static_assert(asl::is_same<const int*, asl::common_type_t<ConvertIntPtr, ConvertConstIntPtr>>);
static_assert(asl::is_same<const int*, asl::common_type_t<int* const, const int*>>);
static_assert(asl::is_same<Base, asl::common_type_t<Base, Derived>>);
static_assert(asl::is_same<Base*, asl::common_type_t<Base*, Derived*>>);
static_assert(asl::is_same<Base, asl::common_type_t<Base, Derived&>>);
static_assert(asl::is_same<Base, asl::common_type_t<Base&, Derived&>>);
static_assert(asl::is_same<Base, asl::common_type_t<const Base&, Derived&>>);
static_assert(asl::is_same<int*, asl::common_type_t<int*, int[]>>);
static_assert(asl::is_same<int*, asl::common_type_t<int[18], int[]>>);
static_assert(asl::is_same<int*, asl::common_type_t<int[18], int[456]>>);
static_assert(asl::is_same<CC, asl::common_type_t<CA, CB>>);

} // namespace common_type_tests

namespace common_reference_tests
{

struct A {};
struct B {};
struct C {};

struct D {};
struct E {};
struct F {};

} // namespace common_reference_tests

template<
    template<typename> typename AQual,
    template<typename> typename BQual>
struct asl::basic_common_reference<common_reference_tests::A, common_reference_tests::B, AQual, BQual>
    : asl::type_identity<BQual<AQual<common_reference_tests::C>>> {};

template<> struct asl::common_type<common_reference_tests::D, common_reference_tests::E> : asl::type_identity<common_reference_tests::F> {};

namespace common_reference_tests
{

struct A1 {};
struct B1 { B1(A1); }; // NOLINT
static_assert(asl::is_same<asl::common_reference_t<A1&, B1&&>, B1> );

static_assert(asl::is_same<asl::common_reference_t<int>, int> );
static_assert(asl::is_same<asl::common_reference_t<int&>, int&> );
static_assert(asl::is_same<asl::common_reference_t<void>, void> );
static_assert(asl::is_same<asl::common_reference_t<const void>, const void> );
static_assert(asl::is_same<asl::common_reference_t<const void, void>, void> );
static_assert(asl::is_same<asl::common_reference_t<void(*const)(), void(*)()>, void(*)()> );
static_assert(asl::is_same<asl::common_reference_t<int, int>, int> );
static_assert(asl::is_same<asl::common_reference_t<int&, int>, int> );
static_assert(asl::is_same<asl::common_reference_t<int, int&>, int> );
static_assert(asl::is_same<asl::common_reference_t<int&&, int>, int> );
static_assert(asl::is_same<asl::common_reference_t<int&, int&>, int&> );
static_assert(asl::is_same<asl::common_reference_t<int&, int&&>, const int&> );
static_assert(asl::is_same<asl::common_reference_t<int&&, int&>, const int&> );
static_assert(asl::is_same<asl::common_reference_t<int&&, int&&>, int&&> );
static_assert(asl::is_same<asl::common_reference_t<int&&, const int&&>, const int&&> );
static_assert(asl::is_same<asl::common_reference_t<int&, int&, int&&>, const int&> );
static_assert(asl::is_same<asl::common_reference_t<int&&, int&, int&>, const int&> );
static_assert(asl::is_same<asl::common_reference_t<char&, int&>, int> );
static_assert(asl::is_same<asl::common_reference_t<long&, int&>, long> );

static_assert(asl::is_same<asl::common_reference_t<A, B>, C> );
static_assert(asl::is_same<asl::common_reference_t<A&, B>, C&> );
static_assert(asl::is_same<asl::common_reference_t<A&, const B>, C&> );
static_assert(asl::is_same<asl::common_reference_t<const A, B&>, const C&> );
static_assert(asl::is_same<asl::common_reference_t<const A&, B&&>, const C&> );
static_assert(asl::is_same<asl::common_reference_t<const A, B&&>, const C&&> );

static_assert(asl::is_same<asl::common_reference_t<D, E>, F> );
static_assert(asl::is_same<asl::common_reference_t<D&, E>, F> );
static_assert(asl::is_same<asl::common_reference_t<D&, E&&>, F> );

} // namespace common_reference_tests

namespace deref_tests
{

static_assert(asl::derefs_as<int, int>);
static_assert(asl::derefs_as<int*, int>);
static_assert(!asl::derefs_as<const int*, int>);
static_assert(asl::derefs_as<int&, int>);
static_assert(!asl::derefs_as<const int&, int>);

class Base {};
class Derived : public Base {};

static_assert(asl::derefs_as<Derived, Base>);
static_assert(!asl::derefs_as<Base, Derived>);
static_assert(asl::derefs_as<Derived*, Base>);
static_assert(asl::derefs_as<Derived&, Base>);
static_assert(!asl::derefs_as<Base&, Derived>);

static void wants_int(int) {}
static void wants_base(const Base&) {}
static void wants_base_ptr(Base*) {}

static void tests()
{
    int a = 4;

    wants_int(asl::deref<int>(5));
    wants_int(asl::deref<int>(a));
    wants_int(asl::deref<int>(&a));

    Derived c{};

    wants_base(asl::deref<Base>(Derived{}));
    wants_base(asl::deref<Base>(c));
    wants_base(asl::deref<Base>(&c));

    wants_base_ptr(&asl::deref<Base>(c));
    wants_base_ptr(&asl::deref<Base>(&c));
}

} // namespace deref_tests

int main()
{
    invoke_tests::tests();
    deref_tests::tests();
    return 0;
}
