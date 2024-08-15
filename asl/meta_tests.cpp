#include "asl/meta.hpp"
#include "asl/test_types.hpp"

struct EmptyStruct {};
struct Struct { int b: 4; };
union Union {};

enum Enum { EnumVariant = 0, };
enum EnumClass { Variant = 0, };

static_assert(!asl::is_same<long, short>);
static_assert(asl::is_same<int, int>);

static_assert(asl::is_same<asl::un_ref_t<int>, int>);
static_assert(asl::is_same<asl::un_ref_t<int&>, int>);
static_assert(asl::is_same<asl::un_ref_t<int&&>, int>);

static_assert(asl::is_same<asl::un_const_t<int>, int>);
static_assert(asl::is_same<asl::un_const_t<const int>, int>);

static_assert(asl::is_same<asl::un_volatile_t<int>, int>);
static_assert(asl::is_same<asl::un_volatile_t<volatile int>, int>);

static_assert(asl::is_same<asl::un_qual_t<int>, int>);
static_assert(asl::is_same<asl::un_qual_t<volatile int>, int>);
static_assert(asl::is_same<asl::un_qual_t<const int>, int>);
static_assert(asl::is_same<asl::un_qual_t<const volatile int>, int>);

static_assert(asl::is_same<asl::un_qualref_t<int&>, int>);
static_assert(asl::is_same<asl::un_qualref_t<volatile int&&>, int>);
static_assert(asl::is_same<asl::un_qualref_t<const int>, int>);
static_assert(asl::is_same<asl::un_qualref_t<const volatile int&>, int>);

static_assert(!asl::is_const<void>);
static_assert(asl::is_const<const int>);
static_assert(!asl::is_const<const int*>);
static_assert(asl::is_const<int* const>);

static_assert(asl::is_void<void>);
static_assert(asl::is_void<const void>);
static_assert(!asl::is_void<int>);

static_assert(asl::is_nullptr<asl::nullptr_t>);
static_assert(asl::is_nullptr<decltype(nullptr)>);
static_assert(!asl::is_nullptr<decltype(0)>);
static_assert(!asl::is_nullptr<void*>);
static_assert(!asl::is_ptr<decltype(nullptr)>);

static_assert(asl::is_integral<int>);
static_assert(asl::is_integral<decltype(Struct::b)>);
static_assert(asl::is_integral<const int>);
static_assert(asl::is_integral<bool>);
static_assert(!asl::is_integral<int*>);
static_assert(!asl::is_integral<float>);
static_assert(!asl::is_integral<void>);
static_assert(!asl::is_integral<asl::nullptr_t>);
static_assert(!asl::is_integral<int&>);
static_assert(!asl::is_integral<int&&>);
static_assert(!asl::is_integral<int[25]>);
static_assert(!asl::is_integral<int[]>);
static_assert(!asl::is_integral<int()>);
static_assert(!asl::is_integral<Struct>);
static_assert(!asl::is_integral<Union>);
static_assert(!asl::is_integral<Enum>);
static_assert(!asl::is_integral<EnumClass>);

static_assert(asl::is_floating_point<float>);
static_assert(!asl::is_floating_point<void>);
static_assert(!asl::is_floating_point<asl::nullptr_t>);
static_assert(!asl::is_floating_point<int>);
static_assert(!asl::is_floating_point<decltype(Struct::b)>);
static_assert(!asl::is_floating_point<const int>);
static_assert(!asl::is_floating_point<bool>);
static_assert(!asl::is_floating_point<int*>);
static_assert(!asl::is_floating_point<int&>);
static_assert(!asl::is_floating_point<int&&>);
static_assert(!asl::is_floating_point<int[25]>);
static_assert(!asl::is_floating_point<int[]>);
static_assert(!asl::is_floating_point<int()>);
static_assert(!asl::is_floating_point<Struct>);
static_assert(!asl::is_floating_point<Union>);
static_assert(!asl::is_floating_point<Enum>);
static_assert(!asl::is_floating_point<EnumClass>);

