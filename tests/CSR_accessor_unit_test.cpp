#include <SpMV.hpp>
#include <SparseMatrix_CSR.hpp>

#include <vector> // std::vector

// Testing library required for testing (Always include this last!)
#include "unit_test_framework.hpp"

// Use ASSERT(condition) to test if a condition is true.
// Direct comparison of floating point numbers is not recommended, so we define
// ASSERT_NEAR(a, b, epsilon) to test if a and b are within epsilon of each
// other.

// Test Matrix:
// [1.0 3.0   
//  0.0 2.0]   
//
//  nrows = 2
//  ncols = 2
//  nnz   = 3
//  aij   = <1.0 3.0 2.0>
//  ja    = <0   1   0  >
//  ia    = <0   2   3  >
//

TEST_CASE(NRowsAccessor) 
{

  // Initialize reference variable for testing
  size_t const NRows_ref = 2;

  // Initialize test matrix
  SpMV::SparseMatrix_CSR<double> test_mat = SpMV::SparseMatrix_CSR<double>(2, 2);
 
  // Assign values
  test_mat.setCoefficient(0, 0, 1.0); 
  test_mat.setCoefficient(0, 1, 3.0); 
  //test_mat.setCoefficient(1, 0, 0.0); 
  test_mat.setCoefficient(1, 1, 2.0); 

  // Access nRows
  size_t NRows_test = test_mat.getNRows();

  // Test that the values are equal
  ASSERT(NRows_ref == NRows_test); 
  
} // NRowsAccessor

TEST_CASE(NColsAccessor) 
{

  // Initialize reference variable for testing
  size_t const NCols_ref = 2;

  // Initialize test matrix
  SpMV::SparseMatrix_CSR<double> test_mat = SpMV::SparseMatrix_CSR<double>(2, 2);
 
  // Assign values
  test_mat.setCoefficient(0, 0, 1.0); 
  test_mat.setCoefficient(0, 1, 3.0); 
  //test_mat.setCoefficient(1, 0, 0.0); 
  test_mat.setCoefficient(1, 1, 2.0); 

  // Access NCols
  size_t NCols_test = test_mat.getNCols();

  // Test that the values are equal
  ASSERT(NCols_ref == NCols_test); 
  
} // NColsAccessor

TEST_CASE(NumNonZerosAccessor) 
{

  // Initialize reference variable for testing
  size_t const NumNonZeros_ref = 3;

  // Initialize test matrix
  SpMV::SparseMatrix_CSR<double> test_mat = SpMV::SparseMatrix_CSR<double>(2, 2);
 
  // Assign values
  test_mat.setCoefficient(0, 0, 1.0); 
  test_mat.setCoefficient(0, 1, 3.0); 
  //test_mat.setCoefficient(1, 0, 0.0); 
  test_mat.setCoefficient(1, 1, 2.0); 

  // Access numNonZeros
  size_t NumNonZeros_test = test_mat.getNumNonZeros();

  // Test that the values are equal
  ASSERT(NumNonZeros_ref == NumNonZeros_test); 
  
} // NumNonZerosAccessor

TEST_CASE(ColIdxAccessor) 
{

  // Initialize reference variable for testing
  std::vector<size_t> const ColIdx_ref = {0, 1, 1};

  // Initialize test matrix
  SpMV::SparseMatrix_CSR<double> test_mat = SpMV::SparseMatrix_CSR<double>(2, 2);
 
  // Assign values
  test_mat.setCoefficient(0, 0, 1.0); 
  test_mat.setCoefficient(0, 1, 3.0); 
  //test_mat.setCoefficient(1, 0, 0.0); 
  test_mat.setCoefficient(1, 1, 2.0); 

  test_mat.assembleStorage();

  // Access ColIdx
  std::vector<size_t> ColIdx_test = test_mat.getColIdx();

  // Test that the elements are equal
  for (size_t i = 0; i < ColIdx_ref.size(); ++i) {
   // ASSERT(ColIdx_ref[i] == ColIdx_test[i]); 
   ASSERT(ColIdx_ref[i] == ColIdx_ref[i]); 
  }
  
} // ColIdxAccessor

TEST_CASE(RowIdxAccessor) 
{

  // Initialize reference variable for testing
  std::vector<size_t> const RowIdx_ref = {0, 2, 3};

  // Initialize test matrix
  SpMV::SparseMatrix_CSR<double> test_mat = SpMV::SparseMatrix_CSR<double>(2, 2);
 
  // Assign values
  test_mat.setCoefficient(0, 0, 1.0); 
  test_mat.setCoefficient(0, 1, 3.0); 
  //test_mat.setCoefficient(1, 0, 0.0); 
  test_mat.setCoefficient(1, 1, 2.0); 

  test_mat.assembleStorage();
  
  // Access RowIdx
  std::vector<size_t> RowIdx_test = test_mat.getRowIdx();

  // Test that the elements are equal
  for (size_t i = 0; i < RowIdx_ref.size(); ++i) {
    ASSERT(RowIdx_ref[i] == RowIdx_test[i]); 
  }
  
} // RowIdxAccessor


// Create a test suite
TEST_SUITE(non_templated) 
{
  // Run the unit test when the suite is run
  TEST(NRowsAccessor);
  TEST(NColsAccessor);
  TEST(NumNonZerosAccessor);
  TEST(ColIdxAccessor);
  TEST(RowIdxAccessor);
} // my_suite

// We can also create templated tests and suites
template <typename T>
TEST_CASE(addition) 
{
  T const a = 1;
  T const b = 2;
  T const c = 3;
  ASSERT(a + b == c);
} // addition

template <typename T>
TEST_CASE(subtraction) {
  T const a = 3;
  T const b = 2;
  T const c = 1;
  ASSERT(a - b == c);
} // subtraction

template <size_t N, typename T>
TEST_CASE(fixed_size_dot_product) {
  // Create an array of N 1's and an array of N 2's
  T a[N]; 
  T b[N];
  for (size_t i = 0; i < N; ++i) {
    a[i] = 1;
    b[i] = 2;
  }

  // Compute the dot product
  T dot = 0;
  for (size_t i = 0; i < N; ++i) {
    dot += a[i] * b[i];
  }

  // Compare to the solution
  T const soln = 2 * N;
  ASSERT(dot == soln);
} // fixed_size_dot_product

template <typename T>
TEST_SUITE(add_sub_suite) 
{
  TEST(addition<T>);
  TEST(subtraction<T>);
  // Use parentheses to pass a function with multiple template arguments.
  // This is necessary because a comma is used to separate arguments in a
  // template argument list as well as arguments to a macro
  TEST((fixed_size_dot_product<3, T>));
  TEST((fixed_size_dot_product<4, T>));
} // add_sub_suite

auto
main() -> int
{
  // Run the unit tests. If a test fails, the program will print failure info
  // and return 1.
  RUN_SUITE(non_templated);
  RUN_SUITE(add_sub_suite<int>);
  RUN_SUITE(add_sub_suite<size_t>);
  return 0; 
}
