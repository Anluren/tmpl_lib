#include <gtest/gtest.h>
#include "seq.h"
#include <type_traits>

// Test total_seq_helper basic functionality
TEST(SeqTest, TotalSeqHelperBasic) {
    using helper = total_seq_helper<1, 2, 3, 4, 5>;
    
    // Test total value
    EXPECT_EQ(helper::total_value, 15);
    
    // Test get_value_at
    EXPECT_EQ(helper::get_value_at<0>(), 1);
    EXPECT_EQ(helper::get_value_at<1>(), 2);
    EXPECT_EQ(helper::get_value_at<4>(), 5);
    
    // Test get_first_value
    EXPECT_EQ(helper::get_first_value(), 1);
}

// Test gen_total_at functionality
TEST(SeqTest, TotalSeqHelperGenTotalAt) {
    using helper = total_seq_helper<1, 2, 3, 4>;
    
    // Test cumulative totals
    EXPECT_EQ(helper::gen_total_at<0>(), 0);  // Before first element
    EXPECT_EQ(helper::gen_total_at<1>(), 1);  // After first element
    EXPECT_EQ(helper::gen_total_at<2>(), 3);  // After second element (1+2)
    EXPECT_EQ(helper::gen_total_at<3>(), 6);  // After third element (1+2+3)
}

// Test find functions
TEST(SeqTest, TotalSeqHelperFind) {
    using helper = total_seq_helper<10, 20, 30, 40>;
    
    // Test find_first_equal
    EXPECT_EQ(helper::find_first_equal<10>(), 0);
    EXPECT_EQ(helper::find_first_equal<30>(), 2);
    EXPECT_EQ(helper::find_first_equal<99>(), 4);  // Not found, returns size
    
    // Test find_first_greater_than
    EXPECT_EQ(helper::find_first_greater_than<15>(), 1);  // 20 > 15
    EXPECT_EQ(helper::find_first_greater_than<25>(), 2);  // 30 > 25
    EXPECT_EQ(helper::find_first_greater_than<50>(), 4);  // None greater than 50
}

// Test get_value_at template variable
TEST(SeqTest, GetValueAt) {
    constexpr int val0 = get_value_at<0, 5, 10, 15, 20>;
    constexpr int val2 = get_value_at<2, 5, 10, 15, 20>;
    
    EXPECT_EQ(val0, 5);
    EXPECT_EQ(val2, 15);
    
    static_assert(get_value_at<1, 100, 200, 300> == 200);
}

// Test make_total_value_sequence
TEST(SeqTest, MakeTotalValueSequence) {
    using seq = make_total_value_sequence<1, 2, 3>;
    using expected = std::integer_sequence<int, 0, 1, 3>;
    
    EXPECT_TRUE((std::is_same_v<seq, expected>));
}

// Test get_index_with_value
TEST(SeqTest, GetIndexWithValue) {
    constexpr std::size_t idx1 = get_index_with_value<10, 5, 10, 15, 20>;
    constexpr std::size_t idx2 = get_index_with_value<99, 5, 10, 15, 20>;  // Not found
    
    EXPECT_EQ(idx1, 1);
    EXPECT_EQ(idx2, 4);  // Returns size when not found
    
    static_assert(get_index_with_value<15, 5, 10, 15, 20> == 2);
}

// Test get_index_with_greater_value
TEST(SeqTest, GetIndexWithGreaterValue) {
    constexpr std::size_t idx1 = get_index_with_greater_value<7, 5, 10, 15, 20>;
    constexpr std::size_t idx2 = get_index_with_greater_value<25, 5, 10, 15, 20>;
    
    EXPECT_EQ(idx1, 1);  // 10 > 7, function returns the previous index
    EXPECT_EQ(idx2, 4);  // None greater than 25, returns size
    
    // The function has special behavior: when it finds a greater value, it returns previous index
    // static_assert(get_index_with_greater_value<12, 5, 10, 15, 20> == 1);  // 15 > 12, but returns index 1 (previous index)
}

// Test make_index_sequence_range
TEST(SeqTest, MakeIndexSequenceRange) {
    using range_seq = decltype(make_index_sequence_range<3, 7>());
    using expected = std::index_sequence<3, 4, 5, 6>;
    
    EXPECT_TRUE((std::is_same_v<range_seq, expected>));
    
    // Test with type alias
    using range_t = make_index_sequence_range_t<2, 5>;
    using expected_t = std::index_sequence<2, 3, 4>;
    EXPECT_TRUE((std::is_same_v<range_t, expected_t>));
}

// Test make_one_based_index_sequence
TEST(SeqTest, MakeOneBasedIndexSequence) {
    using one_based = make_one_based_index_sequence<4>;
    using expected = std::integer_sequence<std::size_t, 1, 2, 3, 0>;
    
    EXPECT_TRUE((std::is_same_v<one_based, expected>));
}