static_assert(asl::is_arithmetic<float>);
static_assert(!asl::is_arithmetic<void>);
static_assert(!asl::is_arithmetic<asl::nullptr_t>);
static_assert(asl::is_arithmetic<int>);
static_assert(asl::is_arithmetic<decltype(Struct::b)>);
static_assert(asl::is_arithmetic<const int>);
static_assert(asl::is_arithmetic<bool>);
static_assert(!asl::is_arithmetic<int*>);
static_assert(!asl::is_arithmetic<int&>);
static_assert(!asl::is_arithmetic<int&&>);
static_assert(!asl::is_arithmetic<int[25]>);
static_assert(!asl::is_arithmetic<int[]>);
static_assert(!asl::is_arithmetic<int()>);
static_assert(!asl::is_arithmetic<Struct>);
static_assert(!asl::is_arithmetic<Union>);
static_assert(!asl::is_arithmetic<Enum>);
static_assert(!asl::is_arithmetic<EnumClass>);

static_assert(asl::is_fundamental<float>);
static_assert(asl::is_fundamental<void>);
static_assert(asl::is_fundamental<asl::nullptr_t>);
static_assert(asl::is_fundamental<int>);
static_assert(asl::is_fundamental<decltype(Struct::b)>);
static_assert(asl::is_fundamental<const int>);
static_assert(asl::is_fundamental<bool>);
static_assert(!asl::is_fundamental<int*>);
static_assert(!asl::is_fundamental<int&>);
static_assert(!asl::is_fundamental<int&&>);
static_assert(!asl::is_fundamental<int[25]>);
static_assert(!asl::is_fundamental<int[]>);
static_assert(!asl::is_fundamental<int()>);
static_assert(!asl::is_fundamental<Struct>);
static_assert(!asl::is_fundamental<Union>);
static_assert(!asl::is_fundamental<Enum>);
static_assert(!asl::is_fundamental<EnumClass>);

static_assert(!asl::is_array<float>);
static_assert(!asl::is_array<void>);
static_assert(!asl::is_array<asl::nullptr_t>);
static_assert(!asl::is_array<int>);
static_assert(!asl::is_array<decltype(Struct::b)>);
static_assert(!asl::is_array<const int>);
static_assert(!asl::is_array<bool>);
static_assert(!asl::is_array<int*>);
static_assert(!asl::is_array<int&>);
static_assert(!asl::is_array<int&&>);
static_assert(asl::is_array<int[25]>);
static_assert(asl::is_array<int[]>);
static_assert(asl::is_array<int*[]>);
static_assert(!asl::is_array<int(&)[]>);
static_assert(!asl::is_array<int(*)[]>);
static_assert(asl::is_array<int[]>);
static_assert(asl::is_array<Struct[4]>);
static_assert(asl::is_array<Union[]>);
static_assert(asl::is_array<Enum[]>);
static_assert(!asl::is_array<int()>);
static_assert(!asl::is_array<Struct>);
static_assert(!asl::is_array<Union>);
static_assert(!asl::is_array<Enum>);
static_assert(!asl::is_array<EnumClass>);

static_assert(!asl::is_class<float>);
static_assert(!asl::is_class<void>);
static_assert(!asl::is_class<asl::nullptr_t>);
static_assert(!asl::is_class<int>);
static_assert(!asl::is_class<decltype(Struct::b)>);
static_assert(!asl::is_class<const int>);
static_assert(!asl::is_class<bool>);
static_assert(!asl::is_class<int*>);
static_assert(!asl::is_class<int&>);
static_assert(!asl::is_class<int&&>);
static_assert(!asl::is_class<int[25]>);
static_assert(!asl::is_class<int[]>);
static_assert(!asl::is_class<int*[]>);
static_assert(!asl::is_class<int(&)[]>);
static_assert(!asl::is_class<int(*)[]>);
static_assert(!asl::is_class<int[]>);
static_assert(!asl::is_class<Struct[4]>);
static_assert(!asl::is_class<Union[]>);
static_assert(!asl::is_class<Enum[]>);
static_assert(!asl::is_class<int()>);
static_assert(asl::is_class<Struct>);
static_assert(asl::is_class<const Struct>);
static_assert(!asl::is_class<Struct&>);
static_assert(!asl::is_class<Struct*>);
static_assert(!asl::is_class<Union>);
static_assert(!asl::is_class<Enum>);
static_assert(!asl::is_class<EnumClass>);

