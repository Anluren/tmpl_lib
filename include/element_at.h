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
template <std::size_t Index, typename... Ts>
struct element_at<Index, TypeList<Ts...>> {
    template <std::size_t I, typename T>
    static constexpr T helper(IndexedType<I, T>);

    using type = decltype(helper<Index>(std::declval<Ts>()...));
};

// Helper alias template
template <std::size_t Index, typename List>
using element_at_t = typename element_at<Index, List>::type;

#endif // ELEMENT_AT_H
