#pragma once

#include <type_traits>
#include <cstddef>

namespace tmpl_lib {

// Helper to generate a type list
template <typename...>
struct type_list {};

// Example: Define your types here
struct TypeA {};
struct TypeB {};
struct TypeC {};

// Define the type list
using MyTypeList = type_list<TypeA, TypeB, TypeC>;

// Define an enum to hold the type ids
enum class TypeId : std::size_t {
    TypeA,
    TypeB,
    TypeC,
    Count // Optional: for the number of types
};

// Mapping from type to enum value
template <typename T>
struct type_to_id;

template <>
struct type_to_id<TypeA> : std::integral_constant<TypeId, TypeId::TypeA> {};

template <>
struct type_to_id<TypeB> : std::integral_constant<TypeId, TypeId::TypeB> {};

template <>
struct type_to_id<TypeC> : std::integral_constant<TypeId, TypeId::TypeC> {};

} // namespace tmpl_lib