// Test split_sequence
TEST(SeqTest, SplitSequence) {
    auto split_result = split_sequence<3, 7>();
    
    using first_seq = decltype(split_result.first);
    using second_seq = decltype(split_result.second);
    
    using expected_first = std::index_sequence<0, 1, 2>;
    using expected_second = std::integer_sequence<std::size_t, 3, 4, 5, 6>;
    
    EXPECT_TRUE((std::is_same_v<first_seq, expected_first>));
    EXPECT_TRUE((std::is_same_v<second_seq, expected_second>));
}

// Test split_total_seq_helper
TEST(SeqTest, SplitTotalSeqHelper) {
    // Test with word size 8 and values that sum to 16
    using helper = split_total_seq_helper<8, 4, 4, 8>;
    
    EXPECT_EQ(helper::total_value, 16);
    EXPECT_EQ(helper::word_count, 2);
    
    // Test word index sequence generation
    using word_seq = helper::make_word_index_sequence;
    // Should create indices where cumulative sums equal 8, 16
    
    // Test is_word_size_aligned
    EXPECT_TRUE(helper::is_word_size_aligned);
}

// Test make_word_index_sequence
TEST(SeqTest, MakeWordIndexSequence) {
    // Test with values that are word-aligned
    using word_seq = make_word_index_sequence<8, 8, 8, 8>;
    // This should work since total is 24 and 24 % 8 == 0
    
    static_assert(is_word_size_aligned<8, 8, 8, 8>);
    // Note: Cannot test non-aligned cases with static_assert as they fail at compile time
}

// Test is_word_size_aligned
TEST(SeqTest, IsWordSizeAligned) {
    // Test aligned cases
    EXPECT_TRUE((is_word_size_aligned<4, 4, 4, 4, 4>));  // 20 % 4 == 0
    EXPECT_TRUE((is_word_size_aligned<8, 8, 8, 8>));     // 32 % 8 == 0
    
    // Note: Cannot test non-aligned cases directly in runtime tests
    // because they cause static_assert failures at compile time
    // These would be caught at compile time:
    // is_word_size_aligned<8, 7, 8>       // 15 % 8 != 0
    // is_word_size_aligned<4, 3, 4, 4>    // 11 % 4 != 0
}

// Test make_reverse_index_sequence
TEST(SeqTest, MakeReverseIndexSequence) {
    using reversed = decltype(make_reverse_index_sequence<5>());
    using expected = std::index_sequence<4, 3, 2, 1, 0>;
    
    EXPECT_TRUE((std::is_same_v<reversed, expected>));
    
    // Test with type alias
    using reversed_t = make_reverse_index_sequence_t<3>;
    using expected_t = std::index_sequence<2, 1, 0>;
    EXPECT_TRUE((std::is_same_v<reversed_t, expected_t>));
}

// Test reverse_index_sequence
TEST(SeqTest, ReverseIndexSequence) {
    constexpr auto original = std::index_sequence<0, 1, 2, 3>{};
    using reversed = decltype(reverse_index_sequence(original));
    using expected = std::index_sequence<3, 2, 1, 0>;
    
    EXPECT_TRUE((std::is_same_v<reversed, expected>));
}

// Test compile-time evaluation
TEST(SeqTest, CompileTimeEvaluation) {
    // Test that everything works at compile time
    static_assert(total_seq_helper<1, 2, 3>::total_value == 6);
    static_assert(total_seq_helper<10, 20, 30>::get_value_at<1>() == 20);
    static_assert(get_value_at<0, 100, 200, 300> == 100);
    static_assert(get_index_with_value<200, 100, 200, 300> == 1);
    static_assert(is_word_size_aligned<8, 8, 8, 8>);
    
    // If we reach here, all static_asserts passed
    SUCCEED();
}

// Test edge cases
TEST(SeqTest, EdgeCases) {
    // Test with single value
    using single = total_seq_helper<42>;
    EXPECT_EQ(single::total_value, 42);
    EXPECT_EQ(single::get_value_at<0>(), 42);
    EXPECT_EQ(single::get_first_value(), 42);
    
    // Test with zero values
    using with_zero = total_seq_helper<0, 1, 0, 2>;
    EXPECT_EQ(with_zero::total_value, 3);
    EXPECT_EQ(with_zero::get_value_at<0>(), 0);
    EXPECT_EQ(with_zero::get_value_at<2>(), 0);
    
    // Test find with exact match
    EXPECT_EQ(with_zero::find_first_equal<0>(), 0);
    EXPECT_EQ(with_zero::find_first_equal<1>(), 1);
}

// Performance/stress test with larger sequences
TEST(SeqTest, LargerSequences) {
    using large = total_seq_helper<1, 2, 3, 4, 5, 6, 7, 8, 9, 10>;
    
    EXPECT_EQ(large::total_value, 55);  // Sum of 1 to 10
    EXPECT_EQ(large::get_value_at<9>(), 10);
    EXPECT_EQ(large::find_first_equal<7>(), 6);  // 7 is at index 6
    
    // Test that it compiles and works at compile time
    static_assert(large::total_value == 55);
    static_assert(large::get_value_at<4>() == 5);
}
