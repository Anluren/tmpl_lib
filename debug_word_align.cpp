#include <iostream>
#include "seq.h"

int main() {
    std::cout << "Testing reverse total sequence [1, 2, 3]:" << std::endl;
    
    using helper = total_seq_helper<1, 2, 3>;
    
    std::cout << "gen_reverse_total_at<0>(): " << helper::gen_reverse_total_at<0>() << " (expected: 5)" << std::endl;
    std::cout << "gen_reverse_total_at<1>(): " << helper::gen_reverse_total_at<1>() << " (expected: 3)" << std::endl;
    std::cout << "gen_reverse_total_at<2>(): " << helper::gen_reverse_total_at<2>() << " (expected: 0)" << std::endl;
    
    return 0;
}
