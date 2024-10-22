#include <SpMV.hpp> // Include the main library header
#include <vector> // For std::vector
#include <sstream> // For capturing std::cout output

// Include the testing framework
#include "unit_test_framework.hpp"

// Helper function to set up ELL matrix and verify view method
template <typename fp_type>
void setupAndView(SparseMatrix_ELL<fp_type>& ell_matrix, const std::string& expected_output) 
{
    std::ostringstream oss;
    auto cout_buff = std::cout.rdbuf(); // Backup original buffer
    std::cout.rdbuf(oss.rdbuf()); // Redirect std::cout to oss

    ell_matrix.view();

    std::cout.rdbuf(cout_buff); // Restore original buffer

    // Check if the captured output matches the expected output
    ASSERT(oss.str() == expected_output);
}

// Test case for ELL view method
TEST_CASE(ELLViewTest) 
{
    // Test 1: Basic functionality with a simple, small matrix
    {
        // Dense representation:
        // [1 2 0 0]
        // [0 3 0 4]
        // [0 0 4 0]
        // [5 0 0 0]
        SpMV::SparseMatrix_ELL<float> ell_matrix(4, 4);
        ell_matrix.setCoefficient(0, 0, 1.0f);
        ell_matrix.setCoefficient(0, 1, 2.0f);
        ell_matrix.setCoefficient(1, 1, 3.0f);
        ell_matrix.setCoefficient(2, 2, 4.0f);
        ell_matrix.setCoefficient(1, 3, 4.0f);
        ell_matrix.setCoefficient(3, 0, 5.0f);
        ell_matrix.assembleStorage();

        std::string expected_output = "Expected output for basic functionality test";
        setupAndView(ell_matrix, expected_output);
    }

    // Test 2: Empty matrix
    {
        // Dense representation:
        // []
        SpMV::SparseMatrix_ELL<float> ell_matrix(0, 0);
        std::string expected_output = "Expected output for empty matrix";
        setupAndView(ell_matrix, expected_output);
    }

    // Test 3: Single element matrix
    {
        // Dense representation:
        // [1]
        SpMV::SparseMatrix_ELL<float> ell_matrix(1, 1);
        ell_matrix.setCoefficient(0, 0, 1.0f);
        ell_matrix.assembleStorage();

        std::string expected_output = "Expected output for single element matrix";
        setupAndView(ell_matrix, expected_output);
    }

    // Test 4: Symmetric matrix
    {
        // Dense representation:
        // [1   0.5 0   0]
        // [0.5 2   1.5 0]
        // [0   1.5 3   0]
        // [0   0   0   4]
        SpMV::SparseMatrix_ELL<float> ell_matrix(4, 4);
        ell_matrix.setCoefficient(0, 0, 1.0f);
        ell_matrix.setCoefficient(1, 0, 0.5f);
        ell_matrix.setCoefficient(0, 1, 0.5f);
        ell_matrix.setCoefficient(1, 1, 2.0f);
        ell_matrix.setCoefficient(1, 2, 1.5f);
        ell_matrix.setCoefficient(2, 1, 1.5f);
        ell_matrix.setCoefficient(2, 2, 3.0f);
        ell_matrix.setCoefficient(3, 3, 4.0f);
        ell_matrix.assembleStorage();

        std::string expected_output = "Expected output for symmetric matrix";
        setupAndView(ell_matrix, expected_output);
    }

    // Test 5: Sparse matrix
        // Dense representation:
        // [1 0 0 0 4]
        // [0 0 0 0 0]
        // [0 0 0 0 0]
        // [0 0 0 0 2]
        SpMV::SparseMatrix_ELL<float> ell_matrix(5, 5);
        ell_matrix.setCoefficient(0, 0, 1.0f);
        ell_matrix.setCoefficient(0, 4, 4.0f);
        ell_matrix.setCoefficient(4, 4, 2.0f);
        ell_matrix.assembleStorage();

        std::string expected_output = "Expected output for sparse matrix";
        setupAndView(ell_matrix, expected_output);
    }

    // Test 6: Row of all zeros (expect proper padding handling)
    {
        // Dense representation:
        // [1 0 0 0]
        // [0 0 0 0]
        // [0 0 2 0]
        // [0 0 0 3]
        SpMV::SparseMatrix_ELL<float> ell_matrix(4, 4);
        ell_matrix.setCoefficient(0, 0, 1.0f);
        ell_matrix.setCoefficient(2, 2, 2.0f);
        ell_matrix.setCoefficient(3, 3, 3.0f);
        ell_matrix.assembleStorage();

        std::string expected_output = "Expected output for row of all zeros";
        setupAndView(ell_matrix, expected_output);
    }

    // Test 7: Large values
    {
        // Dense representation:
        // [1e30 0 0 0]
        // [0 1e-30 0 0]
        // [0 0 -1e30 0]
        // [0 0 0 1e30]
        SpMV::SparseMatrix_ELL<float> ell_matrix(4, 4);
        ell_matrix.setCoefficient(0, 0, 1e30);
        ell_matrix.setCoefficient(1, 1, 1e-30);
        ell_matrix.setCoefficient(2, 2, -1e30);
        ell_matrix.setCoefficient(3, 3, 1e30);
        ell_matrix.assembleStorage();

        std::string expected_output = "Expected output for large values";
        setupAndView(ell_matrix, expected_output);
    }
}

// Define the test suite that includes the test case
TEST_SUITE(ELLTestSuite) 
{
    TEST(ELLViewTest);
}

// Main function to run the test suite
int main() 
{
    RUN_SUITE(ELLTestSuite);
    return 0;
}