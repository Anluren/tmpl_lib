TEST(SeqTest, SplitSequenceBySum) {
    auto seq = std::integer_sequence<int, 1, 2, 3, 4, 5>();
    auto split_seq = split_sequence_by_sum(seq, 6);

    EXPECT_EQ(sequence_to_string(std::get<0>(split_seq)), "1 2 3 ");
    EXPECT_EQ(sequence_to_string(std::get<1>(split_seq)), "4 5 ");
}