#pragma once

// Check if the target architecture supports unaligned memory access
// Use compiler-specific attributes and feature detection instead of exhaustive arch list

#if defined(__has_attribute)
    #if __has_attribute(__packed__)
        #define COMPILER_SUPPORTS_PACKED 1
    #else
        #define COMPILER_SUPPORTS_PACKED 0
    #endif
#else
    #define COMPILER_SUPPORTS_PACKED 0
#endif

// Check for unaligned access support using standard library features
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L)
    // C11 has _Alignof which can help detect alignment requirements
    #include <stdalign.h>
    #define HAS_ALIGNOF 1
#elif defined(__cplusplus) && (__cplusplus >= 201103L)
    // C++11 has alignof
    #define HAS_ALIGNOF 1
#else
    #define HAS_ALIGNOF 0
#endif

// Use a more general approach: test if strict alignment is NOT required
#if defined(__BIGGEST_ALIGNMENT__) && (__BIGGEST_ALIGNMENT__ <= 8)
    // If biggest alignment requirement is reasonable, likely supports unaligned access
    #define SUPPORTS_UNALIGNED_MEMORY_ACCESS 1
#elif defined(__GNUC__) && defined(__CHAR_BIT__) && (__CHAR_BIT__ == 8)
    // GCC with 8-bit bytes usually means common architectures that support unaligned access
    #define SUPPORTS_UNALIGNED_MEMORY_ACCESS 1
#elif defined(_MSC_VER)
    // MSVC typically targets x86/x64 which support unaligned access
    #define SUPPORTS_UNALIGNED_MEMORY_ACCESS 1
#elif defined(__clang__)
    // Clang usually targets modern architectures
    #define SUPPORTS_UNALIGNED_MEMORY_ACCESS 1
#else
    // Conservative default: assume no unaligned access support
    #define SUPPORTS_UNALIGNED_MEMORY_ACCESS 0
#endif

// Alternative macro name for compatibility
#define HAS_UNALIGNED_MEMORY_ACCESS SUPPORTS_UNALIGNED_MEMORY_ACCESS

// Example usage with static_assert
#ifdef __cplusplus
namespace check_env {
    constexpr bool supports_unaligned_access = SUPPORTS_UNALIGNED_MEMORY_ACCESS;
}
#endif
