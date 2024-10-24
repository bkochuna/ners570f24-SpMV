#include <SpMV.hpp>

#include <vector> // std::vector

// Testing library required for testing (Always include this last!)
#include "unit_test_framework.hpp"

// for checking types
#include <typeinfo>
// #include <type_traits>

// Use ASSERT(condition) to test if a condition is true.
// Direct comparison of floating point numbers is not recommended, so we define
// ASSERT_NEAR(a, b, epsilon) to test if a and b are within epsilon of each
// other.

// test to see if we get an object after calling the default constructor
template <typename T>
TEST_CASE(checkDefaultConstructor)
{
  // construct class
  SpMV::SparseMatrix_COO<T> *A = nullptr;

  // initialize by default constructor
  A = new SpMV::SparseMatrix_COO<T>();

  // test that the object exists
  ASSERT(A != nullptr);

}

// check parametrized constructor
template <typename T>
TEST_CASE(checkParametrizedConstructor)
{
  // construct class
  SpMV::SparseMatrix_COO<T> *A = nullptr;

  // initialize by parametrized constructor
  A = new SpMV::SparseMatrix_COO<T>(2,5);

  // test that the object exists
  ASSERT(A != nullptr);

}

// test that the parametrized constructor throws errors for negative


// test to see if we can access class properties that we expect
template <typename T>
TEST_CASE(checkObjectProperties)
{

  // construct class
  SpMV::SparseMatrix_COO<T> *A = nullptr;
  A = new SpMV::SparseMatrix_COO<T>(1, 5);

  // check that matrix state is set to initialized
  ASSERT(A->getState()==SpMV::initialized);

  // test that object properties were set correctly
  ASSERT(A->getNumRows()==1);
  ASSERT(A->getNumCols()==5);

  // test data types of variables
  ASSERT(typeid(*(A->getRowIndices())) == typeid(size_t));
  ASSERT(typeid(*(A->getColIndices())) == typeid(size_t));

  ASSERT(typeid(*(A->getValues())) == typeid(T));
  ASSERT(typeid(A->getCoefficient(1,1)) == typeid(T));

}

// check to see if we still have an object after calling the destructor
template <typename T>
TEST_CASE(checkDestructor)
{

  // construct class
  SpMV::SparseMatrix_COO<T> *A = nullptr;

  // initialize by default constructor
  A = new SpMV::SparseMatrix_COO<T>();

  // delete object
  delete A;

  // if the code reached this point, the destructor was called without an error
  ASSERT(true);

}



template <typename T>
TEST_SUITE(constructorDestructorTests) 
{
  TEST(checkDefaultConstructor<T>);
  TEST(checkParametrizedConstructor<T>);
  TEST(checkObjectProperties<T>);
  TEST(checkDestructor<T>);

} // add_sub_suite

auto
main() -> int
{
  // Run the unit tests. If a test fails, the program will print failure info
  // and return 1.
  RUN_SUITE(constructorDestructorTests<float>);
  RUN_SUITE(constructorDestructorTests<double>);
  return 0; 
}
