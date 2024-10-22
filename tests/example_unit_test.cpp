#include "SpMV.hpp"

#include <vector> // std::vector
#include <float.h> //need for DBL_MAX and DBL_MIN

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

//Unit Tests for the accessor methods
TEST_CASE(ACCESSORS)
{

  //randomly choose number of rows and columns
  // 1000 is upper limit to prevent test from running too long
  // 4 is lower limit to ensure 10% density has at least one element to fill
  srand(static_cast<unsigned int>(time(NULL)));
  size_t R = rand() % 1000 + 4;
  size_t C = rand() % 1000 + 4;

  //initialize the matrix
  SpMV::SparseMatrix_DEN<double> A = SpMV::SparseMatrix_DEN<double>(int(R),int(C));

  //randomly assign the values
  //initialize using COO format
  size_t n_elements = R*C;
  size_t n_nonzero = size_t(double(n_elements) * 0.1);//want 90% of matrix as sparse (sizing arbitrary)
  size_t *R_idx = new size_t[n_nonzero];
  size_t *C_idx = new size_t[n_nonzero];
  double *values = new double[n_nonzero];
  bool has_duplicates; 
  bool is_zero;

  //uniformly sample without replacement
  for (size_t i = 0; i < n_nonzero; ++i){
    values[i] = (rand() / double(RAND_MAX)) * DBL_MAX + DBL_MIN;//choose random value between the double min and max
    R_idx[i] = rand() % R;//choose random value between 0 and R
    C_idx[i] = rand() % C;//choose random value between 0 and C
    has_duplicates = true;//assume we have duplicates
    //check for duplicates
    while (has_duplicates){
      //set the flag to false so we only go through once if needed
      has_duplicates = false; 
      //check each element of the array that has already been set
      for (size_t ii = 0; ii < i; ++ii){
        //if the row is duplicated
        if (R_idx[i] == R_idx[ii] && C_idx[i] == C_idx[ii]){
          //resample C
          C_idx[i] = rand() % C;
          //flag that a duplicate was found and exit to outer loop to check again
          has_duplicates = true;
          break;
        }
      }
    }
  }

  //assign
  //update call after method is implemented
  A.assembleStorage();

  //check the n_rows and n_columns accessors 
  ASSERT(R == A.getNumRows());
  ASSERT(C == A.getNumCols());

  //loop over all elements
  for (size_t i = 0; i < R; ++i){
    for(size_t j = 0; j < C; ++j){
      is_zero = true;
      //check if the value is non zero. If so, compare
      for(size_t ii = 0; ii < n_nonzero; ++ii){
        if (i == R_idx[ii] && j == C_idx[ii]){
          ASSERT_NEAR(values[ii], A.getCoefficient(i, j), 1e-3);
          is_zero = false;
        }
      }
      if (is_zero){ASSERT_NEAR(0.0, A.getCoefficient(i, j), 1e-3);}
    }
  }

  //cleanup 
  delete[] R_idx;
  delete[] C_idx;
  delete[] values;

}

// Create a test suite
TEST_SUITE(my_suite) 
{
  // Run the unit test when the suite is run
  TEST(compareVectors);
  TEST(ACCESSORS);
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
