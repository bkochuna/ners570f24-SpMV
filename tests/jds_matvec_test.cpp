// File: jds_matvec_test.cpp
// Created: Oct 22,2024
//
// Description: Unit Test for JDS mat vec multiplication
//
// Modifications:
// - Oct 22, 2024:  Created by Xianyu Liu. Added 4 tests and test suite.
//                  1. Identity matrix matvec test
//                  2. Non-diagonal
//                  3. b=0
//                  4. VERY LARGE SIZE
//

#include <SpMV.hpp>
#include "../include/SpMV.hpp"
#include "../include/SparseMatrix_JDS.hpp"
#include <vector> // std::vectors
#include "unit_test_framework.hpp"

const double epsilon = 1e-6;

// 1. Test idendity matrix

TEST_CASE(testJDSMatVecIdentity) {
    // Create a 3x3 identity matrix
    SpMV::SparseMatrix_JDS<double> A(3, 3);
    A.setCoefficient(0, 0, 1.0);
    A.setCoefficient(1, 1, 1.0);
    A.setCoefficient(2, 2, 1.0);
    A.assembleStorage();

    // Define x get b and do b=Ax
    std::vector<double> x = {1.0, 2.0, 3.0};
    std::vector<double> b(3, 0.0);
    A.matvec(x.data(), b.data());

    // Compare
    ASSERT_NEAR(b[0], 1.0, epsilon);
    ASSERT_NEAR(b[1], 2.0, epsilon);
    ASSERT_NEAR(b[2], 3.0, epsilon);
}

// 2. Test 4x4 non-diagonal matrix

TEST_CASE(testJDSMatVecRandom) {
    // Create a 4x4 random sparse matrix
    SpMV::SparseMatrix_JDS<double> A(4, 4);
    A.setCoefficient(0, 0, 2.0);
    A.setCoefficient(1, 1, 3.0);
    A.setCoefficient(2, 2, 4.0);
    A.setCoefficient(3, 3, 5.0);
    A.setCoefficient(1, 2, 1.0);
    A.assembleStorage();

    // Define x get b and do b=Ax
    std::vector<double> x = {1.0, 2.0, 3.0, 4.0};
    std::vector<double> b(4, 0.0);
    A.matvec(x.data(), b.data());

    // Expected b
    std::vector<double> expected_b = {2.0, 9.0, 12.0, 20.0};

    // Compare
    for (size_t i = 0; i < 4; ++i) {
        ASSERT_NEAR(b[i], expected_b[i], epsilon);
    }
}

// 3. What if input is ZERO

TEST_CASE(testJDSMatVecWithZeros) {
    // Create a 3x3 matrix with some zero coefficients
    SpMV::SparseMatrix_JDS<double> A(3, 3);
    A.setCoefficient(0, 0, 2.0);
    A.setCoefficient(1, 1, 0.0); // Zero value
    A.setCoefficient(2, 2, 3.0);
    A.assembleStorage();

    // Define x get b and do b=Ax
    std::vector<double> x = {0.0, 2.0, 1.0};
    std::vector<double> b(3, 0.0);
    A.matvec(x.data(), b.data());

    // Expected b
    std::vector<double> expected_b = {0.0, 0.0, 3.0};

    // Compare
    for (size_t i = 0; i < 3; ++i) {
        ASSERT_NEAR(b[i], expected_b[i], epsilon);
    }
}

// 4. VERY LARGE! sparse matrix

TEST_CASE(testJDSMatVecLargeMatrix) {
    // Create a large sparse matrix (100000x100000)
    const size_t size = 100000;
    SpMV::SparseMatrix_JDS<double> A(size, size);
    for (size_t i = 0; i < size; ++i) {
        A.setCoefficient(i, i, 1.0); // Diagonal matrix
    }
    A.assembleStorage();

    // Define x get b and do b=Ax
    std::vector<double> x(size, 2.0);
    std::vector<double> b(size, 0.0);
    A.matvec(x.data(), b.data());

    // Expected output is also a vector of ones
    for (size_t i = 0; i < size; ++i) {
        ASSERT_NEAR(b[i], 2.0, epsilon);
    }
}


TEST_SUITE(JDS_MatVec) {
    TEST(testJDSMatVecIdentity);
    TEST(testJDSMatVecRandom);
    TEST(testJDSMatVecWithZeros);
    TEST(testJDSMatVecLargeMatrix);
}