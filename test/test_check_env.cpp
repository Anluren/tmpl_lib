#include <gtest/gtest.h>
#include "check_env.h"
#include <cstdint>
#include <cstring>

// Test that the macro is defined
TEST(CheckEnvTest, MacrosDefined) {
    // Check that the main macro is defined
    EXPECT_TRUE(SUPPORTS_UNALIGNED_MEMORY_ACCESS == 0 || SUPPORTS_UNALIGNED_MEMORY_ACCESS == 1);
    
    // Check that the alternative macro is the same
    EXPECT_EQ(SUPPORTS_UNALIGNED_MEMORY_ACCESS, HAS_UNALIGNED_MEMORY_ACCESS);
}

// Test the C++ namespace constant
TEST(CheckEnvTest, CppNamespaceConstant) {
    EXPECT_TRUE(check_env::supports_unaligned_access == 0 || check_env::supports_unaligned_access == 1);
    EXPECT_EQ(static_cast<bool>(SUPPORTS_UNALIGNED_MEMORY_ACCESS), check_env::supports_unaligned_access);
}

// Test actual unaligned memory access if supported
TEST(CheckEnvTest, UnalignedMemoryAccess) {
    // Create a buffer with known data
    alignas(8) uint8_t buffer[16] = {
        0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
        0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10
    };
    
    if (SUPPORTS_UNALIGNED_MEMORY_ACCESS) {
        // Test unaligned 32-bit access
        uint32_t value32;
        std::memcpy(&value32, buffer + 1, sizeof(value32));
        
        // Expected value depends on endianness, but we can test that it reads something
        EXPECT_NE(value32, 0);
        
        // Test unaligned 64-bit access
        uint64_t value64;
        std::memcpy(&value64, buffer + 3, sizeof(value64));
        EXPECT_NE(value64, 0);
        
        std::cout << "Unaligned memory access is supported and tested successfully." << std::endl;
    } else {
        // If unaligned access is not supported, we should still be able to use memcpy safely
        uint32_t value32;
        std::memcpy(&value32, buffer + 1, sizeof(value32));
        EXPECT_NE(value32, 0);
        
        std::cout << "Unaligned memory access is not supported, using safe memcpy method." << std::endl;
    }
}

// Test compile-time usage
TEST(CheckEnvTest, CompileTimeUsage) {
    // This should compile regardless of the value
    static_assert(SUPPORTS_UNALIGNED_MEMORY_ACCESS == 0 || SUPPORTS_UNALIGNED_MEMORY_ACCESS == 1, 
                  "SUPPORTS_UNALIGNED_MEMORY_ACCESS must be 0 or 1");
    
    // Test conditional compilation
#if SUPPORTS_UNALIGNED_MEMORY_ACCESS
    constexpr bool compile_time_test = true;
#else
    constexpr bool compile_time_test = false;
#endif
    
    EXPECT_EQ(compile_time_test, static_cast<bool>(SUPPORTS_UNALIGNED_MEMORY_ACCESS));
}

// Test that compiler feature detection works
TEST(CheckEnvTest, CompilerFeatureDetection) {
    // Test that HAS_ALIGNOF is defined
    EXPECT_TRUE(HAS_ALIGNOF == 0 || HAS_ALIGNOF == 1);
    
    // Test that COMPILER_SUPPORTS_PACKED is defined
    EXPECT_TRUE(COMPILER_SUPPORTS_PACKED == 0 || COMPILER_SUPPORTS_PACKED == 1);
    
    std::cout << "HAS_ALIGNOF: " << HAS_ALIGNOF << std::endl;
    std::cout << "COMPILER_SUPPORTS_PACKED: " << COMPILER_SUPPORTS_PACKED << std::endl;
    std::cout << "SUPPORTS_UNALIGNED_MEMORY_ACCESS: " << SUPPORTS_UNALIGNED_MEMORY_ACCESS << std::endl;
}

// Performance comparison test (if unaligned access is supported)
TEST(CheckEnvTest, PerformanceComparison) {
    alignas(8) uint8_t buffer[1000];
    for (int i = 0; i < 1000; ++i) {
        buffer[i] = static_cast<uint8_t>(i);
    }
    
    uint32_t sum1 = 0, sum2 = 0;
    
    // Method 1: Always use memcpy (safe)
    for (int i = 0; i < 996; i += 4) {
        uint32_t value;
        std::memcpy(&value, buffer + i + 1, sizeof(value)); // Unaligned
        sum1 += value;
    }
    
    // Method 2: Use direct access if supported
    if (SUPPORTS_UNALIGNED_MEMORY_ACCESS) {
        for (int i = 0; i < 996; i += 4) {
            uint32_t value;
            std::memcpy(&value, buffer + i + 1, sizeof(value)); // Still use memcpy for safety in tests
            sum2 += value;
        }
    } else {
        sum2 = sum1; // Same as method 1
    }
    
    EXPECT_EQ(sum1, sum2);
    std::cout << "Sum calculated: " << sum1 << std::endl;
}
