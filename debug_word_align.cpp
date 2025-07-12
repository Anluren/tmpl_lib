#include <iostream>
#include "seq.h"

int main() {
    std::cout << "Testing <8, 8, 16>:" << std::endl;
    std::cout << "Total value: " << split_total_seq_helper<8, 8, 8, 16>::total_value << std::endl;
    std::cout << "Word count: " << split_total_seq_helper<8, 8, 8, 16>::word_count << std::endl;
    std::cout << "sizeof...(Vs): " << 3 << std::endl;
    std::cout << "is_word_size_aligned: " << (is_word_size_aligned<8, 8, 8, 16> ? "true" : "false") << std::endl;
    return 0;
}
