#include<iostream>

inline int inlineFunctionWithDifferentImplementation() {
    return 2;
}

void testInline2() {
    std::cout << inlineFunctionWithDifferentImplementation() << "\n";
}
