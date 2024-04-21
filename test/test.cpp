
#include <gtest/gtest.h>
#include <utility>
#include "seq.h"

TEST(SeqTest, SumSequence) {
    auto seq = std::integer_sequence<int, 1, 2, 3, 4, 5>();
    auto sum_seq = gen_total_value_sequence(seq);

    EXPECT_EQ(sequence_to_string(sum_seq), "0 1 3 6 10 ");
}

TEST(SeqTest, MakeIndexSequenceRange) {
    auto seq = make_index_sequence_range<1, 5>();
    EXPECT_EQ(sequence_to_string(seq), "1 2 3 4 ");
    }

TEST(SeqTest, MakeOneBasedIndexSequence) {
        auto seq = make_one_based_index_sequence<5>();
        EXPECT_EQ(sequence_to_string(seq), "1 2 3 4 0 ");
}


TEST(SeqTest, SplitSequence) {
    // auto seq = std::integer_sequence<int, 1, 2, 3, 4, 5>();
    auto split_seq = split_sequence<2, 5>();

    EXPECT_EQ(sequence_to_string(std::get<0>(split_seq)), "0 1 ");
    EXPECT_EQ(sequence_to_string(std::get<1>(split_seq)), "2 3 4 ");
}
TEST(SeqTest, FindFirstGreaterThan) {
    auto seq = std::integer_sequence<int, 1, 2, 3, 4, 5>();
    auto result = find_first_greater_than<3, 0>(seq);
    EXPECT_EQ(result, 3);
    result = find_first_greater_than<4, 0>(seq);
    EXPECT_EQ(result, 4);
    result = find_first_greater_than<7, 0>(seq);
    EXPECT_EQ(result, 5);
}