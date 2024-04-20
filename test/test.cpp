
#include <gtest/gtest.h>
#include <utility>
#include "seq.h"

TEST(SeqTest, SumSequence) {
    auto seq = std::integer_sequence<int, 1, 2, 3, 4, 5>();
    auto sum_seq = gen_total_value_sequence(seq);

    EXPECT_EQ(sequence_to_string(sum_seq), "0 1 3 6 10 ");
}