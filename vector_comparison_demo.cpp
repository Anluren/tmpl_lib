#include <vector>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <sstream>
#include <cstdint>
#include <optional>

// ANSI color codes for terminal output
namespace Colors {
    const std::string RESET = "\033[0m";
    const std::string RED = "\033[31m";
    const std::string GREEN = "\033[32m";
    const std::string YELLOW = "\033[33m";
    const std::string BLUE = "\033[34m";
    const std::string MAGENTA = "\033[35m";
    const std::string CYAN = "\033[36m";
    const std::string WHITE = "\033[37m";
    const std::string BOLD = "\033[1m";
    const std::string BG_RED = "\033[41m";
    const std::string BG_GREEN = "\033[42m";
    const std::string BG_YELLOW = "\033[43m";
}

class VectorComparator {
public:
    struct Difference {
        std::size_t position;
        uint8_t value1;
        uint8_t value2;
        
        Difference(std::size_t pos, uint8_t v1, uint8_t v2) 
            : position(pos), value1(v1), value2(v2) {}
    };
    
    // Compare two memory regions and return all differences
    static std::vector<Difference> compare(const uint8_t* vec1, std::size_t len1,
                                         const uint8_t* vec2, std::size_t len2) {
        std::vector<Difference> differences;
        
        std::size_t max_size = std::max(len1, len2);
        
        for (std::size_t i = 0; i < max_size; ++i) {
            uint8_t v1 = (i < len1) ? vec1[i] : 0;
            uint8_t v2 = (i < len2) ? vec2[i] : 0;
            
            if (v1 != v2 || i >= len1 || i >= len2) {
                differences.emplace_back(i, v1, v2);
            }
        }
        
        return differences;
    }
    
    // Print side-by-side comparison with color highlighting
    static void print_comparison(const uint8_t* vec1, std::size_t len1,
                               const uint8_t* vec2, std::size_t len2,
                               std::size_t bytes_per_line = 32) {
        std::size_t max_size = std::max(len1, len2);
        
        std::cout << Colors::BOLD << Colors::CYAN << "Vector comparison (showing hex values, 32 bytes per line):" << Colors::RESET << "\n";
        std::cout << Colors::BOLD << "Position   | Vec1 Data" << std::string(117, ' ') << "| Vec2 Data" << std::string(117, ' ') << "|" << Colors::RESET << "\n";
        std::cout << std::string(11, '-') << "|" << std::string(120, '-') << "|" << std::string(120, '-') << "|" << "\n";
        
        for (std::size_t i = 0; i < max_size; i += bytes_per_line) {
            // Print position
            std::cout << Colors::BLUE << std::setfill('0') << std::setw(8) << std::hex << i << Colors::RESET << "   | ";
            
            // Print vec1 line with color coding
            for (std::size_t j = 0; j < bytes_per_line && (i + j) < max_size; ++j) {
                bool has_diff = false;
                if (i + j < len1 && i + j < len2) {
                    has_diff = (vec1[i + j] != vec2[i + j]);
                } else {
                    has_diff = true; // Size difference
                }
                
                if (i + j < len1) {
                    if (has_diff) {
                        std::cout << Colors::BG_RED << Colors::WHITE;
                    } else {
                        std::cout << Colors::GREEN;
                    }
                    std::cout << std::setfill('0') << std::setw(2) << std::hex 
                             << static_cast<unsigned>(vec1[i + j]) << Colors::RESET << " ";
                } else {
                    std::cout << Colors::YELLOW << "--" << Colors::RESET << " ";
                }
            }
            
            // Pad to fixed width
            std::size_t printed_bytes = std::min(bytes_per_line, max_size - i);
            for (std::size_t k = printed_bytes; k < bytes_per_line; ++k) {
                std::cout << "   ";
            }
            std::cout << "| ";
            
            // Print vec2 line with color coding
            for (std::size_t j = 0; j < bytes_per_line && (i + j) < max_size; ++j) {
                bool has_diff = false;
                if (i + j < len1 && i + j < len2) {
                    has_diff = (vec1[i + j] != vec2[i + j]);
                } else {
                    has_diff = true; // Size difference
                }
                
                if (i + j < len2) {
                    if (has_diff) {
                        std::cout << Colors::BG_RED << Colors::WHITE;
                    } else {
                        std::cout << Colors::GREEN;
                    }
                    std::cout << std::setfill('0') << std::setw(2) << std::hex 
                             << static_cast<unsigned>(vec2[i + j]) << Colors::RESET << " ";
                } else {
                    std::cout << Colors::YELLOW << "--" << Colors::RESET << " ";
                }
            }
            
            // Pad to fixed width
            for (std::size_t k = printed_bytes; k < bytes_per_line; ++k) {
                std::cout << "   ";
            }
            std::cout << "|\n";
        }
        
        // Print legend
        std::cout << "\n" << Colors::BOLD << "Legend:" << Colors::RESET << "\n";
        std::cout << Colors::GREEN << "●" << Colors::RESET << " Matching bytes  ";
        std::cout << Colors::BG_RED << Colors::WHITE << "●" << Colors::RESET << " Different bytes  ";
        std::cout << Colors::YELLOW << "●" << Colors::RESET << " Missing bytes\n";
        
        std::cout << std::dec; // Reset to decimal
    }
    
