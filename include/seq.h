#include <__utility/integer_sequence.h>
#include <cstddef>
#include <type_traits>
#include <utility>
#include <string>

template<typename T, T... Is>
std::string sequence_to_string(std::integer_sequence<T, Is...>) {
    std::string result;
    ((result += std::to_string(Is) + ' '), ...);
    return result;
}

template<std::size_t I, int ... Vs>
auto constexpr    gen_total_valuee(const std::integer_sequence<int, Vs...>&) {
    std::size_t cur_index = 0;
    int total = 0;
    return (void)(((I == cur_index++) || (total += Vs, false)) || ...), total;
}

template<std::size_t ...Is, int ... Vs>
auto gen_total_value_sequence_impl(const std::index_sequence<Is...>, const std::integer_sequence<int, Vs...>&) {
    return std::integer_sequence
        <int, gen_total_valuee<Is>(std::integer_sequence<int, Vs...>{})...>{};
}

template<int ... Vs>
auto gen_total_value_sequence(const std::integer_sequence<int, Vs...>& seq) {
    return gen_total_value_sequence_impl(std::make_index_sequence<sizeof...(Vs)>{}, seq);
}

template<std::size_t Min, std::size_t Max, std::size_t... Is>
auto make_index_sequence_range_impl(std::index_sequence<Is...>) {
    return std::index_sequence<(Min + Is)...>{};
}

template<std::size_t Min, std::size_t Max>
auto make_index_sequence_range() {
    return make_index_sequence_range_impl<Min, Max>(std::make_index_sequence<Max - Min>{});
}

// make 1 based index sequence, and last element is 0
template <std::size_t N, std::size_t ...Is>
auto make_one_based_index_sequence_impl(std::index_sequence<Is...>) {
    return std::integer_sequence<std::size_t, ((Is + 1)%N)...>{};
}
template<std::size_t N>
auto make_one_based_index_sequence() {
    return make_one_based_index_sequence_impl<N>(std::make_index_sequence<N>{});
}