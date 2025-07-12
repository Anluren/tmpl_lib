#ifndef SEQ_H
#define SEQ_H

#include <type_traits>
#include <utility>
#include <array>

/**
 * \brief Helper struct for generating a sequence of total values.
 *
 * This struct provides static member functions and type aliases for generating
 * a sequence of total values from a variadic template parameter pack.
 *
 * \tparam Vs The variadic template parameter pack representing the values.
 */
template <int... Vs>
struct total_seq_helper {
    /**
     * The total value of the sequence.
     */
    static constexpr int total_value = (Vs + ...);

    /**
     * \brief Generates the value at the specified index.
     *
     * This function generates the value at the specified index in the sequence.
     *
     * \tparam I The index.
     * \return The value at the specified index.
     */
    template <std::size_t I>
    auto static constexpr get_value_at() noexcept {
        static_assert(I < sizeof...(Vs), "Index out of range");
        std::size_t cur_index = 0;
        int value = 0;
        return (void)((value = Vs, I == cur_index++) || ...), value;
    }

    /**
     * \brief Generates the total value at the specified index.
     *
     * This function generates the total value at the specified index in the
     * sequence.
     *
     * \tparam I The index.
     * \return The total value at the specified index.
     */
    template <std::size_t I>
    auto static constexpr gen_total_at() {
        std::size_t cur_index = 0;
        int total = 0;
        return (void)(((I == cur_index++) || (total += Vs, false)) || ...), total;
    }

    template <std::size_t... Is>
    auto static constexpr gen_total_value_sequence_impl(const std::index_sequence<Is...>) {
        return std::integer_sequence<int, gen_total_at<Is>()...>{};
    }

    /**
     * \brief Helper function for generating the sequence of total values.
     *
     * This function generates the sequence of total values using the
     * `gen_total_at` function.
     *
     * \tparam Is The indices.
     * \param The index sequence.
     * \return The generated sequence of total values.
     */
    auto static constexpr gen_total_value_sequence() {
        return gen_total_value_sequence_impl(std::make_index_sequence<sizeof...(Vs)>{});
    }

    /**
     * \brief Finds the index of the first element in the sequence that is equal
     * to the specified value.
     *
     * This function finds the index of the first element in the sequence that is
     * equal to the specified value. If no such element is found, it returns the
     * size of the sequence.
     *
     * \tparam V The value to search for.
     * \return The index of the first element that is equal to the specified
     * value.
     */
    template <int V>
    std::size_t static constexpr find_first_equal() noexcept {
        std::size_t index = 0;
        return (void)((((Vs == V) || (++index, false))) || ...), index;
    }

    /**
     * \brief Finds the index of the first element in the sequence that is
     * greater than the specified value.
     *
     * This function finds the index of the first element in the sequence that is
     * greater than the specified value. If no such element is found, it returns
     * the size of the sequence.
     *
     * \tparam V The value to compare against.
     * \return The index of the first element that is greater than the specified value.
     */
    template <int V>
    std::size_t static constexpr find_first_greater_than() noexcept {
        std::size_t index = 0;
        return (void)((((Vs > V) || (++index, false))) || ...), index;
    }

    // /**
    //  * \brief Gets the last value in the sequence.
    //  *
    //  * This function returns the last value in the sequence.
    //  *
    //  * \return The last value in the sequence.
    //  */
    // auto static constexpr get_last_value() noexcept {
    //     int last_value = 0;
    //     return (void)((last_value = Vs, ...)), last_value;
    // }

    /**
     * \brief Gets the first value in the sequence.
     *
     * This function returns the first value in the sequence.
     *
     * \return The first value in the sequence.
     */
    auto static constexpr get_first_value() noexcept {
        constexpr std::array<int, sizeof...(Vs)> first_value_array = {Vs...};
        return first_value_array[0];
    }
};

/**
 * \brief gets the value at the specified index.
 *
 * \tparam I   The index
 * \tparam Vs  The values in the sequence
 */
template <std::size_t I, int... Vs>
auto static constexpr get_value_at = total_seq_helper<Vs...>::template get_value_at<I>();

/**
 * \brief Alias template for generating a sequence of total values.
 *
 * This alias template takes a variadic template parameter pack `Vs` and
 * generates a sequence of total values using the `total_seq_helper` class. The
 * generated sequence is returned as the type of the alias.
 *
 * \tparam Vs The variadic template parameter pack representing the values.
 */
template <int... Vs>
using make_total_value_sequence = decltype(total_seq_helper<Vs...>::gen_total_value_sequence());

/**
 * \brief The index of the first element in the sequence that is equal to the
 * specified value.
 *
 * This constant expression represents the index of the first element in the
 * sequence that is equal to the specified value. If no such element is found,
 * it represents the size of the sequence.
 */
