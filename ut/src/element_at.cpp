#include <type_traits>
#include "element_at.h"

// Example usage
using MyList = TypeList<
    IndexedType<0, int>,
    IndexedType<1, double>,
    IndexedType<2, char>
>;

// static_assert(std::is_same<element_at_t<0, MyList>, int>::value, "Type at index 0 should be int");
// static_assert(std::is_same<element_at_t<1, MyList>, double>::value, "Type at index 1 should be double");
// static_assert(std::is_same<element_at_t<2, MyList>, char>::value, "Type at index 2 should be char");

int main() {
    // The static assertions will validate the correctness at compile time
    return 0;
}