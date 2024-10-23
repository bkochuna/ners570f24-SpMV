#include <SpMV.hpp>
#include <vector> // std::vector

// Testing library required for testing (Always include this last!)
#include "unit_test_framework.hpp"

// Use ASSERT(condition) to test if a condition is true.
// Direct comparison of floating point numbers is not recommended, so we define
// ASSERT_NEAR(a, b, epsilon) to test if a and b are within epsilon of each
// other.

// testing empty JDS construction
template <typename T>
TEST_CASE(empty_construction_test)
{
  SpMV::SparseMatrix_JDS<T> mat;
  ASSERT(mat.getNumCols()==0);
  ASSERT(mat.getNumRows()==0);
  std::cout << "Empty Constructor Test Passed." << std::endl;
}

// testing a defined constructor
template <typename T>
TEST_CASE(defined_construction_test)
{
  int nrows = 3, ncols = 3;
  std::vector<size_t> perm = {0, 1, 2};
  std::vector<size_t> jds_col_ptr = {0, 1, 2};
  std::vector<T> jds_values = {1.0, 2.0, 3.0};
  std::vector<size_t> jd_ptr = {0, 1, 2, 3};
  
  SpMV::SparseMatrix_JDS<T> mat(nrows, ncols, perm, jds_col_ptr, jds_values, jd_ptr);
  ASSERT(mat.getNumRows() == nrows);
  ASSERT(mat.getNumCols() == ncols);

  for (size_t i = 0; i < 3; i++)
  {
    ASSERT(mat.getPerm()[i] == perm[i]);
    ASSERT(mat.getJdsColPtr()[i] == jds_col_ptr[i]);
    ASSERT(mat.getJdsValues()[i] == jds_values[i]);
    ASSERT(mat.getJdPtr[i] == jd_ptr[i]);
  }
  
  std::cout << "Defined Constructor Test Passed." << std::endl;
}

// testing destructor
template <typename T>
TEST_CASE(destruction_test)
{
  {
    SpMV::SparseMatrix_JDS<T> mat;
    mat.~SparseMatrix_JDS();
  }
  std::cout << "Destructor Test Passed." << std::endl;
}

template <typename T>
TEST_SUITE(constructor_destructor)
{
  TEST(empty_construction_test<T>);
  TEST(defined_construction_test<T>);
  TEST(destruction_test<T>);
}

auto
main() -> int
{
  // Run the unit tests. If a test fails, the program will print failure info
  // and return 1.
  RUN_SUITE(constructor_destructor<float>);
  RUN_SUITE(constructor_destructor<double>);
  return 0; 
}
