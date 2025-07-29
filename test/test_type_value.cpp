#include <gtest/gtest.h>
#include "type_value.h"
#include <type_traits>
#include <string>

TEST(TypeValueTest, IndexWrapperBasicUsage) {
    // Test basic TypeValue creation and usage
    using MyTypeValue = IndexWrapper<5>::TypeValue<int, 1, 2, 3>;
    
    MyTypeValue tv1(42);
    MyTypeValue tv2(100);
    
    EXPECT_EQ(tv1.value, 42);
    EXPECT_EQ(tv2.value, 100);
    EXPECT_EQ(MyTypeValue::index, 5);
}

TEST(TypeValueTest, IndexWrapperDifferentTypes) {
    // Test with different types
    using IntTypeValue = IndexWrapper<0>::TypeValue<int, 1, 2>;
    using StringTypeValue = IndexWrapper<1>::TypeValue<std::string, 3, 4>;
    using DoubleTypeValue = IndexWrapper<2>::TypeValue<double, 5>;
    
    IntTypeValue iv(123);
    StringTypeValue sv(std::string("hello"));
    DoubleTypeValue dv(3.14);
    
    EXPECT_EQ(iv.value, 123);
    EXPECT_EQ(IntTypeValue::index, 0);
    
    EXPECT_EQ(sv.value, "hello");
    EXPECT_EQ(StringTypeValue::index, 1);
    
    EXPECT_EQ(dv.value, 3.14);
    EXPECT_EQ(DoubleTypeValue::index, 2);
}

TEST(TypeValueContainerTest, BasicUsage) {
    // Test TypeValueContainer basic functionality
    using TypeValue1 = IndexWrapper<0>::TypeValue<int, 1, 2>;
    using TypeValue2 = IndexWrapper<1>::TypeValue<std::string, 3, 4>;
    
    using MyContainer = OutputIndicesWrapper<1, 2, 3, 4>::TypeValueContainer<TypeValue1, TypeValue2>;
    
    TypeValue1 tv1(42);
    TypeValue2 tv2(std::string("test"));
    
    MyContainer container(std::move(tv1), std::move(tv2));
    
    // Test get() methods
    EXPECT_EQ(container.get<0>().value, 42);
    EXPECT_EQ(container.get<1>().value, "test");
    
    // Test const get()
    const auto& const_container = container;
    EXPECT_EQ(const_container.get<0>().value, 42);
    EXPECT_EQ(const_container.get<1>().value, "test");
}

TEST(TypeValueContainerTest, OutputIndicesAccess) {
    // Test access to output indices
    using MyContainer = OutputIndicesWrapper<10, 20, 30>::TypeValueContainer<int>;
    
    // Verify the output_indices is correct
    constexpr auto indices = MyContainer::output_indices;
    static_assert(std::is_same_v<decltype(indices), const std::index_sequence<10, 20, 30>>);
    
    // Test that we can access the size
    EXPECT_EQ(indices.size(), 3);
}

TEST(TypeValueContainerTest, EmptyContainer) {
    // Test empty container
    using EmptyContainer = OutputIndicesWrapper<>::TypeValueContainer<>;
    
    EmptyContainer empty_container;
    constexpr auto empty_indices = EmptyContainer::output_indices;
    static_assert(std::is_same_v<decltype(empty_indices), const std::index_sequence<>>);
    
    EXPECT_EQ(empty_indices.size(), 0);
}

TEST(TypeValueContainerTest, SingleElement) {
    // Test container with single element
    using TypeValue1 = IndexWrapper<99>::TypeValue<double, 5, 6, 7>;
    using SingleContainer = OutputIndicesWrapper<5, 6, 7>::TypeValueContainer<TypeValue1>;
    
    TypeValue1 tv(2.71828);
    SingleContainer container(std::move(tv));
    
    EXPECT_DOUBLE_EQ(container.get<0>().value, 2.71828);
    EXPECT_EQ(container.get<0>().index, 99);
}

TEST(TypeValueContainerTest, MultipleElementsWithModification) {
    // Test modifying values through get()
    using TypeValue1 = IndexWrapper<0>::TypeValue<int, 1>;
    using TypeValue2 = IndexWrapper<1>::TypeValue<int, 2>;
    using TypeValue3 = IndexWrapper<2>::TypeValue<int, 3>;
    
    using MyContainer = OutputIndicesWrapper<1, 2, 3>::TypeValueContainer<TypeValue1, TypeValue2, TypeValue3>;
    
    TypeValue1 tv1(10);
    TypeValue2 tv2(20);
    TypeValue3 tv3(30);
    
    MyContainer container(std::move(tv1), std::move(tv2), std::move(tv3));
    
    // Modify values
    container.get<0>().value = 100;
    container.get<1>().value = 200;
    container.get<2>().value = 300;
    
    // Verify modifications
    EXPECT_EQ(container.get<0>().value, 100);
    EXPECT_EQ(container.get<1>().value, 200);
    EXPECT_EQ(container.get<2>().value, 300);
}

TEST(TypeValueTest, CompileTimeProperties) {
    // Test compile-time properties
    using TypeValue1 = IndexWrapper<42>::TypeValue<int, 1, 2, 3, 4>;
    using Container = OutputIndicesWrapper<1, 2, 3, 4>::TypeValueContainer<TypeValue1>;
    
    // These should all be compile-time constants
    static_assert(TypeValue1::index == 42);
    static_assert(std::tuple_size_v<std::tuple<TypeValue1>> == 1);
    
    // Test that output_indices has correct type
    static_assert(std::is_same_v<
        decltype(Container::output_indices),
        const std::index_sequence<1, 2, 3, 4>
    >);
}

// Test with move-only types
class MoveOnlyType {
public:
    int value;
    
    explicit MoveOnlyType(int v) : value(v) {}
    MoveOnlyType(const MoveOnlyType&) = delete;
    MoveOnlyType& operator=(const MoveOnlyType&) = delete;
    MoveOnlyType(MoveOnlyType&&) = default;
    MoveOnlyType& operator=(MoveOnlyType&&) = default;
};

TEST(TypeValueTest, MoveOnlyType) {
    using MoveOnlyTypeValue = IndexWrapper<7>::TypeValue<MoveOnlyType, 1, 2>;
    using MoveOnlyContainer = OutputIndicesWrapper<1, 2>::TypeValueContainer<MoveOnlyTypeValue>;
    
    MoveOnlyTypeValue tv(MoveOnlyType{99});
    MoveOnlyContainer container(std::move(tv));
    
    EXPECT_EQ(container.get<0>().value.value, 99);
    EXPECT_EQ(container.get<0>().index, 7);
}
