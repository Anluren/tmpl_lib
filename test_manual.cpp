#include "type_value.h"
#include <iostream>

int main() {
    // Test the new get_value_at functionality
    using Wrapper = OutputIndicesWrapper<5, 10, 15, 20>;
    
    std::cout << "Testing get_value_at functionality:" << std::endl;
    std::cout << "Value at index 0: " << Wrapper::get_value_at<0>() << std::endl;
    std::cout << "Value at index 1: " << Wrapper::get_value_at<1>() << std::endl;
    std::cout << "Value at index 2: " << Wrapper::get_value_at<2>() << std::endl;
    std::cout << "Value at index 3: " << Wrapper::get_value_at<3>() << std::endl;
    std::cout << "Size: " << Wrapper::size() << std::endl;
    
    // Test with TypeValueContainer
    using Container = Wrapper::TypeValueContainer<int>;
    std::cout << "\nTesting container methods:" << std::endl;
    std::cout << "Output index at 0: " << Container::get_output_index_at<0>() << std::endl;
    std::cout << "Output index at 1: " << Container::get_output_index_at<1>() << std::endl;
    std::cout << "Output index at 2: " << Container::get_output_index_at<2>() << std::endl;
    std::cout << "Output index at 3: " << Container::get_output_index_at<3>() << std::endl;
    std::cout << "Output size: " << Container::output_size() << std::endl;
    
    return 0;
}
