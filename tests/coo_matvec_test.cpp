// test_matvec_coo.cpp

#include "../include/SparseMatrix_COO.hpp" // Adjust the path as necessary
#include <vector>
#include "unit_test_framework.hpp" // Include your testing framework
/*
The tests included are:

1.Test for Diagonal Matrix (testMatvecCOO_Diagonal)
    Purpose: Verifies that matvec_COO() correctly computes the matrix-vector product when the matrix is diagonal.
    Setup:
        Creates a diagonal matrix where the diagonal elements are 1.0, 2.0, ..., N.
        Uses a vector x of ones.
    Expected Result:
        The result vector b should contain the diagonal elements, i.e., b[i] = (i + 1).

2.Test for Tridiagonal Matrix (testMatvecCOO_Tridiagonal)
    Purpose: Checks the correctness of matvec_COO() with a tridiagonal matrix.
    Setup:
        Constructs a tridiagonal matrix with 2.0 on the main diagonal and -1.0 on the sub- and super-diagonals.
        Uses a vector x of ones.
    Expected Result:
        Each element b[i] is calculated as 2.0 * x[i] - x[i - 1] - x[i + 1], accounting for boundary conditions.

3. Test for General Sparse Matrix (testMatvecCOO_GeneralSparse)
    Purpose: Ensures matvec_COO() handles a general sparse matrix with arbitrary non-zero elements.
    Setup:
        Sets non-zero elements at specific positions in the matrix to create a sparse pattern.
        Uses a vector x with known values.
    Expected Result:
        Computes the expected result by manually calculating the contributions from the non-zero elements.

4. Tests for Different Matrix Sizes (testMatvecCOO_Size)
    Purpose: Validates matvec_COO() across various matrix sizes to ensure scalability and correctness.
    Setup:
    Templated test cases for sizes N = 1, 3, 5, 10.
    Constructs identity matrices of size N x N.
    Uses a vector x of ones.
    Expected Result:
    The result vector b should be identical to the input vector x, since multiplying by an identity matrix should not change the vector.

5.Test for Empty Matrix (testMatvecCOO_EmptyMatrix)
    Purpose: Tests the function's behavior when the matrix is empty (i.e., has zero size).
    Setup:
        Creates an empty matrix with N = 0.
        Uses an empty vector x.
    Expected Result:
        The result vector b should also be empty, and the function should handle this case without errors.

6. Tests with Different Floating-Point Types
    Purpose: Confirms that matvec_COO() works correctly with both float and double precision types.
    Setup:
        All the above tests are run with fp_type instantiated as both float and double.
    Expected Result:
        The function should produce accurate results within the specified tolerance for both data types.
*/

// Test case for Diagonal Matrix
template <typename fp_type>
TEST_CASE(testMatvecCOO_Diagonal)
{
    const size_t N = 5; // Adjust size as needed
    SpMV::SparseMatrix_COO<fp_type> matrix(N, N);

    // Set up a diagonal matrix with values 1.0, 2.0, ..., N
    for (size_t i = 0; i < N; ++i)
    {
        matrix.setCoefficient(i, i, static_cast<fp_type>(i + 1));
    }

    // Assemble the storage if required
    matrix.assembleStorage();

    // Prepare the input vector
    std::vector<fp_type> x(N, static_cast<fp_type>(1.0)); // Vector of ones

    // Expected result after matvec
    std::vector<fp_type> expected(N);
    for (size_t i = 0; i < N; ++i)
    {
        expected[i] = static_cast<fp_type>(i + 1);
    }

    // Compute the matrix-vector product
    std::vector<fp_type> result(N, static_cast<fp_type>(0.0));
    matrix.matvec_COO(x, result);

    // Check that the result matches the expected result
    for (size_t i = 0; i < N; ++i)
    {
        ASSERT_NEAR(result[i], expected[i], static_cast<fp_type>(1e-6));
    }
}

// Test case for Tridiagonal Matrix
template <typename fp_type>
TEST_CASE(testMatvecCOO_Tridiagonal)
{
    const size_t N = 5; // Adjust size as needed
    SpMV::SparseMatrix_COO<fp_type> matrix(N, N);

    // Set up a tridiagonal matrix
    for (size_t i = 0; i < N; ++i)
    {
        if (i > 0)
            matrix.setCoefficient(i, i - 1, static_cast<fp_type>(-1.0)); // Sub-diagonal
        matrix.setCoefficient(i, i, static_cast<fp_type>(2.0));           // Main diagonal
        if (i < N - 1)
            matrix.setCoefficient(i, i + 1, static_cast<fp_type>(-1.0)); // Super-diagonal
    }

    // Assemble the storage if required
    matrix.assembleStorage();

    // Prepare the input vector
    std::vector<fp_type> x(N, static_cast<fp_type>(1.0)); // Vector of ones

    // Expected result after matvec
    std::vector<fp_type> expected(N, static_cast<fp_type>(0.0));
    for (size_t i = 0; i < N; ++i)
    {
        expected[i] = static_cast<fp_type>(2.0) * x[i];
        if (i > 0)
            expected[i] += static_cast<fp_type>(-1.0) * x[i - 1];
        if (i < N - 1)
            expected[i] += static_cast<fp_type>(-1.0) * x[i + 1];
    }

    // Compute the matrix-vector product
    std::vector<fp_type> result(N, static_cast<fp_type>(0.0));
    matrix.matvec_COO(x, result);

    // Check that the result matches the expected result
    for (size_t i = 0; i < N; ++i)
    {
        ASSERT_NEAR(result[i], expected[i], static_cast<fp_type>(1e-6));
    }
}