template <int V, int... Vs>
std::size_t constexpr get_index_with_value =
    total_seq_helper<Vs...>::template find_first_equal<V>();

template <int V, int... Vs>
std::size_t constexpr get_index_with_greater_value =
    total_seq_helper<Vs...>::template find_first_greater_than<V>();

/**
 * \brief Generates an index sequence in the specified range.
 *
 * This template function generates an index sequence in the specified range
 * [Min, Max).
 *
 * \tparam Min The minimum value of the range.
 * \tparam Max The maximum value of the range.
 * \return The generated index sequence.
 */
template <std::size_t Min, std::size_t Max, std::size_t... Is>
auto constexpr make_index_sequence_range_impl(std::index_sequence<Is...>) {
    return std::index_sequence<(Min + Is)...>{};
}

/**
 * \brief Alias template for generating an index sequence in the specified
 * range.
 *
 * This alias template generates an index sequence in the specified range [Min,
 * Max).
 *
 * \tparam Min The minimum value of the range.
 * \tparam Max The maximum value of the range.
 */
template <std::size_t Min, std::size_t Max>
auto constexpr make_index_sequence_range() {
    return make_index_sequence_range_impl<Min, Max>(std::make_index_sequence<Max - Min>{});
}

/**
 * \brief Alias template for the generated index sequence in the specified
 * range.
 *
 * This alias template represents the type of the index sequence generated by
 * `make_index_sequence_range`.
 *
 * \tparam Min The minimum value of the range.
 * \tparam Max The maximum value of the range.
 */
template <std::size_t Min, std::size_t Max>
using make_index_sequence_range_t = decltype(make_index_sequence_range<Min, Max>());

// implement 1 based index sequence, and last element is 0
template <std::size_t N, std::size_t... Is>
auto constexpr make_one_based_index_sequence_impl(std::index_sequence<Is...>) {
    return std::integer_sequence<std::size_t, ((Is + 1) % N)...>{};
}

/**
 * \brief Alias template for the generated one-based index sequence.
 *
 * This alias template represents the type of the one-based index sequence
 * generated by `make_one_based_index_sequence`.
 *
 * \tparam N The size of the sequence.
 */
template <std::size_t N>
using make_one_based_index_sequence =
    decltype(make_one_based_index_sequence_impl<N>(std::make_index_sequence<N>{}));

/**
 * \brief Splits a sequence into two sequences.
 *
 * This template function splits a sequence into two sequences.
 * The first sequence has N elements starting from 0, and the second sequence
 * has M - N elements.
 *
 * \tparam N The number of elements in the first sequence.
 * \tparam Is The indices of the elements in the first sequence.
 * \tparam Js The indices of the elements in the second sequence.
 * \param seq1 The index sequence of the first sequence.
 * \param seq2 The index sequence of the second sequence.
 * \return A pair of index sequences representing the two sequences.
 */
template <std::size_t N, std::size_t... Is, std::size_t... Js>
auto constexpr split_sequence(std::index_sequence<Is...>, std::index_sequence<Js...>) {
    return std::make_pair(std::index_sequence<Is...>{},
                          std::integer_sequence<std::size_t, (N + Js)...>{});
}

/**
 * \brief Helper function for splitting a sequence into two sequences.
 *
 * This template function splits a sequence into two sequences.
 * The first sequence has N elements starting from 0, and the second sequence
 * has M - N elements.
 *
 * \tparam N The number of elements in the first sequence.
 * \tparam Is The indices of the elements in the first sequence.
 * \param seq The index sequence.
 * \return A pair of index sequences representing the two sequences.
 */
template <std::size_t N, std::size_t... Is>
auto constexpr split_sequence_impl(std::index_sequence<Is...>) {
    return split_sequence<N>(std::make_index_sequence<N>{},
                             std::make_index_sequence<sizeof...(Is) - N>{});
}

/**
 * \brief Splits a sequence into two sequences.
 *
 * This template function splits a sequence into two sequences.
 * The first sequence has N elements starting from 0, and the second sequence
 * has M - N elements.
 *
 * \tparam N The number of elements in the first sequence.
 * \tparam M The total number of elements in the sequence.
 * \return A pair of index sequences representing the two sequences.
 */
template <std::size_t N, std::size_t M>
auto constexpr split_sequence() {
    return split_sequence_impl<N>(std::make_index_sequence<M>{});
}

/**
 * \brief Helper struct for splitting a sequence into word size (in bits) aligned indexes
 *
 * \tparam W The word size.
 * \tparam Vs The values in the sequence.
 */
template <int W, int... Vs>
struct split_total_seq_helper {
    /**
     * \brief The total value of the sequence.
     */
    static constexpr int total_value = (Vs + ...);

    /**
     * \brief The number of words in the sequence.
     */
    static constexpr std::size_t word_count = total_value / W;