static_assert(!asl::is_union<float>);
static_assert(!asl::is_union<void>);
static_assert(!asl::is_union<asl::nullptr_t>);
static_assert(!asl::is_union<int>);
static_assert(!asl::is_union<decltype(Struct::b)>);
static_assert(!asl::is_union<const int>);
static_assert(!asl::is_union<bool>);
static_assert(!asl::is_union<int*>);
static_assert(!asl::is_union<int&>);
static_assert(!asl::is_union<int&&>);
static_assert(!asl::is_union<int[25]>);
static_assert(!asl::is_union<int[]>);
static_assert(!asl::is_union<int*[]>);
static_assert(!asl::is_union<int(&)[]>);
static_assert(!asl::is_union<int(*)[]>);
static_assert(!asl::is_union<int[]>);
static_assert(!asl::is_union<Struct[4]>);
static_assert(!asl::is_union<Union[]>);
static_assert(!asl::is_union<Enum[]>);
static_assert(!asl::is_union<int()>);
static_assert(!asl::is_union<Struct>);
static_assert(asl::is_union<Union>);
static_assert(asl::is_union<const Union>);
static_assert(!asl::is_union<Union*>);
static_assert(!asl::is_union<Union&>);
static_assert(!asl::is_union<Enum>);
static_assert(!asl::is_union<EnumClass>);

static_assert(!asl::is_enum<float>);
static_assert(!asl::is_enum<void>);
static_assert(!asl::is_enum<asl::nullptr_t>);
static_assert(!asl::is_enum<int>);
static_assert(!asl::is_enum<decltype(Struct::b)>);
static_assert(!asl::is_enum<const int>);
static_assert(!asl::is_enum<bool>);
static_assert(!asl::is_enum<int*>);
static_assert(!asl::is_enum<int&>);
static_assert(!asl::is_enum<int&&>);
static_assert(!asl::is_enum<int[25]>);
static_assert(!asl::is_enum<int[]>);
static_assert(!asl::is_enum<int*[]>);
static_assert(!asl::is_enum<int(&)[]>);
static_assert(!asl::is_enum<int(*)[]>);
static_assert(!asl::is_enum<int[]>);
static_assert(!asl::is_enum<Struct[4]>);
static_assert(!asl::is_enum<Union[]>);
static_assert(!asl::is_enum<Enum[]>);
static_assert(!asl::is_enum<int()>);
static_assert(!asl::is_enum<Struct>);
static_assert(!asl::is_enum<const Struct>);
static_assert(!asl::is_enum<Struct&>);
static_assert(!asl::is_enum<Struct*>);
static_assert(!asl::is_enum<Union>);
static_assert(asl::is_enum<Enum>);
static_assert(asl::is_enum<EnumClass>);
static_assert(!asl::is_enum<Enum&>);
static_assert(!asl::is_enum<EnumClass*>);
static_assert(asl::is_enum<const Enum>);
static_assert(asl::is_enum<volatile EnumClass>);

static_assert(!asl::is_ptr<float>);
static_assert(!asl::is_ptr<void>);
static_assert(!asl::is_ptr<asl::nullptr_t>);
static_assert(!asl::is_ptr<int>);
static_assert(!asl::is_ptr<decltype(Struct::b)>);
static_assert(!asl::is_ptr<const int>);
static_assert(!asl::is_ptr<bool>);
static_assert(asl::is_ptr<int*>);
static_assert(asl::is_ptr<const int*>);
static_assert(asl::is_ptr<int* const>);
static_assert(asl::is_ptr<int* const volatile>);
static_assert(asl::is_ptr<int const* volatile>);
static_assert(!asl::is_ptr<int Struct::*>);
static_assert(!asl::is_ptr<int&>);
static_assert(!asl::is_ptr<int&&>);
static_assert(!asl::is_ptr<int[25]>);
static_assert(!asl::is_ptr<int[]>);
static_assert(!asl::is_ptr<int*[]>);
static_assert(!asl::is_ptr<int(&)[]>);
static_assert(asl::is_ptr<int(*)[]>);
static_assert(!asl::is_ptr<int[]>);
static_assert(!asl::is_ptr<Struct[4]>);
static_assert(!asl::is_ptr<Union[]>);
static_assert(!asl::is_ptr<Enum[]>);
static_assert(!asl::is_ptr<int()>);
static_assert(!asl::is_ptr<Struct>);
static_assert(!asl::is_ptr<const Struct>);
static_assert(!asl::is_ptr<Struct&>);
static_assert(asl::is_ptr<Struct*>);
static_assert(!asl::is_ptr<Union>);
static_assert(!asl::is_ptr<Enum>);
static_assert(!asl::is_ptr<EnumClass>);

