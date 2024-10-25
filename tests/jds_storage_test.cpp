#include "../include/SpMV.hpp"
#include <iostream>
#include <vector> // std::vector

// Testing library required for testing (Always include this last!)
#include "unit_test_framework.hpp"
#include "../include/SparseMatrix_JDS.hpp"

// Use ASSERT(condition) to test if a condition is true.
// Direct comparison of floating point numbers is not recommended, so we define
// ASSERT_NEAR(a, b, epsilon) to test if a and b are within epsilon of each
// other.

// Create a unit test
// start with templating
const float eps = .0000001
template <typename fp_type>
TEST_CASE(testJDSAssembleStorage) 
{
  // test the getters
  // turn off warnings
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wfloat-equal"
  #pragma GCC diagnostic pop
  // call the class in the header file, SparseMatrix_JDS
  SparseMatrix_JDS = jds
  // Initialize variables for testing
  // permutation index (row), column index, value in the index
  std::vector<size_t> const perm = {0, 1, 2};
  std::vector<size_t> const colIdx = {1, 2, 1, 2, 1};
  std::vector<fp_type> const values = {2., 4., 6., 8., 10.};
  
  // call assemble storage
  jsd.assembleStorage(perm,colIdx,values);
  // perform unit tests
  // build testing arrays. should be the values from the assembled jds class
  std::vector<size_t> const perm_test = {0, 2, 3};
  std::vector<size_t> const colIdx_test = {0, 1, 1, 2, 2};
  std::vector<fp_type> const values_test = {2., 4., 8., 6., 10.};

  ASSERT_NEAR(jds.getPerm(), perm_test, eps);
  ASSERT_NEAR(jds.getColIdx(), colIdx_test, eps);
  ASSERT_NEAR(jds.getValues(), values_test, eps);

} // testJDSAssembleStorage


template <typename fp_type>
TEST_CASE(testJDSAssembleStorageSym) 
{
  // test case for symmetric matrices
  // test the getters
  // turn off warnings
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wfloat-equal"
  #pragma GCC diagnostic pop
  // call the class in the header file, SparseMatrix_JDS
  SparseMatrix_JDS = jds;
  // Initialize variables for testing. MUST USE SIZE_T'S
  // permutation index (row), column index, value in the index
  std::vector<size_t> const perm = {0, 1, 2};
  std::vector<size_t> const colIdx = {0, 0, 1, 1, 2, 2};
  std::vector<fp_type> const values = {2., 4., 6., 8., 10., 12.};
  
  // call assemble storage
  jsd.assembleStorage(perm,colIdx,values);
  // perform unit tests
  // build testing arrays. should be the values from the assembled jds class
  std::vector<size_t> const perm_test = {0, 2, 4};
  std::vector<size_t> const colIdx_test = {0, 0, 1, 1, 2, 2};
  std::vector<fp_type> const values_test = {2., 4., 6., 8., 10., 12.};

  ASSERT_NEAR(jds.getPerm(), perm_test, eps);
  ASSERT_NEAR(jds.getColIdx(), colIdx_test, eps);
  ASSERT_NEAR(jds.getValues(), values_test, eps);
}//testJDSAssembleStorageSym

template <typename fp_type>
TEST_CASE(testJDSAssembleStorageEmptyRow) 
{
  // test case for symmetric matrices
  // test the getters
  // turn off warnings
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wfloat-equal"
  #pragma GCC diagnostic pop
  // call the class in the header file, SparseMatrix_JDS
  SparseMatrix_JDS = jds;
  // Initialize variables for testing. MUST USE SIZE_T'S
  // permutation index (row), column index, value in the index
  std::vector<size_t> const perm = {0, 1, 2};
  std::vector<size_t> const colIdx = {0, 0, 1};
  std::vector<fp_type> const values = {2., 4., 6.};
  
  // call assemble storage
  jsd.assembleStorage(perm,colIdx,values);
  // perform unit tests
  // build testing arrays. should be the values from the assembled jds class
  std::vector<size_t> const perm_test = {0, 2, 4};
  std::vector<size_t> const colIdx_test = {0, 0, 1};
  std::vector<fp_type> const values_test = {2., 4., 6.};

  ASSERT_NEAR(jds.getPerm(), perm_test, eps);
  ASSERT_NEAR(jds.getColIdx(), colIdx_test, eps);
  ASSERT_NEAR(jds.getValues(), values_test, eps);
} //testJDSAssembleStorageEmptyRow

template <typename fp_type>
TEST_CASE(testJDSdisassembleStorage) 
{
  // test the getters
  // turn off warnings
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wfloat-equal"
  #pragma GCC diagnostic pop
  // call the class in the header file, SparseMatrix_JDS
  SparseMatrix_JDS = jds;
  // Initialize variables for testing
  // permutation index (row), column index, value in the index
  std::vector<size_t> const perm = {0, 1, 2};
  std::vector<size_t> const colIdx = {1, 2, 1, 2, 1};
  std::vector<fp_type> const values = {2., 4., 6., 8., 10.};
  
  // call assemble storage
  jsd.assembleStorage(perm,colIdx,values);
  // perform unit tests
  // build testing arrays. should be the values from the assembled jds class
  std::vector<size_t> const perm_test = {0, 2, 3};
  std::vector<size_t> const colIdx_test = {0, 1, 1, 2, 2};
  std::vector<fp_type> const values_test = {2., 4., 8., 6., 10.};

  ASSERT_NEAR(jds.getPerm(), perm_test, eps);
  ASSERT_NEAR(jds.getColIdx(), colIdx_test, eps);
  ASSERT_NEAR(jds.getValues(), values_test, eps);

  // now check disassemble storage
  auto [perm_dis, colIdx_dis, values_dis] = jds.disassembleStorage();
  ASSERT_NEAR(jds.getPerm(), perm_test, eps);
  ASSERT_NEAR(jds.getColIdx(), colIdx_test, eps);
  ASSERT_NEAR(jds.getValues(), values_test, eps);

} // testJDSDisassembleStorage


// Create a test suite
TEST_SUITE(JDS_storage) 
{
  // Run the unit test when the suite is run
  TEST(testJDSAssembleStorage);
  TEST(testJDSAssembleStorageSym);
  TEST(testJDSAssembleStorageEmptyRow);
  TEST(testJDSDisassembleStorage);
} // my_suite

auto
main() -> int
{
  // Run the unit tests. If a test fails, the program will print failure info
  // and return 1.
  // run for floats and doubles
  RUN_SUITE(JDS_storage<float>);
  RUN_SUITE(JDS_storage<double>);
  return 0; 
}
