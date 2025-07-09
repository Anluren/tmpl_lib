#include <array>
#include <utility>

template<std::size_t... Is>
constexpr auto make_array_impl(std::index_sequence<Is...>) {
    return std::array<NoDefault, sizeof...(Is)>{{NoDefault(Is + 1)...}};
}

template<std::size_t N>
constexpr auto make_array() {
    return make_array_impl(std::make_index_sequence<N>{});
}

constexpr auto arr = make_array<5>();  // Creates array with values 1,2,3,4,5