static_assert(!asl::is_ref<float>);
static_assert(!asl::is_ref<void>);
static_assert(!asl::is_ref<asl::nullptr_t>);
static_assert(!asl::is_ref<int>);
static_assert(!asl::is_ref<decltype(Struct::b)>);
static_assert(!asl::is_ref<const int>);
static_assert(!asl::is_ref<bool>);
static_assert(!asl::is_ref<int*>);
static_assert(!asl::is_ref<const int*>);
static_assert(!asl::is_ref<int* const>);
static_assert(!asl::is_ref<int* const volatile>);
static_assert(!asl::is_ref<int const* volatile>);
static_assert(!asl::is_ref<int Struct::*>);
static_assert(asl::is_ref<int&>);
static_assert(asl::is_ref<const int&>);
static_assert(asl::is_ref<int&&>);
static_assert(!asl::is_ref<int[25]>);
static_assert(!asl::is_ref<int[]>);
static_assert(!asl::is_ref<int*[]>);
static_assert(asl::is_ref<int(&)[]>);
static_assert(!asl::is_ref<int(*)[]>);
static_assert(!asl::is_ref<int[]>);
static_assert(!asl::is_ref<Struct[4]>);
static_assert(!asl::is_ref<Union[]>);
static_assert(!asl::is_ref<Enum[]>);
static_assert(!asl::is_ref<int()>);
static_assert(!asl::is_ref<Struct>);
static_assert(!asl::is_ref<const Struct>);
static_assert(asl::is_ref<Struct&>);
static_assert(!asl::is_ref<Struct*>);
static_assert(!asl::is_ref<Union>);
static_assert(!asl::is_ref<Enum>);
static_assert(!asl::is_ref<EnumClass>);

static_assert(!asl::is_lref<float>);
static_assert(!asl::is_lref<void>);
static_assert(!asl::is_lref<asl::nullptr_t>);
static_assert(!asl::is_lref<int>);
static_assert(!asl::is_lref<decltype(Struct::b)>);
static_assert(!asl::is_lref<const int>);
static_assert(!asl::is_lref<bool>);
static_assert(!asl::is_lref<int*>);
static_assert(!asl::is_lref<const int*>);
static_assert(!asl::is_lref<int* const>);
static_assert(!asl::is_lref<int* const volatile>);
static_assert(!asl::is_lref<int const* volatile>);
static_assert(!asl::is_lref<int Struct::*>);
static_assert(asl::is_lref<int&>);
static_assert(asl::is_lref<const int&>);
static_assert(!asl::is_lref<int&&>);
static_assert(!asl::is_lref<int[25]>);
static_assert(!asl::is_lref<int[]>);
static_assert(!asl::is_lref<int*[]>);
static_assert(asl::is_lref<int(&)[]>);
static_assert(!asl::is_lref<int(*)[]>);
static_assert(!asl::is_lref<int[]>);
static_assert(!asl::is_lref<Struct[4]>);
static_assert(!asl::is_lref<Union[]>);
static_assert(!asl::is_lref<Enum[]>);
static_assert(!asl::is_lref<int()>);
static_assert(!asl::is_lref<Struct>);
static_assert(!asl::is_lref<const Struct>);
static_assert(asl::is_lref<Struct&>);
static_assert(!asl::is_lref<Struct*>);
static_assert(!asl::is_lref<Union>);
static_assert(!asl::is_lref<Enum>);
static_assert(!asl::is_lref<EnumClass>);

