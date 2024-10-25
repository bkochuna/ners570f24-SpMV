#include <SpMV.hpp>
#include <vector> // std::vector
#include "unit_test_framework.hpp"

// Testing empty constructor for ELL
template <typename T>
TEST_CASE(test_empty_construct_ell)
{
  SpMV::SparseMatrix_ELL<T> A;

  assert(A.getLmax() == 0);
  assert(A.getNnzs() == 0);
  assert(A.getColIdx() == nullptr);
  assert(A.getVal() == nullptr);

  assert(A.getNumRows() == 0);
  assert(A.getNumCols() == 0);

  std::cout << "Empty constructor test for ELL passed." << std::endl;
}

// Testing constructor for ELL format
template <typename T>
TEST_CASE(test_construct_ell)
{
  int nrows = 4, ncols = 4;
  SpMV::SparseMatrix_ELL<T> A(nrows, ncols);
  
  // Testing for proper initialization
  assert(A.getLmax() == 0);
  assert(A.getNnzs() == 0);
  assert(A.getColIdx() == nullptr);
  assert(A.getVal() == nullptr);
  
  // Testing for correct matrix dimensions
  assert(A.getNumRows() == nrows);
  assert(A.getNumCols() == ncols);

  std::cout << "Constructor test for ELL passed." << std::endl;
}

// Testing destructor for ELL
template <typename T>
TEST_CASE(test_destruct_ell)
{
  {
    SpMV::SparseMatrix_ELL<T> A;
    A.~SparseMatrix_ELL();
  }
  std::cout << "Destructor test for ELL passed." << std::endl;
}

// Test suite for constructor/destructor tests
template <typename T>
TEST_SUITE(test_constructor_destructor_ell)
{
  TEST(test_empty_construct_ell<T>);
  TEST(test_construct_ell<T>);
  TEST(test_destruct_ell<T>);
}

auto main() -> int
{
  // Run the unit tests. If a test fails, the program will print failure info
  // and return 1.
  RUN_SUITE(test_constructor_destructor_ell<float>);
  RUN_SUITE(test_constructor_destructor_ell<double>);
  return 0; 
}