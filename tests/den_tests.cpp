#include <SpMV.hpp>
#include <SparseMatrix_COO.hpp>

#include <vector> // std::vector

// Testing library required for testing (Always include this last!)
#include "unit_test_framework.hpp"


// Test to ensure view throws error for viewing an initialized array
template <typename T>
TEST_CASE(view_initialized) 
{

} 

// Test suite for view
TEST_SUITE(view_suite) 
{
  // Run the unit test for both floats an doubles
  TEST(view_initialized<float>);
  TEST(view_initialized<double>);
} 

auto
main() -> int
{
  // Run the unit tests. If a test fails, the program will print failure info
  // and return 1.
  RUN_SUITE(view_suite);
  return 0; 
}