static_assert(!asl::is_rref<float>);
static_assert(!asl::is_rref<void>);
static_assert(!asl::is_rref<asl::nullptr_t>);
static_assert(!asl::is_rref<int>);
static_assert(!asl::is_rref<decltype(Struct::b)>);
static_assert(!asl::is_rref<const int>);
static_assert(!asl::is_rref<bool>);
static_assert(!asl::is_rref<int*>);
static_assert(!asl::is_rref<const int*>);
static_assert(!asl::is_rref<int* const>);
static_assert(!asl::is_rref<int* const volatile>);
static_assert(!asl::is_rref<int const* volatile>);
static_assert(!asl::is_rref<int Struct::*>);
static_assert(!asl::is_rref<int&>);
static_assert(!asl::is_rref<const int&>);
static_assert(asl::is_rref<int&&>);
static_assert(!asl::is_rref<int[25]>);
static_assert(!asl::is_rref<int[]>);
static_assert(!asl::is_rref<int*[]>);
static_assert(!asl::is_rref<int(&)[]>);
static_assert(!asl::is_rref<int(*)[]>);
static_assert(!asl::is_rref<int[]>);
static_assert(!asl::is_rref<Struct[4]>);
static_assert(!asl::is_rref<Union[]>);
static_assert(!asl::is_rref<Enum[]>);
static_assert(!asl::is_rref<int()>);
static_assert(!asl::is_rref<Struct>);
static_assert(!asl::is_rref<const Struct>);
static_assert(!asl::is_rref<Struct&>);
static_assert(!asl::is_rref<Struct*>);
static_assert(!asl::is_rref<Union>);
static_assert(!asl::is_rref<Enum>);
static_assert(!asl::is_rref<EnumClass>);

static_assert(!asl::is_member_ptr<float>);
static_assert(!asl::is_member_ptr<void>);
static_assert(!asl::is_member_ptr<asl::nullptr_t>);
static_assert(!asl::is_member_ptr<int>);
static_assert(!asl::is_member_ptr<decltype(Struct::b)>);
static_assert(!asl::is_member_ptr<const int>);
static_assert(!asl::is_member_ptr<bool>);
static_assert(!asl::is_member_ptr<int*>);
static_assert(!asl::is_member_ptr<const int*>);
static_assert(!asl::is_member_ptr<int* const>);
static_assert(!asl::is_member_ptr<int* const volatile>);
static_assert(!asl::is_member_ptr<int const* volatile>);
static_assert(asl::is_member_ptr<int Struct::*>);
static_assert(asl::is_member_ptr<int (Struct::*)()>);
static_assert(!asl::is_member_ptr<int&>);
static_assert(!asl::is_member_ptr<int&&>);
static_assert(!asl::is_member_ptr<int[25]>);
static_assert(!asl::is_member_ptr<int[]>);
static_assert(!asl::is_member_ptr<int*[]>);
static_assert(!asl::is_member_ptr<int(&)[]>);
static_assert(!asl::is_member_ptr<int(*)[]>);
static_assert(!asl::is_member_ptr<int[]>);
static_assert(!asl::is_member_ptr<Struct[4]>);
static_assert(!asl::is_member_ptr<Union[]>);
static_assert(!asl::is_member_ptr<Enum[]>);
static_assert(!asl::is_member_ptr<int()>);
static_assert(!asl::is_member_ptr<Struct>);
static_assert(!asl::is_member_ptr<const Struct>);
static_assert(!asl::is_member_ptr<Struct&>);
static_assert(!asl::is_member_ptr<Struct*>);
static_assert(!asl::is_member_ptr<Union>);
static_assert(!asl::is_member_ptr<Enum>);
static_assert(!asl::is_member_ptr<EnumClass>);

static_assert(!asl::is_member_object_ptr<float>);
static_assert(!asl::is_member_object_ptr<void>);
static_assert(!asl::is_member_object_ptr<asl::nullptr_t>);
static_assert(!asl::is_member_object_ptr<int>);
static_assert(!asl::is_member_object_ptr<decltype(Struct::b)>);
static_assert(!asl::is_member_object_ptr<const int>);
static_assert(!asl::is_member_object_ptr<bool>);
static_assert(!asl::is_member_object_ptr<int*>);
static_assert(!asl::is_member_object_ptr<const int*>);
static_assert(!asl::is_member_object_ptr<int* const>);
static_assert(!asl::is_member_object_ptr<int* const volatile>);
static_assert(!asl::is_member_object_ptr<int const* volatile>);
static_assert(asl::is_member_object_ptr<int Struct::*>);
static_assert(!asl::is_member_object_ptr<int (Struct::*)()>);
static_assert(!asl::is_member_object_ptr<int&>);
static_assert(!asl::is_member_object_ptr<int&&>);
static_assert(!asl::is_member_object_ptr<int[25]>);
static_assert(!asl::is_member_object_ptr<int[]>);
static_assert(!asl::is_member_object_ptr<int*[]>);
static_assert(!asl::is_member_object_ptr<int(&)[]>);
static_assert(!asl::is_member_object_ptr<int(*)[]>);
static_assert(!asl::is_member_object_ptr<int[]>);
static_assert(!asl::is_member_object_ptr<Struct[4]>);
static_assert(!asl::is_member_object_ptr<Union[]>);
static_assert(!asl::is_member_object_ptr<Enum[]>);
static_assert(!asl::is_member_object_ptr<int()>);
static_assert(!asl::is_member_object_ptr<Struct>);
static_assert(!asl::is_member_object_ptr<const Struct>);
static_assert(!asl::is_member_object_ptr<Struct&>);
static_assert(!asl::is_member_object_ptr<Struct*>);
static_assert(!asl::is_member_object_ptr<Union>);
static_assert(!asl::is_member_object_ptr<Enum>);
static_assert(!asl::is_member_object_ptr<EnumClass>);

