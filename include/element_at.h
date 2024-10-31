#ifndef ELEMENT_AT_H
#define ELEMENT_AT_H

#include <type_traits>

// Define a type list using variadic templates
template <typename... Ts>
struct TypeList {};

// Define a pair of index and type
template <std::size_t Index, typename T>
struct IndexedType {};

// Primary template for element_at
template <std::size_t Index, typename List>
struct element_at;

// Specialization for TypeList
template <std::size_t Index, std::size_t HeadIndex, typename Head, typename... Tail>
struct element_at<Index, TypeList<IndexedType<HeadIndex, Head>, Tail...>> {
    using type = typename element_at<Index, TypeList<Tail...>>::type;
};

// Specialization for the base case (Index matches HeadIndex)
template <std::size_t Index, typename Head, typename... Tail>
struct element_at<Index, TypeList<IndexedType<Index, Head>, Tail...>> {
    using type = Head;
};

// Helper alias template
template <std::size_t Index, typename List>
using element_at_t = typename element_at<Index, List>::type;

#endif // ELEMENT_AT_H
