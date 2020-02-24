#include <iostream>

void testCheckedDelete();
void testCpp();
void testClass();
void testException();
void testInheritance();
void testInheritance2();
void testStlVector();
void testFundamentalType();
void testStatic();
void testFStream();
void testStlStringStream();
void testTemplate();
void testStl();
void testReturnValueOptimization();
void testLambda();
void testStlIterator();
void testStlReverseIterator();
void testStlUnorderedSet();
void testStlSet();
void testStlMap();
void testStlList();
void testStlForwardList();
void testStlArray();
void testChrono();
void testStlString();
void testStlAlgorithm();
void testSizeof();
void testUndefinedBehavior();
void testStructuredBinding();
void testRegex();
void testRvalue();
void testStruct();
void testInline();
void testInline2();
int inlineFunctionWithDifferentImplementation();

// Unlike C, C++ defines an implicit return 0; at the end of int main()
int main()
{
  testInline();
  testInline2();
  std::cout << inlineFunctionWithDifferentImplementation() << '\n';
}