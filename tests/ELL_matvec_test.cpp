#include <SpMV.hpp>
#include <vector>
#include <iostream>
#include <stdexcept>
#include <SparseMatrix.hpp>
#include "unit_test_framework.hpp" //always include last 

//needs to match correct functions/variables etc.


template <typename T>
TEST_CASE(matVecMultiplyTest) {
    const size_t n = 3;
    SpMV::SparseMatrix_ELL<T> matrix(n, n);

    for (size_t i = 0; i < n; i++){
        for (size_t j = 0; j < n; j++){
           matrix.setCoefficient(i, j, static_cast<T>(i*n+j)); 
        }
    }
    /*
    0 1 2       1
    3 4 5       2
    6 7 8       3
    */
    matrix.assembleStorage();
    std::vector<T> x = {static_cast<T>(1), static_cast<T>(2), static_cast<T>(3)};
    std::vector<T> expected = {0*1+1*2+2*3, 3*1+4*2+5*3, 6*1+7*2+8*3};
    
    std::vector<T> result;
    matrix.matvec(result, x);

    ASSERT(result.size() == expected.size());
    for (size_t i = 0; i < expected.size(); ++i) {
        ASSERT_NEAR(result[i], expected[i], 1e-7);
    }
} // matVecMultiplyTest

template<typename T>
TEST_CASE(matVecMultiplyInvalidSizeTest) {
    const size_t n = 3;
    SpMV::SparseMatrix_ELL<T> matrix(n, n);
    std::vector<T> x = {1.0, 2.0}; // Invalid size
    std::vector<T> result;
    try {
        matrix.matvec(result, x);
        ASSERT(false); // If no exception is thrown, this should fail
    } catch (const std::invalid_argument&) {
        // Expected exception
    }
} // matVecMultiplyInvalidSizeTest

template <typename T>
TEST_CASE(setValueOutOfRangeTest) {
    const size_t n = 3;
    SpMV::SparseMatrix_ELL<T> matrix(n, n);
    
    try {
        matrix.setCoefficient(3, 0, static_cast<T>(1.0));
        ASSERT(false); // Should fail
    } catch (const std::out_of_range&) {
        // Expected exception
    }

    try {
        matrix.setCoefficient(0, 5, static_cast<T>(1.0));
        ASSERT(false); // Should fail
    } catch (const std::out_of_range&) {
        // Expected exception
    }
} // setValueOutOfRangeTest

// Create a test suite
template <typename T>
TEST_SUITE(ELLMatrixTests) {
    TEST(matVecMultiplyTest<T>);
    TEST(matVecMultiplyInvalidSizeTest<T>);
    TEST(setValueOutOfRangeTest<T>);

} // ELLMatrixTests

// Main function to run the tests
auto
main() -> int {
    
    RUN_SUITE(ELLMatrixTests<float>);
    RUN_SUITE(ELLMatrixTests<double>);
    return 0;
}