    // Print interleaved comparison (vec1 and vec2 on adjacent lines)
    static void print_interleaved_comparison(const uint8_t* vec1, std::size_t len1,
                                           const uint8_t* vec2, std::size_t len2,
                                           std::size_t bytes_per_line = 32) {
        std::size_t max_size = std::max(len1, len2);
        
        std::cout << Colors::BOLD << Colors::CYAN << "Interleaved Vector Comparison (32 bytes per line):" << Colors::RESET << "\n";
        std::cout << Colors::BOLD << "Position   | Data (Vec1 then Vec2 on adjacent lines)" << Colors::RESET << "\n";
        std::cout << std::string(11, '-') << "|" << std::string(101, '-') << "\n";
        
        for (std::size_t i = 0; i < max_size; i += bytes_per_line) {
            // Print ruler line for every 16 bytes on first chunk
            if (i == 0) {
                std::cout << Colors::CYAN << "           | Ruler:";
                for (std::size_t j = 0; j < bytes_per_line; ++j) {
                    if (j % 16 == 0) {
                        std::cout << Colors::BOLD << Colors::BLUE << std::setfill('0') << std::setw(2) << std::hex << j << Colors::RESET;
                    } else {
                        std::cout << Colors::CYAN << ".." << Colors::RESET;
                    }
                    std::cout << " ";
                }
                std::cout << "\n";
                std::cout << "           |" << std::string(101, '-') << "\n";
            }
            // Print position for this chunk
            std::cout << Colors::BLUE << std::setfill('0') << std::setw(8) << std::hex << i << Colors::RESET << "   | ";
            
            // Print vec1 line with color coding
            std::cout << Colors::BOLD << "Vec1: " << Colors::RESET;
            for (std::size_t j = 0; j < bytes_per_line && (i + j) < max_size; ++j) {
                bool has_diff = false;
                if (i + j < len1 && i + j < len2) {
                    has_diff = (vec1[i + j] != vec2[i + j]);
                } else {
                    has_diff = true; // Size difference
                }
                
                if (i + j < len1) {
                    if (has_diff) {
                        std::cout << Colors::BG_RED << Colors::WHITE;
                    } else {
                        std::cout << Colors::GREEN;
                    }
                    std::cout << std::setfill('0') << std::setw(2) << std::hex 
                             << static_cast<unsigned>(vec1[i + j]) << Colors::RESET << " ";
                } else {
                    std::cout << Colors::YELLOW << "--" << Colors::RESET << " ";
                }
            }
            std::cout << "\n";
            
            // Print vec2 line directly below vec1 with same position
            std::cout << "           | " << Colors::BOLD << "Vec2: " << Colors::RESET;
            for (std::size_t j = 0; j < bytes_per_line && (i + j) < max_size; ++j) {
                bool has_diff = false;
                if (i + j < len1 && i + j < len2) {
                    has_diff = (vec1[i + j] != vec2[i + j]);
                } else {
                    has_diff = true; // Size difference
                }
                
                if (i + j < len2) {
                    if (has_diff) {
                        std::cout << Colors::BG_RED << Colors::WHITE;
                    } else {
                        std::cout << Colors::GREEN;
                    }
                    std::cout << std::setfill('0') << std::setw(2) << std::hex 
                             << static_cast<unsigned>(vec2[i + j]) << Colors::RESET << " ";
                } else {
                    std::cout << Colors::YELLOW << "--" << Colors::RESET << " ";
                }
            }
            std::cout << "\n";
            
            // Print difference indicators (^ symbols) under differing positions
            std::cout << "           | " << std::string(6, ' '); // Align with "Vec2: " prefix
            for (std::size_t j = 0; j < bytes_per_line && (i + j) < max_size; ++j) {
                bool has_diff = false;
                if (i + j < len1 && i + j < len2) {
                    has_diff = (vec1[i + j] != vec2[i + j]);
                } else {
                    has_diff = true; // Size difference
                }
                
                if (has_diff && (i + j < len1 || i + j < len2)) {
                    std::cout << Colors::RED << "^" << Colors::RESET << "  ";
                } else {
                    std::cout << "   ";
                }
            }
            std::cout << "\n";
            
            // Add a separator line between chunks for clarity
            if (i + bytes_per_line < max_size) {
                std::cout << "           |" << std::string(101, '.') << "\n";
            }
        }
        
        // Print legend
        std::cout << "\n" << Colors::BOLD << "Legend:" << Colors::RESET << "\n";
        std::cout << Colors::GREEN << "●" << Colors::RESET << " Matching bytes  ";
        std::cout << Colors::BG_RED << Colors::WHITE << "●" << Colors::RESET << " Different bytes  ";
        std::cout << Colors::YELLOW << "●" << Colors::RESET << " Missing bytes\n";
        
        std::cout << std::dec; // Reset to decimal
    }
    
