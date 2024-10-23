#include <SpMV.hpp> // Include the main library header
#include <vector> // For std::vector
#include <sstream> // For capturing std::cout output

// Include the testing framework
#include "unit_test_framework.hpp"

// Helper function to set up ELL matrix and verify view method output
template <typename fp_type>
void setupAndView(SpMV::SparseMatrix_ELL<fp_type>& ell_matrix, const std::string& expected_output) 
{
    std::ostringstream oss;
    auto cout_buff = std::cout.rdbuf(); // Backup original buffer
    std::cout.rdbuf(oss.rdbuf()); // Redirect std::cout to oss

    ell_matrix.view(); // Call the view method to produce output

    std::cout.rdbuf(cout_buff); // Restore original buffer

    // Check if the captured output matches the expected output
    ASSERT(oss.str() == expected_output);
}

// Test case for ELL view method
TEST_CASE(ELLViewTest) 
{
    // Test 1: Basic functionality with a simple, small matrix (3x3)
    {
        // Dense representation:
        // [1 2 0]
        // [0 3 0]
        // [0 0 4]
        
        // ELL representation with max non-zero elements per row (max_nnz_per_row) is 2:
        // Col Indices: [0, 1], Values: [1.0, 2.0]
        // Col Indices: [1, -1], Values: [3.0, 0.0]
        // Col Indices: [2, -1], Values: [4.0, 0.0]
        SpMV::SparseMatrix_ELL<float> ell_matrix(3, 3);
        ell_matrix.setCoefficient(0, 0, 1.0f);
        ell_matrix.setCoefficient(0, 1, 2.0f);
        ell_matrix.setCoefficient(1, 1, 3.0f);
        ell_matrix.setCoefficient(2, 2, 4.0f);
        ell_matrix.assembleStorage();

        std::string expected_output = 
            "Col Indices: [0, 1], Values: [1.0, 2.0]\n"
            "Col Indices: [1, -1], Values: [3.0, 0.0]\n"
            "Col Indices: [2, -1], Values: [4.0, 0.0]\n";
        setupAndView(ell_matrix, expected_output);
    }

    // Test 2: Empty matrix
    {
        // Dense representation:
        // []
        
        // ELL representation:
        // Matrix is empty
        SpMV::SparseMatrix_ELL<float> ell_matrix(0, 0);
        std::string expected_output = "Matrix is empty\n";
        setupAndView(ell_matrix, expected_output);
    }

    // Test 3: Single element matrix
    {
        // Dense representation:
        // [1]
        
        // ELL representation:
        // Col Indices: [0], Values: [1.0]
        SpMV::SparseMatrix_ELL<float> ell_matrix(1, 1);
        ell_matrix.setCoefficient(0, 0, 1.0f);
        ell_matrix.assembleStorage();

        std::string expected_output = "Col Indices: [0], Values: [1.0]\n";
        setupAndView(ell_matrix, expected_output);
    }

    // Test 4: Symmetric matrix (4x4)
    {
        // Dense representation:
        // [1   0.5 0   0]
        // [0.5 2   1.5 0]
        // [0   1.5 3   0]
        // [0   0   0   4]
        
        // ELL representation with max non-zero elements per row (max_nnz_per_row) is 3:
        // Col Indices: [0, 1, -1], Values: [1.0, 0.5, 0.0]
        // Col Indices: [0, 1, 2], Values: [0.5, 2.0, 1.5]
        // Col Indices: [1, 2, -1], Values: [1.5, 3.0, 0.0]
        // Col Indices: [3, -1, -1], Values: [4.0, 0.0, 0.0]
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

        std::string expected_output = 
            "Col Indices: [0, 1, -1], Values: [1.0, 0.5, 0.0]\n"
            "Col Indices: [0, 1, 2], Values: [0.5, 2.0, 1.5]\n"
            "Col Indices: [1, 2, -1], Values: [1.5, 3.0, 0.0]\n"
            "Col Indices: [3, -1, -1], Values: [4.0, 0.0, 0.0]\n";
        setupAndView(ell_matrix, expected_output);
    }

    // Test 5: Sparse matrix (5x5) with more elements
    {
        // Dense representation:
        // [1 0 0 0 4]
        // [0 0 0 0 0]
        // [0 0 0 0 0]
        // [0 0 0 0 0]
        // [0 0 0 0 2]
        
        // ELL representation with max non-zero elements per row (max_nnz_per_row) is 2:
        // Col Indices: [0, 4], Values: [1.0, 4.0]
        // Col Indices: [-1, -1], Values: [0.0, 0.0]
        // Col Indices: [-1, -1], Values: [0.0, 0.0]
        // Col Indices: [-1, -1], Values: [0.0, 0.0]
        // Col Indices: [4, -1], Values: [2.0, 0.0]
        SpMV::SparseMatrix_ELL<float> ell_matrix(5, 5);
        ell_matrix.setCoefficient(0, 0, 1.0f);
        ell_matrix.setCoefficient(0, 4, 4.0f);
        ell_matrix.setCoefficient(4, 4, 2.0f);
        ell_matrix.assembleStorage();

        std::string expected_output = 
            "Col Indices: [0, 4], Values: [1.0, 4.0]\n"
            "Col Indices: [-1, -1], Values: [0.0, 0.0]\n"
            "Col Indices: [-1, -1], Values: [0.0, 0.0]\n"
            "Col Indices: [-1, -1], Values: [0.0, 0.0]\n"
            "Col Indices: [4, -1], Values: [2.0, 0.0]\n";
        setupAndView(ell_matrix, expected_output);
    }

    // Test 6: Row of all zeros (expect proper padding handling)
    {
        // Dense representation:
        // [1 0 0 0]
        // [0 0 0 0]
        // [0 0 2 0]
        // [0 0 0 3]
        
        // ELL representation with max non-zero elements per row (max_nnz_per_row) is 1:
        // Col Indices: [0], Values: [1.0]
        // Col Indices: [-1], Values: [0.0]
        // Col Indices: [2], Values: [2.0]
        // Col Indices: [3], Values: [3.0]
        SpMV::SparseMatrix_ELL<float> ell_matrix(4, 4);
        ell_matrix.setCoefficient(0, 0, 1.0f);
        ell_matrix.setCoefficient(2, 2, 2.0f);
        ell_matrix.setCoefficient(3, 3, 3.0f);
        ell_matrix.assembleStorage();

        std::string expected_output = 
            "Col Indices: [0], Values: [1.0]\n"
            "Col Indices: [-1], Values: [0.0]\n"
            "Col Indices: [2], Values: [2.0]\n"
            "Col Indices: [3], Values: [3.0]\n";
        setupAndView(ell_matrix, expected_output);
    }

    // Test 7: Large values
    {
        // Dense representation:
        // [1e30 0 0 0]
        // [0 1e-30 0 0]
        // [0 0 -1e30 0]
        // [0 0 0 1e30]
        
        // ELL representation with max non-zero elements per row (max_nnz_per_row) is 1:
        // Col Indices: [0], Values: [1e30]
        // Col Indices: [1], Values: [1e-30]
        // Col Indices: [2], Values: [-1e30]
        // Col Indices: [3], Values: [1e30]
        SpMV::SparseMatrix_ELL<float> ell_matrix(4, 4);
        ell_matrix.setCoefficient(0, 0, 1e30f);
        ell_matrix.setCoefficient(1, 1, 1e-30f);
        ell_matrix.setCoefficient(2, 2, -1e30f);
        ell_matrix.setCoefficient(3, 3, 1e30f);
        ell_matrix.assembleStorage();

        std::string expected_output = 
            "Col Indices: [0], Values: [1e30]\n"
            "Col Indices: [1], Values: [1e-30]\n"
            "Col Indices: [2], Values: [-1e30]\n"
            "Col Indices: [3], Values: [1e30]\n";
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