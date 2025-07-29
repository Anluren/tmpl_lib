#ifndef TYPE_VALUE_H
#define TYPE_VALUE_H

#include <cstddef>
#include <utility>
#include <tuple>
#include <array>

template <std::size_t I>
struct IndexWrapper {
    template <typename T, std::size_t ...Os>
    struct TypeValue {
        T value;
        static constexpr std::size_t index = I;

        explicit TypeValue(const T& val)
            : value(val) {}

        explicit TypeValue(T&& val)
            : value(std::move(val)) {}
    };
};

// Using nested structure approach for TypeValueContainer
template <std::size_t ...Os>
struct OutputIndicesWrapper {
    // Compile-time function to get value at given index from Os parameter pack
    template <std::size_t Index>
    static constexpr std::size_t get_value_at() {
        static_assert(Index < sizeof...(Os), "Index out of range");
        constexpr std::array<std::size_t, sizeof...(Os)> values = {Os...};
        return values[Index];
    }

    // Get the size of the parameter pack
    static constexpr std::size_t size() {
        return sizeof...(Os);
    }

    template <typename... TypeValues>
    struct TypeValueContainer : public TypeValues... {
        
        explicit TypeValueContainer(TypeValues&&... vals)
            : TypeValues(std::forward<TypeValues>(vals))... {}

        template <std::size_t Index>
        auto& get() {
            return std::get<Index>(std::forward_as_tuple(static_cast<TypeValues&>(*this)...));
        }

        template <std::size_t Index>
        const auto& get() const {
            return std::get<Index>(std::forward_as_tuple(static_cast<const TypeValues&>(*this)...));
        }

        // Access to the output indices
        static constexpr auto output_indices = std::index_sequence<Os...>{};
        
        // Compile-time function to get value at given index from Os parameter pack
        template <std::size_t Index>
        static constexpr std::size_t get_output_index_at() {
            return OutputIndicesWrapper::template get_value_at<Index>();
        }
        
        // Get the size of the output indices
        static constexpr std::size_t output_size() {
            return OutputIndicesWrapper::size();
        }
    };
};

#endif // TYPE_VALUE_H