    // Compact difference summary
    static void print_differences(const uint8_t* vec1, std::size_t len1,
                                const uint8_t* vec2, std::size_t len2) {
        auto differences = compare(vec1, len1, vec2, len2);
        
        if (differences.empty()) {
            std::cout << "✓ Vectors are identical\n";
            return;
        }
        
        std::cout << "✗ Found " << differences.size() << " differences:\n";
        std::cout << "Pos   | Vec1 | Vec2 | Decimal Diff\n";
        std::cout << "------|------|------|-------------\n";
        
        for (const auto& diff : differences) {
            std::cout << std::setfill(' ') << std::setw(5) << std::dec << diff.position << " | "
                     << std::setfill('0') << std::setw(2) << std::hex 
                     << static_cast<unsigned>(diff.value1) << "   | "
                     << std::setfill('0') << std::setw(2) << std::hex 
                     << static_cast<unsigned>(diff.value2) << "   | "
                     << std::dec << static_cast<int>(diff.value2) - static_cast<int>(diff.value1) << "\n";
        }
    }
    
    // Statistical summary
    static void print_statistics(const uint8_t* vec1, std::size_t len1,
                               const uint8_t* vec2, std::size_t len2) {
        auto differences = compare(vec1, len1, vec2, len2);
        std::size_t common_length = std::min(len1, len2);
        std::size_t max_length = std::max(len1, len2);
        
        std::cout << "\n=== Comparison Statistics ===\n";
        std::cout << "Vector 1 size: " << len1 << " bytes\n";
        std::cout << "Vector 2 size: " << len2 << " bytes\n";
        std::cout << "Size difference: " << static_cast<int>(len2) - static_cast<int>(len1) << " bytes\n";
        std::cout << "Total differences: " << differences.size() << "\n";
        std::cout << "Common length: " << common_length << " bytes\n";
        
        if (max_length > 0) {
            double match_ratio = static_cast<double>(max_length - differences.size()) / max_length * 100.0;
            std::cout << "Match ratio: " << std::fixed << std::setprecision(2) << match_ratio << "%\n";
        }
    }
};

// Quick utility functions for common use cases
namespace VectorDiff {
    // Quick check if memory regions are identical
    bool are_identical(const uint8_t* vec1, std::size_t len1, const uint8_t* vec2, std::size_t len2) {
        if (len1 != len2) return false;
        return std::equal(vec1, vec1 + len1, vec2);
    }
    
    // Find first difference position
    std::optional<std::size_t> first_difference(const uint8_t* vec1, std::size_t len1,
                                               const uint8_t* vec2, std::size_t len2) {
        std::size_t min_len = std::min(len1, len2);
        auto mismatch_pair = std::mismatch(vec1, vec1 + min_len, vec2);
        if (mismatch_pair.first != vec1 + min_len) {
            return std::distance(vec1, mismatch_pair.first);
        }
        // Check if one is longer than the other
        if (len1 != len2) {
            return min_len;
        }
        return std::nullopt;
    }
    