// Test case for General Sparse Matrix
template <typename fp_type>
TEST_CASE(testMatvecCOO_GeneralSparse)
{
    const size_t N = 5;
    SpMV::SparseMatrix_COO<fp_type> matrix(N, N);

    // Set up a general sparse matrix
    matrix.setCoefficient(0, 2, static_cast<fp_type>(3.0));
    matrix.setCoefficient(1, 0, static_cast<fp_type>(-1.0));
    matrix.setCoefficient(3, 4, static_cast<fp_type>(4.5));
    matrix.setCoefficient(4, 1, static_cast<fp_type>(2.2));

    // Assemble the storage if required
    matrix.assembleStorage();

    // Prepare the input vector
    std::vector<fp_type> x = {static_cast<fp_type>(1.0), static_cast<fp_type>(2.0),
                              static_cast<fp_type>(3.0), static_cast<fp_type>(4.0),
                              static_cast<fp_type>(5.0)};

    // Expected result after matvec
    std::vector<fp_type> expected(N, static_cast<fp_type>(0.0));
    expected[0] = static_cast<fp_type>(3.0) * x[2];
    expected[1] = static_cast<fp_type>(-1.0) * x[0];
    expected[3] = static_cast<fp_type>(4.5) * x[4];
    expected[4] = static_cast<fp_type>(2.2) * x[1];

    // Compute the matrix-vector product
    std::vector<fp_type> result(N, static_cast<fp_type>(0.0));
    matrix.matvec_COO(x, result);

    // Check that the result matches the expected result
    for (size_t i = 0; i < N; ++i)
    {
        ASSERT_NEAR(result[i], expected[i], static_cast<fp_type>(1e-6));
    }
}

// Test case for Different Sizes
template <typename fp_type, size_t N>
TEST_CASE(testMatvecCOO_Size)
{
    SpMV::SparseMatrix_COO<fp_type> matrix(N, N);

    // Set up an identity matrix
    for (size_t i = 0; i < N; ++i)
    {
        matrix.setCoefficient(i, i, static_cast<fp_type>(1.0));
    }

    // Assemble the storage if required
    matrix.assembleStorage();

    // Prepare the input vector
    std::vector<fp_type> x(N, static_cast<fp_type>(1.0)); // Vector of ones

    // Expected result after matvec
    std::vector<fp_type> expected(N, static_cast<fp_type>(1.0));

    // Compute the matrix-vector product
    std::vector<fp_type> result(N, static_cast<fp_type>(0.0));
    matrix.matvec_COO(x, result);

    // Check that the result matches the expected result
    for (size_t i = 0; i < N; ++i)
    {
        ASSERT_NEAR(result[i], expected[i], static_cast<fp_type>(1e-6));
    }
}

// Test case for Empty Matrix
template <typename fp_type>
TEST_CASE(testMatvecCOO_EmptyMatrix)
{
    const size_t N = 0;
    SpMV::SparseMatrix_COO<fp_type> matrix(N, N);

    // Assemble the storage if required
    matrix.assembleStorage();

    // Prepare the input vector
    std::vector<fp_type> x; // Empty vector

    // Expected result after matvec
    std::vector<fp_type> expected;

    // Compute the matrix-vector product
    std::vector<fp_type> result;
    matrix.matvec_COO(x, result);

    // Check that the result is empty
    ASSERT(result.empty());
}

// Test suite to group matvec_COO tests
TEST_SUITE(matvec_coo_suite)
{
    // Tests for double precision
    TEST((testMatvecCOO_Diagonal<double>));
    TEST((testMatvecCOO_Tridiagonal<double>));
    TEST((testMatvecCOO_GeneralSparse<double>));
    TEST((testMatvecCOO_Size<double, 1>));
    TEST((testMatvecCOO_Size<double, 3>));
    TEST((testMatvecCOO_Size<double, 5>));
    TEST((testMatvecCOO_Size<double, 10>));
    TEST((testMatvecCOO_EmptyMatrix<double>));

    // Tests for float precision
    TEST((testMatvecCOO_Diagonal<float>));
    TEST((testMatvecCOO_Tridiagonal<float>));
    TEST((testMatvecCOO_GeneralSparse<float>));
    TEST((testMatvecCOO_Size<float, 1>));
    TEST((testMatvecCOO_Size<float, 3>));
    TEST((testMatvecCOO_Size<float, 5>));
    TEST((testMatvecCOO_Size<float, 10>));
    TEST((testMatvecCOO_EmptyMatrix<float>));
}

// Main function to run the test suite
int main()
{
    RUN_SUITE(matvec_coo_suite);
    return 0;
}