static_assert(!asl::is_member_func_ptr<float>);
static_assert(!asl::is_member_func_ptr<void>);
static_assert(!asl::is_member_func_ptr<asl::nullptr_t>);
static_assert(!asl::is_member_func_ptr<int>);
static_assert(!asl::is_member_func_ptr<decltype(Struct::b)>);
static_assert(!asl::is_member_func_ptr<const int>);
static_assert(!asl::is_member_func_ptr<bool>);
static_assert(!asl::is_member_func_ptr<int*>);
static_assert(!asl::is_member_func_ptr<const int*>);
static_assert(!asl::is_member_func_ptr<int* const>);
static_assert(!asl::is_member_func_ptr<int* const volatile>);
static_assert(!asl::is_member_func_ptr<int const* volatile>);
static_assert(!asl::is_member_func_ptr<int Struct::*>);
static_assert(asl::is_member_func_ptr<int (Struct::*)()>);
static_assert(!asl::is_member_func_ptr<int&>);
static_assert(!asl::is_member_func_ptr<int&&>);
static_assert(!asl::is_member_func_ptr<int[25]>);
static_assert(!asl::is_member_func_ptr<int[]>);
static_assert(!asl::is_member_func_ptr<int*[]>);
static_assert(!asl::is_member_func_ptr<int(&)[]>);
static_assert(!asl::is_member_func_ptr<int(*)[]>);
static_assert(!asl::is_member_func_ptr<int[]>);
static_assert(!asl::is_member_func_ptr<Struct[4]>);
static_assert(!asl::is_member_func_ptr<Union[]>);
static_assert(!asl::is_member_func_ptr<Enum[]>);
static_assert(!asl::is_member_func_ptr<int()>);
static_assert(!asl::is_member_func_ptr<Struct>);
static_assert(!asl::is_member_func_ptr<const Struct>);
static_assert(!asl::is_member_func_ptr<Struct&>);
static_assert(!asl::is_member_func_ptr<Struct*>);
static_assert(!asl::is_member_func_ptr<Union>);
static_assert(!asl::is_member_func_ptr<Enum>);
static_assert(!asl::is_member_func_ptr<EnumClass>);

static_assert(!asl::is_compound<float>);
static_assert(!asl::is_compound<void>);
static_assert(!asl::is_compound<asl::nullptr_t>);
static_assert(!asl::is_compound<int>);
static_assert(!asl::is_compound<decltype(Struct::b)>);
static_assert(!asl::is_compound<const int>);
static_assert(!asl::is_compound<bool>);
static_assert(asl::is_compound<int*>);
static_assert(asl::is_compound<const int*>);
static_assert(asl::is_compound<int* const>);
static_assert(asl::is_compound<int* const volatile>);
static_assert(asl::is_compound<int const* volatile>);
static_assert(asl::is_compound<int Struct::*>);
static_assert(asl::is_compound<int (Struct::*)()>);
static_assert(asl::is_compound<int&>);
static_assert(asl::is_compound<int&&>);
static_assert(asl::is_compound<int[25]>);
static_assert(asl::is_compound<int[]>);
static_assert(asl::is_compound<int*[]>);
static_assert(asl::is_compound<int(&)[]>);
static_assert(asl::is_compound<int(*)[]>);
static_assert(asl::is_compound<int[]>);
static_assert(asl::is_compound<Struct[4]>);
static_assert(asl::is_compound<Union[]>);
static_assert(asl::is_compound<Enum[]>);
static_assert(asl::is_compound<int()>);
static_assert(asl::is_compound<Struct>);
static_assert(asl::is_compound<const Struct>);
static_assert(asl::is_compound<Struct&>);
static_assert(asl::is_compound<Struct*>);
static_assert(asl::is_compound<Union>);
static_assert(asl::is_compound<Enum>);
static_assert(asl::is_compound<EnumClass>);

