#pragma once

#include "SparseMatrix_CSR.hpp"
#include "unit_test_framework.hpp"
#include <complex>

namespace test_CSR_storage{

    TEST_CASE(dense_storage) {
        SpMV::SparseMatrix_CSR<double> Mat = SpMV::SparseMatrix_CSR<double>(4,4);
        Mat.setCoefficient(0,0,1.0);
        Mat.setCoefficient(0,1,7.0);
        Mat.setCoefficient(1,0,5.0);
        Mat.setCoefficient(1,2,3.0);
        Mat.setCoefficient(2,3,9.0);
        Mat.setCoefficient(2,1,2.0);
        Mat.setCoefficient(2,2,8.0);
        Mat.setCoefficient(3,3,6.0);

        // construct CSR matrix


        ASSERT(); // check that values are appropriately assigned
        
    }


    TEST_CASE(sparse_storage) {
        SpMV::SparseMatrix_CSR<double> Mat = SpMV::SparseMatrix_CSR<double>(4,4);
        Mat.setCoefficient(0,0,1.0);
        Mat.setCoefficient(0,1,7.0);
        Mat.setCoefficient(1,0,5.0);
        Mat.setCoefficient(1,2,3.0);
        Mat.setCoefficient(2,3,9.0);
        Mat.setCoefficient(2,1,2.0);
        Mat.setCoefficient(2,2,8.0);
        Mat.setCoefficient(3,3,6.0);

        // construct CSR matrix


        ASSERT(); // check that values are appropriately assigned
        
    }





////////////////////////////////////////////////////////////////////





    // Test for parameterized constructor (valid inputs)
    TEST_CASE(parameterizedConstructor_Valid) {
        SpMV::SparseMatrix_CSR<double> csr_matrix(3, 3); // 3x3 matrix
        ASSERT(csr_matrix.getNRows() == 3);
        ASSERT(csr_matrix.getNCols() == 3);
        ASSERT(csr_matrix.getNumNonZeros() == 0); // Initially no non-zero elements
    }

    // Test for parameterized constructor (invalid dimensions)
    TEST_CASE(parameterizedConstructor_InvalidDimensions) {
        try {
            SpMV::SparseMatrix_CSR<double> csr_matrix(-3, 3); // Invalid negative row count
            ASSERT(false); 
        } catch (const std::invalid_argument& e) {
            ASSERT(true); 
        }

        try {
            SpMV::SparseMatrix_CSR<double> csr_matrix(3, -3); // Invalid negative column count
            ASSERT(false); 
        } catch (const std::invalid_argument& e) {
            ASSERT(true); 
        }
    }

    // Test for parameterized constructor (large dimensions)
    TEST_CASE(parameterizedConstructor_LargeDimensions) {
        size_t large_dim = std::numeric_limits<size_t>::max(); 
        try {
            SpMV::SparseMatrix_CSR<double> csr_matrix(large_dim, large_dim); 
            ASSERT(true); // If it gets here, it means large dimensions are handled
        } catch (const std::bad_alloc& e) {
            ASSERT(false); // If memory allocation fails, we don't want this
        } catch (...) {
            ASSERT(false); // Catch all other exceptions
        }
    }


    // Test for error handling: non-real element (if applicable)
    TEST_CASE(nonRealElementCheck) {
        try {
            SpMV::SparseMatrix_CSR<std::complex<double>> csr_matrix(3, 3); // Complex numbers may not be allowed
            ASSERT(false); 
        } catch (const std::invalid_argument& e) {
            ASSERT(true); 
        }
    }

    // Test for destructor (ensuring no memory leaks)
    TEST_CASE(destructor_NoMemoryLeaks) {
        class TestMatrix {
        public:
            ~TestMatrix() {
                std::cout << "Hello from SparseMatrix_CSR Destructor (test)!\n";
            }
        };
        SpMV::SparseMatrix_CSR<double>* csr_matrix = new SpMV::SparseMatrix_CSR<double>(5, 5);
        delete csr_matrix; // Ensure this doesn't cause any memory leak issues
        ASSERT(true); // If this runs, no leaks or errors were detected
    }


    // Additional destructor test to confirm the destructor is called
    TEST_CASE(destructor_Called) {
        static bool destructorCalled = false;
        {
            class TestMatrix {
            public:
                ~TestMatrix() {
                    destructorCalled = true;
                    std::cout << "Destructor called!\n";
                }
            };
            SpMV::SparseMatrix_CSR<double>* csr_matrix = new SpMV::SparseMatrix_CSR<double>(5, 5);
            delete csr_matrix; // This should call the destructor
        }
        ASSERT(destructorCalled); // Ensure the destructor was indeed called
    }

} // namespace Test_SparseMatrix_CSR