    // Count total differences
    std::size_t count_differences(const uint8_t* vec1, std::size_t len1,
                                const uint8_t* vec2, std::size_t len2) {
        return VectorComparator::compare(vec1, len1, vec2, len2).size();
    }
    
    // Generate diff string (similar to unified diff format)
    std::string generate_diff_string(const uint8_t* vec1, std::size_t len1,
                                   const uint8_t* vec2, std::size_t len2) {
        std::ostringstream oss;
        auto differences = VectorComparator::compare(vec1, len1, vec2, len2);
        
        if (differences.empty()) {
            oss << "No differences found\n";
            return oss.str();
        }
        
        oss << "--- Vector1\n";
        oss << "+++ Vector2\n";
        oss << "@@ Differences: " << differences.size() << " @@\n";
        
        for (const auto& diff : differences) {
            oss << "-[" << diff.position << "] 0x" << std::hex << std::setfill('0') 
                << std::setw(2) << static_cast<unsigned>(diff.value1) << "\n";
            oss << "+[" << diff.position << "] 0x" << std::hex << std::setfill('0') 
                << std::setw(2) << static_cast<unsigned>(diff.value2) << "\n";
        }
        
        return oss.str();
    }
}

int main() {
    // Create larger test vectors to showcase the 40-byte display
    std::vector<uint8_t> vec1;
    std::vector<uint8_t> vec2;
    
    // Generate test data - vec1 with pattern
    for (int i = 0; i < 80; ++i) {
        vec1.push_back(static_cast<uint8_t>(i % 256));
    }
    
    // Generate test data - vec2 with some differences
    vec2 = vec1; // Start with same data
    vec2[5] = 0xFF;   // Change some bytes
    vec2[15] = 0xAA;
    vec2[25] = 0xBB;
    vec2[35] = 0xCC;
    vec2[45] = 0xDD;
    vec2.push_back(0xEE); // Add extra byte
    vec2.push_back(0xFF);
    
    // Get pointers and lengths for the new API
    const uint8_t* data1 = vec1.data();
    std::size_t len1 = vec1.size();
    const uint8_t* data2 = vec2.data();
    std::size_t len2 = vec2.size();
    
    std::cout << Colors::BOLD << Colors::MAGENTA << "=== Vector Comparison Demo with Color Highlighting ===" << Colors::RESET << "\n\n";
    
    std::cout << "=== Method 1: Simple equality check ===\n";
    std::cout << "Vectors identical: " << (VectorDiff::are_identical(data1, len1, data2, len2) ? "Yes" : "No") << "\n";
    
    std::cout << "\n=== Method 2: Find first difference ===\n";
    auto first_diff = VectorDiff::first_difference(data1, len1, data2, len2);
    if (first_diff) {
        std::cout << "First difference at position: " << Colors::YELLOW << *first_diff << Colors::RESET << "\n";
    } else {
        std::cout << "No differences found\n";
    }
    
    std::cout << "\n=== Method 3: Count differences ===\n";
    std::cout << "Total differences: " << Colors::RED << VectorDiff::count_differences(data1, len1, data2, len2) << Colors::RESET << "\n";
    
    std::cout << "\n=== Method 4: Detailed comparison ===\n";
    VectorComparator::print_differences(data1, len1, data2, len2);
    
    std::cout << "\n=== Method 5: Side-by-side view (32 bytes per line with colors) ===\n";
    VectorComparator::print_comparison(data1, len1, data2, len2); // Uses default 32 bytes per line
    
    std::cout << "\n=== Method 6: Interleaved view (vec1 and vec2 on adjacent lines) ===\n";
    VectorComparator::print_interleaved_comparison(data1, len1, data2, len2); // Uses default 32 bytes per line
    
    std::cout << "\n=== Method 7: Statistics ===\n";
    VectorComparator::print_statistics(data1, len1, data2, len2);
    
    std::cout << "\n=== Method 8: Diff string format ===\n";
    std::cout << VectorDiff::generate_diff_string(data1, len1, data2, len2);
    
    return 0;
}