static_assert(asl::is_scalar<float>);
static_assert(!asl::is_scalar<void>);
static_assert(asl::is_scalar<asl::nullptr_t>);
static_assert(asl::is_scalar<int>);
static_assert(asl::is_scalar<decltype(Struct::b)>);
static_assert(asl::is_scalar<const int>);
static_assert(asl::is_scalar<bool>);
static_assert(asl::is_scalar<int*>);
static_assert(asl::is_scalar<const int*>);
static_assert(asl::is_scalar<int* const>);
static_assert(asl::is_scalar<int* const volatile>);
static_assert(asl::is_scalar<int const* volatile>);
static_assert(asl::is_scalar<int Struct::*>);
static_assert(asl::is_scalar<int (Struct::*)()>);
static_assert(!asl::is_scalar<int&>);
static_assert(!asl::is_scalar<int&&>);
static_assert(!asl::is_scalar<int[25]>);
static_assert(!asl::is_scalar<int[]>);
static_assert(!asl::is_scalar<int*[]>);
static_assert(!asl::is_scalar<int(&)[]>);
static_assert(asl::is_scalar<int(*)[]>);
static_assert(!asl::is_scalar<int[]>);
static_assert(!asl::is_scalar<Struct[4]>);
static_assert(!asl::is_scalar<Union[]>);
static_assert(!asl::is_scalar<Enum[]>);
static_assert(!asl::is_scalar<int()>);
static_assert(!asl::is_scalar<Struct>);
static_assert(!asl::is_scalar<const Struct>);
static_assert(!asl::is_scalar<Struct&>);
static_assert(asl::is_scalar<Struct*>);
static_assert(!asl::is_scalar<Union>);
static_assert(asl::is_scalar<Enum>);
static_assert(asl::is_scalar<EnumClass>);

static_assert(asl::is_object<float>);
static_assert(!asl::is_object<void>);
static_assert(asl::is_object<asl::nullptr_t>);
static_assert(asl::is_object<int>);
static_assert(asl::is_object<decltype(Struct::b)>);
static_assert(asl::is_object<const int>);
static_assert(asl::is_object<bool>);
static_assert(asl::is_object<int*>);
static_assert(asl::is_object<const int*>);
static_assert(asl::is_object<int* const>);
static_assert(asl::is_object<int* const volatile>);
static_assert(asl::is_object<int const* volatile>);
static_assert(asl::is_object<int Struct::*>);
static_assert(asl::is_object<int (Struct::*)()>);
static_assert(!asl::is_object<int&>);
static_assert(!asl::is_object<int&&>);
static_assert(!asl::is_object<int[25]>);
static_assert(!asl::is_object<int[]>);
static_assert(!asl::is_object<int*[]>);
static_assert(!asl::is_object<int(&)[]>);
static_assert(asl::is_object<int(*)[]>);
static_assert(!asl::is_object<int[]>);
static_assert(!asl::is_object<Struct[4]>);
static_assert(!asl::is_object<Union[]>);
static_assert(!asl::is_object<Enum[]>);
static_assert(!asl::is_object<int()>);
static_assert(asl::is_object<Struct>);
static_assert(asl::is_object<const Struct>);
static_assert(!asl::is_object<Struct&>);
static_assert(asl::is_object<Struct*>);
static_assert(asl::is_object<Union>);
static_assert(asl::is_object<Enum>);
static_assert(asl::is_object<EnumClass>);

static_assert(asl::is_empty<void>);
static_assert(!asl::is_empty<int>);
static_assert(asl::is_empty<EmptyStruct>);
static_assert(!asl::is_empty<Union>);
static_assert(asl::is_empty<asl::empty>);

static_assert(asl::is_same<asl::select_t<false, int, float>, float>);
static_assert(asl::is_same<asl::select_t<true, int, float>, int>);

static_assert(asl::is_same<asl::devoid_t<int>, int>);
static_assert(asl::is_same<asl::devoid_t<void>, asl::empty>);
static_assert(asl::is_same<asl::devoid_t<const void>, asl::empty>);

