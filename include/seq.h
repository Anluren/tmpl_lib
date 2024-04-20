#include <cstddef>
#include <type_traits>
#include <utility>
#include <string>

template<int... Is>
std::string sequence_to_string(std::integer_sequence<int, Is...>) {
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


#include <utility> // Add missing include directive

template<int ... Vs>
auto gen_total_value_sequence(const std::integer_sequence<int, Vs...>& seq) {
    return gen_total_value_sequence_impl(std::make_index_sequence<sizeof...(Vs)>{}, seq);
}
