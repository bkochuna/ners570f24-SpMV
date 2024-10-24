#include <SpMV.hpp>
#include <SparseMatrix_COO.hpp>

#include <vector> // std::vector
#include <fstream>
#include <sstream>
#include <string>

// Testing library required for testing (Always include this last!)
#include "unit_test_framework.hpp"

/*
 * Goals of the view tests:
 * 1. Test that the view method will no create and output file if the matrix is empty
 * 2. Test that the view method will correctly report values to output file
 * 3. Test that the view method will concatinate doubles and strings to reasonable sizes for viewing
 * We are not testing for unexpected behavior and making sure it returns an error
 * We are only checking for file outputs because command line parsing is not necessary (according to kyle)
 * Curently assuming view matrix outputs to file `view.out`
 */

// Test to ensure that the view method does not create an output file if the matrix if empty
template <typename T>
TEST_CASE(empty_matrix) 
{

  SpMV::SparseMatrix_DEN<T>* matrix = new SpMV::SparseMatrix_DEN<T>(1, 5);

  matrix.view();

  std::ifstream file("view.out"); 

  ASSERT(file.good() == false);
} 

// Tests that the view method creates a view.out file and output all zeros for a 1 by 1 matrix
template <typename T>
TEST_CASE(zero_matrix)
{

  SpMV::SparseMatrix_DEN<T>* matrix = new SpMV::SparseMatrix_DEN<T>(1, 1);

  size_t row = 0;
  size_t col = 0;
  T val = 0.0;

  matrix.setCoefficient(row, col, val);
  matrix.setCoefficient(row, col, val);

  matrix.assembleStorage();

  matrix.view();

  std::ifstream file("view.out"); 
  ASSERT(file.good() == true);
  std::string str; 
  while (std::getline(file, str))
  {
    if(std::is_same<T, float>::value){
      ASSERT_NEAR(stof(str), val, 0);
    } else if (std::is_same<T, double>::value)
    {
      ASSERT_NEAR(stod(str), val, 0);
    }
  }
}

// Tests that the view method creates a view.out file and concats to reasonable size (4 decimal places)
// should output something similar to:
//  0.333 0.0
//  0.0 0.3333
template <typename T>
TEST_CASE(multi_matrix)
{

  SpMV::SparseMatrix_DEN<T>* matrix = new SpMV::SparseMatrix_DEN<T>(2, 2);

  std::size_t row1 = 0;
  std::size_t col1 = 0;
  T val1 = 1.0 / 3.0;
  std::size_t row2 = 1;
  std::size_t col2 = 1;
  T val2 = 2.0 / 3.0;
  T zero = 0.0;

  matrix.setCoefficient(row1, col1, val1);
  matrix.setCoefficient(row2, col2, val2);

  matrix.assembleStorage();

  matrix.view();

  std::ifstream file("view.out"); 
  ASSERT(file.good() == true);
  std::string str; 
  std::vector<std::string> v;
  while (std::getline(file, str))
  {
    // Putting line into vector
    std::stringstream ss(str);
    std::string s;
    
    while (std::getline(ss, s, ' ')) {
      v.push_back(s);
    }
  }
  // Checking vector for correct values for index: 0,0
  if(std::is_same<T, float>::value){
    ASSERT_NEAR(stof(v[0]), val1, 3);
  } else if (std::is_same<T, double>::value)
  {
    ASSERT_NEAR(stod(v[0]), val1, 3);
  }

  // Checking vector for correct values for index: 0,1
  if(std::is_same<T, float>::value){
    ASSERT_NEAR(stof(v[1]), zero, 0);
  } else if (std::is_same<T, double>::value)
  {
    ASSERT_NEAR(stod(v[1]), zero, 0);
  }

  // Checking vector for correct values for index: 1,0
  if(std::is_same<T, float>::value){
    ASSERT_NEAR(stof(v[2]), zero, 0);
  } else if (std::is_same<T, double>::value)
  {
    ASSERT_NEAR(stod(v[2]), zero, 0);
  }

  // Checking vector for correct values for index: 1,1
  if(std::is_same<T, float>::value){
    ASSERT_NEAR(stof(v[3]), val2, 0.0001);
  } else if (std::is_same<T, double>::value)
  {
    ASSERT_NEAR(stod(v[3]), val2, 0.0001);
  }
}

// Test suite for view
TEST_SUITE(view_suite) 
{
  // Run the unit test for both floats an doubles
  TEST(empty_matrix<float>);
  TEST(empty_matrix<double>);
  TEST(zero_matrix<double>);
  TEST(zero_matrix<double>);
  TEST(multi_matrix<double>);
  TEST(multi_matrix<double>);
} 

auto
main() -> int
{
  // Run the unit tests. If a test fails, the program will print failure info
  // and return 1.
  RUN_SUITE(view_suite);
  return 0; 
}
