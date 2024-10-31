#ifndef ELEMENT_AT_H
#define ELEMENT_AT_H

#include <type_traits>

// Define a type list using variadic templates
template <typename... Ts>
struct TypeList {};

// Primary template for element_at
template <std::size_t Index, typename List>
struct element_at;

// Specialization for TypeList
template <std::size_t Index, typename Head, typename... Tail>
struct element_at<Index, TypeList<Head, Tail...>> {
    using type = typename element_at<Index - 1, TypeList<Tail...>>::type;
};

// Specialization for the base case (Index == 0)
template <typename Head, typename... Tail>
struct element_at<0, TypeList<Head, Tail...>> {
    using type = Head;
};

// Helper alias template
template <std::size_t Index, typename List>
using element_at_t = typename element_at<Index, List>::type;

#endif // ELEMENT_AT_H
