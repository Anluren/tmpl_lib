# Advanced C++ Metaprogramming Sequence Library Documentation

## Overview

This library provides advanced C++ metaprogramming utilities for compile-time integer sequences, type traits, and related template tricks. All functionality is implemented using modern C++17 features and is thoroughly tested with GoogleTest.

## Table of Contents

1. [Core Sequence Utilities](#core-sequence-utilities)
2. [Index Sequence Utilities](#index-sequence-utilities)
3. [Word Alignment Utilities](#word-alignment-utilities)
4. [Reverse Operations](#reverse-operations)
5. [Testing](#testing)
6. [Build System](#build-system)
7. [Static Initialization Patterns](#static-initialization-patterns)

---

## Core Sequence Utilities

### `total_seq_helper<int... Vs>`

The main helper struct for generating sequences of total values from variadic template parameters.

#### Basic Properties

```cpp
template <int... Vs>
struct total_seq_helper {
    static constexpr int total_value = (Vs + ...);  // Sum of all values
    
    // Get value at specific index
    template <std::size_t I>
    static constexpr auto get_value_at() noexcept;
    
    // Get first value in sequence
    static constexpr auto get_first_value() noexcept;
};
```

#### Example Usage

```cpp
using helper = total_seq_helper<1, 2, 3, 4, 5>;
static_assert(helper::total_value == 15);
static_assert(helper::get_value_at<0>() == 1);
static_assert(helper::get_first_value() == 1);
```

### Cumulative Total Functions

#### `gen_total_at<I>()`
Generates cumulative totals up to index I (exclusive).

```cpp
using helper = total_seq_helper<1, 2, 3, 4>;
static_assert(helper::gen_total_at<0>() == 0);  // Before first element
static_assert(helper::gen_total_at<1>() == 1);  // After first element
static_assert(helper::gen_total_at<2>() == 3);  // After second element (1+2)
static_assert(helper::gen_total_at<3>() == 6);  // After third element (1+2+3)
```

#### `make_total_value_sequence<int... Vs>`
Creates an integer sequence of cumulative totals.

```cpp
using seq = make_total_value_sequence<1, 2, 3>;
// Results in: std::integer_sequence<int, 0, 1, 3>
```

### Search Functions

#### `find_first_equal<V>()`
Finds the index of the first element equal to V.

```cpp
using helper = total_seq_helper<10, 20, 30, 40>;
static_assert(helper::find_first_equal<20>() == 1);
static_assert(helper::find_first_equal<99>() == 4);  // Not found, returns size
```

#### `find_first_greater_than<V>()`
Finds the index of the first element greater than V.

```cpp
static_assert(helper::find_first_greater_than<15>() == 1);  // 20 > 15
static_assert(helper::find_first_greater_than<50>() == 4);  // None greater, returns size
```

### Convenience Templates

```cpp
// Get value at index I
template <std::size_t I, int... Vs>
constexpr auto get_value_at = total_seq_helper<Vs...>::template get_value_at<I>();

// Find index with specific value
template <int V, int... Vs>
constexpr std::size_t get_index_with_value = 
    total_seq_helper<Vs...>::template find_first_equal<V>();

// Find index with first greater value
template <int V, int... Vs>
constexpr std::size_t get_index_with_greater_value = 
    total_seq_helper<Vs...>::template find_first_greater_than<V>();
```

---

## Index Sequence Utilities

### Range-Based Index Sequences

#### `make_index_sequence_range<Min, Max>()`
Creates an index sequence in the range [Min, Max).

```cpp
auto range_seq = make_index_sequence_range<3, 7>();
// Results in: std::index_sequence<3, 4, 5, 6>

using range_t = make_index_sequence_range_t<2, 5>;
// Results in: std::index_sequence<2, 3, 4>
```

### One-Based Index Sequences

#### `make_one_based_index_sequence<N>`
Creates a 1-based index sequence where the last element is 0.

```cpp
using one_based = make_one_based_index_sequence<4>;
// Results in: std::integer_sequence<std::size_t, 1, 2, 3, 0>
```

### Sequence Splitting

#### `split_sequence<N, M>()`
Splits a sequence of size M into two sequences: first N elements and remaining M-N elements.

```cpp
auto result = split_sequence<3, 7>();
// result.first:  std::index_sequence<0, 1, 2>
// result.second: std::integer_sequence<std::size_t, 3, 4, 5, 6>
```

---

## Word Alignment Utilities

### `split_total_seq_helper<W, int... Vs>`

Helper for splitting sequences into word-size aligned chunks.

```cpp
template <int W, int... Vs>
struct split_total_seq_helper {
    static constexpr int total_value = (Vs + ...);
    static constexpr std::size_t word_count = total_value / W;
    
    static_assert(total_value % W == 0, "total value must be divisible by W");
    
    using make_word_index_sequence = /* implementation details */;
    static constexpr bool is_word_size_aligned = /* implementation details */;
};
```

#### Example Usage

```cpp
// Test with word size 8 and values that sum to 16
using helper = split_total_seq_helper<8, 4, 4, 8>;
static_assert(helper::total_value == 16);
static_assert(helper::word_count == 2);
static_assert(helper::is_word_size_aligned);
```

### Word Alignment Checking

#### `is_word_size_aligned<W, int... Vs>`
Compile-time check if values are word-size aligned.

```cpp
static_assert(is_word_size_aligned<4, 4, 4, 4, 4>);  // 20 % 4 == 0
static_assert(is_word_size_aligned<8, 8, 8, 8>);     // 32 % 8 == 0

// These would cause compile-time errors:
// static_assert(is_word_size_aligned<8, 7, 8>);     // 15 % 8 != 0
// static_assert(is_word_size_aligned<4, 3, 4, 4>);  // 11 % 4 != 0
```

---

## Reverse Operations

### Reverse Index Sequences

#### `make_reverse_index_sequence<N>()`
Creates an index sequence with indices in reverse order.

```cpp
auto reversed = make_reverse_index_sequence<5>();
// Results in: std::index_sequence<4, 3, 2, 1, 0>

using reversed_t = make_reverse_index_sequence_t<3>;
// Results in: std::index_sequence<2, 1, 0>
```

#### `reverse_index_sequence(seq)`
Reverses an existing index sequence.

```cpp
constexpr auto original = std::index_sequence<0, 1, 2, 3>{};
auto reversed = reverse_index_sequence(original);
// Results in: std::index_sequence<3, 2, 1, 0>
```

### Reverse Total Value Sequences

#### `gen_reverse_total_at<I>()`
Generates the sum of all values after index I.

```cpp
using helper = total_seq_helper<1, 2, 3, 4>;
static_assert(helper::gen_reverse_total_at<0>() == 9);  // Sum after index 0: 2+3+4
static_assert(helper::gen_reverse_total_at<1>() == 7);  // Sum after index 1: 3+4
static_assert(helper::gen_reverse_total_at<2>() == 4);  // Sum after index 2: 4
static_assert(helper::gen_reverse_total_at<3>() == 0);  // Sum after index 3: (empty)
```

#### `make_reverse_total_value_sequence<int... Vs>`
Creates an integer sequence where each element contains the sum of all values after its position.

```cpp
// For sequence [1, 2, 3, 4]:
using seq = make_reverse_total_value_sequence<1, 2, 3, 4>;
// Results in: std::integer_sequence<int, 9, 7, 4, 0>
// Where: 9 = 2+3+4, 7 = 3+4, 4 = 4, 0 = (empty)

// For sequence [5, 10, 15]:
using seq2 = make_reverse_total_value_sequence<5, 10, 15>;
// Results in: std::integer_sequence<int, 25, 15, 0>

// For single element [42]:
using seq3 = make_reverse_total_value_sequence<42>;
// Results in: std::integer_sequence<int, 0>
```

---

## Testing

The library includes comprehensive unit tests using GoogleTest framework:

### Test Coverage

- **20 total tests** covering all functionality
- **Compile-time validation** with static_assert
- **Runtime behavior verification** with EXPECT_* macros
- **Edge cases** including single elements, zero values, and large sequences
- **Type trait verification** using std::is_same_v

### Running Tests

```bash
cd build
make seq_test
./test/seq_test
```

### Example Test Cases

```cpp
// Basic functionality test
TEST(SeqTest, TotalSeqHelperBasic) {
    using helper = total_seq_helper<1, 2, 3, 4, 5>;
    EXPECT_EQ(helper::total_value, 15);
    EXPECT_EQ(helper::get_value_at<0>(), 1);
    EXPECT_EQ(helper::get_first_value(), 1);
}

// Compile-time evaluation test
TEST(SeqTest, CompileTimeEvaluation) {
    static_assert(total_seq_helper<1, 2, 3>::total_value == 6);
    static_assert(get_value_at<0, 100, 200, 300> == 100);
    static_assert(is_word_size_aligned<8, 8, 8, 8>);
    SUCCEED();
}
```

---

## Build System

### CMake Configuration

The project uses CMake with modern C++17 support:

```cmake
cmake_minimum_required(VERSION 3.12)
project(MyProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Export compile commands for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
```

### IDE Support

#### clangd Configuration (`.clangd`)

```yaml
CompileFlags:
  Add:
    - -std=c++17
    - -Wall
    - -Wextra
    - -I/Users/dzheng/workspace/tmpl_lib/include
    - -I/Users/dzheng/workspace/tmpl_lib/util_macro
    - -I/Users/dzheng/workspace/tmpl_lib/googletest/googletest/include
    - -I/Users/dzheng/workspace/tmpl_lib/googletest/googlemock/include

Diagnostics:
  ClangTidy:
    Add:
      - modernize-*
      - readability-*
      - performance-*
    Remove:
      - modernize-use-trailing-return-type
      - readability-identifier-length

Index:
  Background: Build
```

### Build Commands

```bash
# Configure and build
mkdir build && cd build
cmake ..
make

# Run all tests
make test

# Run specific test
./test/seq_test
```

---

## Static Initialization Patterns

For executing code during initialization in each source file with global std::map access, here are the recommended patterns:

### 1. Function-Local Static (Recommended)

```cpp
// globals.h
#include <map>
#include <string>

std::map<std::string, int>& getGlobalMap() {
    static std::map<std::string, int> globalMap;
    return globalMap;
}

bool registerInGlobalMap(const std::string& key, int value) {
    getGlobalMap()[key] = value;
    return true;
}

// source1.cpp
namespace {
    bool initialized = registerInGlobalMap("source1", 42);
}
```

### 2. Registration Class with Constructor

```cpp
// registry.h
class GlobalRegistry {
public:
    static std::map<std::string, int>& getMap() {
        static std::map<std::string, int> map;
        return map;
    }
    
    GlobalRegistry(const std::string& key, int value) {
        getMap()[key] = value;
    }
};

#define REGISTER_GLOBAL(key, value) \
    namespace { \
        static GlobalRegistry reg_##__LINE__(key, value); \
    }

// source1.cpp
REGISTER_GLOBAL("source1_feature", 42);
```

### 3. Template-Based Auto-Registration

```cpp
// auto_register.h
class AutoRegister {
public:
    using InitFunction = std::function<void()>;
    
    static std::map<std::string, int>& getGlobalMap() {
        static std::map<std::string, int> map;
        return map;
    }
    
    AutoRegister(const std::string& name, InitFunction func) {
        func();
    }
};

#define AUTO_REGISTER(name, code) \
    namespace { \
        AutoRegister reg_##__LINE__(name, []() { code }); \
    }

// source1.cpp
AUTO_REGISTER("source1", {
    AutoRegister::getGlobalMap()["source1_key"] = 42;
});
```

### Key Benefits

- **Thread-safe**: C++11 guarantees thread-safe initialization of function-local statics
- **No initialization order issues**: Maps are created when first accessed
- **Lazy initialization**: Resources created only when needed
- **Clean and maintainable**: Easy to understand and extend

---

## Repository Information

- **GitHub**: https://github.com/Anluren/tmpl_lib.git
- **Language**: C++17
- **Build System**: CMake
- **Testing**: GoogleTest
- **All tests passing**: ✅ 20/20 tests successful

## Recent Updates

- ✅ Added reverse total value sequence functionality
- ✅ Fixed C++ implementation issues (get_first_value, find_first_greater_than)
- ✅ Replaced C-style arrays with std::array for type safety
- ✅ Updated CMake for C++17 and compile commands export
- ✅ Added comprehensive clangd configuration
- ✅ All 20 tests passing successfully
- ✅ Documentation and examples provided
