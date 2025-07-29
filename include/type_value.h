#ifndef TYPE_VALUE_H
#define TYPE_VALUE_H

#include <cstddef>
#include <utility>
#include <tuple>

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
    template <typename... TypeValues>
    struct TypeValueContainer {
        std::tuple<TypeValues...> values;

        explicit TypeValueContainer(TypeValues&&... vals)
            : values(std::forward<TypeValues>(vals)...) {}

        template <std::size_t Index>
        auto& get() {
            return std::get<Index>(values);
        }

        template <std::size_t Index>
        const auto& get() const {
            return std::get<Index>(values);
        }

        // Access to the output indices
        static constexpr auto output_indices = std::index_sequence<Os...>{};
    };
};

#endif // TYPE_VALUE_H
