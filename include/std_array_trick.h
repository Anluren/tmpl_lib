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

template<std::size_t N>
constexpr std::array<uint8_t, N> init_array_from_pointer(const uint8_t* ptr) {
    std::array<uint8_t, N> result;
    std::copy(ptr, ptr + N, result.begin());
    return result;
}

constexpr const uint8_t* get_buffer_pointer(const std::array<uint8_t, N>& arr) {
    return arr.data();
}