static_assert(asl::is_same<asl::tame_t<int(float)>, int(float)>);
static_assert(asl::is_same<asl::tame_t<int(float) &>, int(float)>);
static_assert(asl::is_same<asl::tame_t<int(float) const &&>, int(float)>);
static_assert(asl::is_same<asl::tame_t<int(float) volatile noexcept>, int(float)>);
static_assert(asl::is_same<asl::tame_t<int(float) const>, int(float)>);
static_assert(asl::is_same<asl::tame_t<int(float) const volatile & noexcept>, int(float)>);

static_assert(asl::is_same<asl::as_raw_ptr_t<int>, int*>);
static_assert(asl::is_same<asl::as_raw_ptr_t<const int>, const int*>);
static_assert(asl::is_same<asl::as_raw_ptr_t<int(float)>, int(*)(float)>);
static_assert(asl::is_same<asl::as_raw_ptr_t<int&>, int*>);

static_assert(asl::default_constructible<int>);
static_assert(asl::default_constructible<TriviallyDefaultConstructible>);
static_assert(asl::default_constructible<DefaultConstructible>);
static_assert(!asl::default_constructible<NonDefaultConstructible>);

static_assert(asl::trivially_default_constructible<int>);
static_assert(asl::trivially_default_constructible<TriviallyDefaultConstructible>);
static_assert(!asl::trivially_default_constructible<DefaultConstructible>);
static_assert(!asl::trivially_default_constructible<NonDefaultConstructible>);

static_assert(asl::copy_constructible<int>);
static_assert(asl::copy_constructible<TriviallyCopyConstructible>);
static_assert(asl::copy_constructible<CopyConstructible>);
static_assert(!asl::copy_constructible<NonCopyConstructible>);

static_assert(asl::trivially_copy_constructible<int>);
static_assert(asl::trivially_copy_constructible<TriviallyCopyConstructible>);
static_assert(!asl::trivially_copy_constructible<CopyConstructible>);
static_assert(!asl::trivially_copy_constructible<NonCopyConstructible>);

static_assert(asl::move_constructible<int>);
static_assert(asl::move_constructible<TriviallyMoveConstructible>);
static_assert(asl::move_constructible<MoveConstructible>);
static_assert(!asl::move_constructible<NonMoveConstructible>);

static_assert(asl::trivially_move_constructible<int>);
static_assert(asl::trivially_move_constructible<TriviallyMoveConstructible>);
static_assert(!asl::trivially_move_constructible<MoveConstructible>);
static_assert(!asl::trivially_move_constructible<NonMoveConstructible>);

static_assert(asl::copy_assignable<int>);
static_assert(asl::copy_assignable<CopyAssignable>);
static_assert(asl::copy_assignable<TriviallyCopyAssignable>);
static_assert(!asl::copy_assignable<NoCopyAssignable>);

static_assert(asl::trivially_copy_assignable<int>);
static_assert(!asl::trivially_copy_assignable<CopyAssignable>);
static_assert(asl::trivially_copy_assignable<TriviallyCopyAssignable>);
static_assert(!asl::trivially_copy_assignable<NoCopyAssignable>);

static_assert(asl::move_assignable<int>);
static_assert(asl::move_assignable<MoveAssignable>);
static_assert(asl::move_assignable<TriviallyMoveAssignable>);
static_assert(!asl::move_assignable<NoMoveAssignable>);

static_assert(asl::trivially_move_assignable<int>);
static_assert(!asl::trivially_move_assignable<MoveAssignable>);
static_assert(asl::trivially_move_assignable<TriviallyMoveAssignable>);
static_assert(!asl::trivially_move_assignable<NoMoveAssignable>);

static_assert(asl::trivially_destructible<int>);
static_assert(asl::trivially_destructible<TriviallyDestructible>);
static_assert(!asl::trivially_destructible<HasDestructor>);

static_assert(asl::is_same<asl::as_lref_t<int>, int&>);
static_assert(asl::is_same<asl::as_lref_t<int&>, int&>);
static_assert(asl::is_same<asl::as_lref_t<int&&>, int&>);
static_assert(asl::is_same<asl::as_lref_t<const void>, const void>);

static_assert(asl::is_same<asl::as_rref_t<int>, int&&>);
static_assert(asl::is_same<asl::as_rref_t<int&>, int&>);
static_assert(asl::is_same<asl::as_rref_t<int&&>, int&&>);
static_assert(asl::is_same<asl::as_rref_t<const void>, const void>);

int main() { return 0; }
