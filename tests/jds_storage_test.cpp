#include "../include/SpMV.hpp"
#include <iostream>
#include <vector> // std::vector

// Testing library required for testing (Always include this last!)
#include "unit_test_framework.hpp"
#include "../include/SparseMatrix_JDS.hpp"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-equal"
#pragma GCC diagnostic pop
const float eps = .0000001; //tolerance

// The test for assemble/disassemble storage will be simple. I'm writing
// 2 tests, isAssembled, and isDisassembled. It will ensure that matrix is receiving
// values, that the matrix is built, and the matrix storage is destroyed.
// since the matrix input format isn't symmetric, I'm not writing a test for
// an input matrix in symmetric format.

template <typename fp_type>
TEST_CASE(jdsIsAssembled)
{
  // init 6x6 test matrix
  SpMV::SparseMatrix_JDS<fp_type> jds(6, 6);

  // add elements to SpMV::_buildCoeff in COO format
  // Define the matrix values with specified non-zero counts per row
  jds._buildCoeff[{0, 0}] = 1.0; // r0, zero indexed
  jds._buildCoeff[{0, 1}] = 2.0;
  jds._buildCoeff[{0, 2}] = 3.0;
  jds._buildCoeff[{1, 1}] = 4.0; // r1
  jds._buildCoeff[{1, 3}] = 5.0;
  jds._buildCoeff[{2, 0}] = 6.0; // r2
  jds._buildCoeff[{2, 1}] = 7.0;
  jds._buildCoeff[{2, 2}] = 8.0;
  jds._buildCoeff[{2, 3}] = 9.0;
  jds._buildCoeff[{2, 4}] = 10.0;
  // row 3: zero elements. make sure that the assembleStorage() can handle rows
  // of all zeros
  jds._buildCoeff[{4, 0}] = 11.0; // r4
  jds._buildCoeff[{5, 2}] = 12.0; // r5

  // Call the assembleStorage()
  jds.assembleStorage();

  // THIS IS WHAT THE MATRIX SHOULD BE
  // i calculated this by hand
  std::vector<size_t> perm_ref = {2, 0, 1, 5, 4, 3};
  std::vector<size_t> colIdx_ref = {0, 1, 2, 3, 4, 0, 1, 2, 1, 3, 0, 2};
  std::vector<fp_type> values_ref = {6., 7., 8., 9., 10., 1., 2., 3., 4., 5., 11., 12.};

  // Call the assembleStorage()
  jds.assembleStorage();

  // test assembled
  ASSERT(jds.getState() == assembled);

  // Compare perm, colIdx, and values vectors against expected reference values
  ASSERT(perm_test.size() == perm_ref.size());

  for (size_t i = 0; i < perm_ref.size(); ++i)
    ASSERT_NEAR(perm_test[i], perm_ref[i], eps);

  ASSERT(colIdx_test.size() == colIdx_ref.size());
  for (size_t i = 0; i < colIdx_ref.size(); ++i){
      ASSERT_NEAR(colIdx_test[i], colIdx_ref[i], eps);
      ASSERT_NEAR(values_test[i], values_ref[i], eps);
  }

} // jdsIsAssembled

template <typename fp_type>
TEST_CASE(jdsIsDisassembled)
{
  // init 3x3 test matrix
  SpMV::SparseMatrix<fp_type> jds(3, 3);
  // call the assembleStorage function, make sure it's undefined by the end
  jds.disassembleStorage();
  ASSERT(jds.getState() == undefined);

}//jdsIsDisassembled

// Create a test suite
template <typename fp_type>
TEST_SUITE(jdsStorage)
{
  // Run the unit test when the suite is run
  TEST(jdsIsAssembled<fp_type>);
  TEST(jdsIsDisassembled<fp_type>);
} // my_suite

auto
main() -> int
{
  // Run the unit tests. If a test fails, the program will print failure info
  // and return 1.
  // run for floats and doubles
  RUN_SUITE(jdsStorage<float>);
  RUN_SUITE(jdsStorage<double>);
  return 0;
}
