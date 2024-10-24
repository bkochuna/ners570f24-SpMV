#include <SpMV.hpp>

#include <vector>
#include <iostream>
#include <cmath>
// Testing library required for testing
#include "unit_test_framework.hpp"

// quote from MoonBH-UoM
template <typename T>
TEST_CASE(matrix_state_test)
{
    // Define COO Matrix
    SpMV::SparseMatrix_COO ptr_A(1, 1);
    ptr_A.setCoefficient(0,0,static_cast<T>(1));
    // assert the state of the matrix after setCoefficient/prior to assembleStorage()
    ASSERT(ptr_A.getState() == SpMV::building);
    // pass coefficients to assembleStorage()
    ptr_A.assembleStorage();
    // assert the state of the matrix after assembleStorage()
    ASSERT(ptr_A.getState() == SpMV::assembled);
    // call disassembleStorage() and assert the matrix state
    ptr_A.disassembleStorage();
    ASSERT(ptr_A.getState() == SpMV::building);
}

// test if assembleStorage() returns _I, _J, _val correctly for a nxn matrix (n can be divided by ten)
template <size_t n, typename T>
TEST_CASE(random_nxn) 
{
    ASSERT(n>0);

    // Define COO Matrix and attributes
    SpMV::SparseMatrix<T>* ptr_A = new SpMV::SparseMatrix_COO<T>(n, n);
    size_t nnz = (n/10)*(n/10);
    size_t *rowIdx = new size_t[nnz];
    size_t *colIdx = new size_t[nnz];
    fp_type *val = new fp_type[nnz];

    // Set values of attribute and set the coefficients for ptr_A
    for (size_t i = 0; i < n/10; ++i) {
        for (size_t j = 0; j < n/10; ++j) {
	    index = i*n/10+j;
	    rowIdx[index] = i*10;
    	    colIdx[index] = j*5+i*5;
            value[index] = n*std::sin(static_cast<T>(colIdx[index]));
	    ptr_A.setCoefficient(rowIdx[index],colIdx[index],value[index]);
	}
    }

    // Set coefficients
    ptr_A.assembleStorage();
    size_t* I = ptr_A.getRowIndices();
    size_t* J = ptr_A.getColIndices();
    fp_type* val = ptr_A.getValues();

    // Check if the attributes are the same as expected
    for (size_t i = 0; i < nnz; ++i) {
        ASSERT(I[i] == rowIdx[i]);
	ASSERT(J[i] == colIdx[i]);
        ASSERT_NEAR(val[i], value[i], static_cast<T>(1e-16));
    }

    ptr_A.disassembleStorage();

}

template <typename T>
TEST_SUITE(COO_compare)
{
    TEST((randon_nxn<10,T>));
    TEST((randon_nxn<100,T>));
    TEST((randon_nxn<1000,T>));
    TEST((randon_nxn<10000,T>));

}

auto
main() -> int
{
    RUN_SUITE(COO_compare<double>);
    RUN_SUITE(COO_compare<float>);
  return 0; 
}
