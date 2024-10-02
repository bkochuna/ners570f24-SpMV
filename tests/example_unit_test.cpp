#include <SpMV.hpp>

#include <vector> // std::vector

// Testing library required for testing (Always include this last!)
#include "unit_test_framework.hpp"

// Use ASSERT(condition) to test if a condition is true.
// Direct comparison of floating point numbers is not recommended, so we define
// ASSERT_NEAR(a, b, epsilon) to test if a and b are within epsilon of each
// other.

// Create a unit test
TEST_CASE(compareVectors) 
{

  // Initialize variables for testing
  std::vector<int> const a = {1, 2, 3};
  std::vector<int> const b = {1, 2, 3};

  // Test that the elements are equal
  for (size_t i = 0; i < a.size(); ++i) {
    ASSERT(a[i] == b[i]); 
  }
  
  // Repeat for floating point numbers
  std::vector<double> x = {1, 2, 3};
  std::vector<double> y = {1, 2, 3.00001};

  // One should avoid floating point equality comparisons due to rounding errors
  // We will briefly turn off compiler warnings about this to demonstrate why
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-equal"
  //
  // In ℝ (real numbers), 0.1 + 0.2 == 0.3
  // However, in finite precision binary arithmetic, 0.1 + 0.2 != 0.3
  // In binary 0.1 is an irrational number, hence in finite precision it is
  // truncated, incuring a rounding error.
  ASSERT(0.1 + 0.2 != 0.3); // Will evaluate to true if 0.1 + 0.2 != 0.3
#pragma GCC diagnostic pop

  // Instead, we can test if the numbers are within a small epsilon of each
  // other
  for (size_t i = 0; i < x.size(); ++i) {
    ASSERT_NEAR(x[i], y[i], 1e-3); 
  }
} // compareVectors

// Create a test suite
TEST_SUITE(my_suite) 
{
  // Run the unit test when the suite is run
  TEST(compareVectors);
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
  RUN_SUITE(my_suite);
  RUN_SUITE(add_sub_suite<int>);
  RUN_SUITE(add_sub_suite<size_t>);
  return 0; 
}