    static_assert(total_value % W == 0, "total value must be divisible by W");

    /**
     * \brief The total sequence of values.
     */
    using total_seq = make_total_value_sequence<Vs..., 0>;

    /**
     * \brief The sequence of word indices.
     */
    using word_index_seq = std::make_index_sequence<word_count + 1>;

    /**
     * \brief Helper function to generate the word total value sequence.
     *
     * \tparam Is The indices of the word total value sequence.
     * \tparam TotalValues The total values of the sequence.
     * \param seq The word index sequence.
     * \param values The total value sequence.
     * \return The word total value sequence.
     */
    template <std::size_t... Is, int... TotalValues>
    auto static constexpr gen_word_total_value_sequence_impl(
        std::index_sequence<Is...>, std::integer_sequence<int, TotalValues...>) noexcept {
        return std::integer_sequence<std::size_t,
                                     get_index_with_value<W * (1 + Is), TotalValues...>...>{};
    }

    /**
     * \brief Generate the word total index sequence.
     *
     * \return The word total index sequence.
     */
    auto static constexpr gen_word_total_index_sequence() noexcept {
        return gen_word_total_value_sequence_impl(word_index_seq{}, total_seq{});
    }

    /**
     * \brief Alias for the word index sequence.
     */
    using make_word_index_sequence =
        decltype(split_total_seq_helper<W, Vs...>::gen_word_total_index_sequence());

    /**
     * \brief Apply a predicate to the word index sequence.
     *
     * \tparam Predicate The predicate function.
     * \tparam T The type of the values in the sequence.
     * \tparam Is The values in the sequence.
     * \param pred The predicate function.
     * \param seq The word index sequence.
     * \return True if the predicate is true for all values in the sequence, false otherwise.
     */
    template <typename Predicate, typename T, T... Is>
    static constexpr bool apply_predicate(Predicate pred,
                                          std::integer_sequence<T, Is...>) noexcept {
        constexpr std::array<T, sizeof...(Is)> arr = {Is...};
        for (std::size_t i = 0; i < (sizeof...(Is) - 1); ++i) {
            if (!pred(arr[i])) {
                return false;
            }
        }
        return true;
    }

    /**
     * \brief Check if the word size is aligned.
     */
    static constexpr bool is_word_size_aligned =
        apply_predicate([](int i) { return i <= (sizeof...(Vs)); }, make_word_index_sequence{});
};

/**
 * \brief Generates index sequence based on values and word size.
 *
 * This template alias uses the `split_total_seq_helper` class to generate a word index sequence
 * based on the provided width (`W`) and values (`Vs...`).
 *
 * \tparam W The width of the word
 * \tparam Vs The values of the sequence
 * \return The generated word index sequence.
 */
template <int W, int... Vs>
using make_word_index_sequence =
    typename split_total_seq_helper<W, Vs...>::make_word_index_sequence;

/**
 * \brief Check if the values are word size aligned.
 *
 * \tparam W    The width of the word
 * \tparam Vs   The values of the sequence
 */
template <int W, int... Vs>
constexpr bool is_word_size_aligned = split_total_seq_helper<W, Vs...>::is_word_size_aligned;

/**
 * \brief Creates a reversed index sequence.
 *
 * This template function creates an index sequence with indices in reverse order.
 * For example, make_reverse_index_sequence<5>() creates std::index_sequence<4, 3, 2, 1, 0>.
 *
 * \tparam N The size of the sequence.
 * \tparam Is The indices (automatically deduced).
 * \return A reversed index sequence.
 */
template<std::size_t N, std::size_t... Is>
constexpr auto make_reverse_index_sequence_impl(std::index_sequence<Is...>) {
    return std::index_sequence<(N - 1 - Is)...>{};
}

/**
 * \brief Creates a reversed index sequence of size N.
 *
 * \tparam N The size of the sequence.
 * \return A reversed index sequence from N-1 down to 0.
 */
template<std::size_t N>
constexpr auto make_reverse_index_sequence() {
    return make_reverse_index_sequence_impl<N>(std::make_index_sequence<N>{});
}

/**
 * \brief Type alias for reversed index sequence.
 *
 * \tparam N The size of the sequence.
 */
template<std::size_t N>
using make_reverse_index_sequence_t = 
    decltype(make_reverse_index_sequence_impl<N>(std::make_index_sequence<N>{}));

/**
 * \brief Reverses an existing index sequence.
 *
 * \tparam Is The indices in the original sequence.
 * \return A reversed version of the input sequence.
 */
template<std::size_t... Is>
constexpr auto reverse_index_sequence(std::index_sequence<Is...>) {
    constexpr std::size_t N = sizeof...(Is);
    return std::index_sequence<(N - 1 - Is)...>{};
}

#endif // SEQ_H