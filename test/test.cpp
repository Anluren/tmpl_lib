#include <string>
#include <iostream>
#include <gtest/gtest.h>

#include "seq.h"

/**
 * \brief Converts an integer sequence to a string.
 *
 * This function takes an integer sequence and converts it to a string
 * representation. Each element in the sequence is separated by a space.
 *
 * \tparam T The type of the elements in the sequence.
 * \tparam Is The elements in the sequence.
 * \param seq The integer sequence to convert.
 * \return The string representation of the integer sequence.
 */
template <typename T, T... Is>
std::string sequence_to_string(std::integer_sequence<T, Is...>) {
    std::string result;
    ((result += std::to_string(Is) + ' '), ...);
    return result;
}

TEST(SeqTest, Basictest) {
    auto seq = std::integer_sequence<int, 1, 2, 3, 4, 5>();
    static_assert(get_value_at<0, 1, 2, 3, 4, 5> == 1);
    static_assert(get_value_at<1, 1, 2, 3, 4, 5> == 2);
    static_assert(get_value_at<2, 1, 2, 3, 4, 5> == 3);
    static_assert(get_value_at<3, 1, 2, 3, 4, 5> == 4);
    static_assert(get_value_at<4, 1, 2, 3, 4, 5> == 5);
}

TEST(SeqTest, SumSequence) {
    // auto seq = std::integer_sequence<int, 1, 2, 3, 4, 5>();
    auto sum_seq = make_total_value_sequence<1, 2, 3, 4, 5>{};

    static_assert((std::is_same_v<decltype(sum_seq), std::integer_sequence<int, 0, 1, 3, 6, 10>>));
    EXPECT_EQ(sequence_to_string(sum_seq), "0 1 3 6 10 ");

    static_assert((get_index_with_value<3, 0, 1, 3, 6, 10> == 2));
    static_assert((get_index_with_greater_value<3, 0, 1, 3, 6, 10> == 2));
    static_assert((get_index_with_greater_value<7, 0, 1, 3, 6, 10> == 3));
}

TEST(SeqTest, MakeIndexSequenceRange) {
    auto seq = make_index_sequence_range<1, 5>();
    EXPECT_EQ(sequence_to_string(seq), "1 2 3 4 ");
}

TEST(SeqTest, MakeOneBasedIndexSequence) {
    auto seq = make_one_based_index_sequence<5>{};
    EXPECT_EQ(sequence_to_string(seq), "1 2 3 4 0 ");
}

TEST(SeqTest, SplitSequence) {
    auto split_seq = split_sequence<2, 5>();
    EXPECT_EQ(sequence_to_string(std::get<0>(split_seq)), "0 1 ");
    EXPECT_EQ(sequence_to_string(std::get<1>(split_seq)), "2 3 4 ");
}

TEST(split_total_seq_helper, SplitTotalSequence) {
    auto seq = make_total_value_sequence<1, 31, 3, 29, 5, 27>{};
    auto word_index = make_word_index_sequence<32, 1, 31, 3, 29, 5, 27>{};
    EXPECT_EQ(sequence_to_string(word_index), "2 4 6 7 ");

    EXPECT_EQ(sequence_to_string(make_word_index_sequence<32, 1, 1, 30, 3, 29, 5, 27>{}),
              "3 5 7 8 ");

    static_assert(is_word_size_aligned<32, 1, 31, 3, 29, 5, 27>);
    static_assert(!is_word_size_aligned<32, 1, 31, 3, 28, 5, 28>);
    auto word_index_1 = make_word_index_sequence<32, 1, 31, 3, 28, 5, 28>{};
    EXPECT_EQ(sequence_to_string(word_index_1), "2 7 6 7 ");
    auto word_values = make_total_value_sequence<32, 1, 31, 3, 28, 5, 28>{};
    EXPECT_EQ(sequence_to_string(word_values), "0 32 33 64 67 95 100 ");
}