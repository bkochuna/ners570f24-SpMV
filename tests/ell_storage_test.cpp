#include <SpMV.hpp>
#ifndef _LIBSPMV_
#  error("unit_test_framework.hpp must be included AFTER SpMV.hpp!")
#endif
#include <SparseMatrix.hpp>
#include <SparseMatrix_ELL.hpp>
#include <vector>  // For std::vector
#include <map>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <cassert>
#include <cmath>
#include "unit_test_framework.hpp"

template <typename T>
TEST_CASE(matrix_state_test)
{
    // Define ELL Matrix
    SpMV::SparseMatrix_ELL<T> ptr_A(1, 1);
    ptr_A.setCoefficient(0,0,static_cast<T>(1));
    // assert the state of the matrix after setCoefficient/prior to assembleStorage()
    ASSERT(ptr_A.getState() == building);
    // pass coefficients to assembleStorage()
    ptr_A.assembleStorage();
    // assert the state of the matrix after assembleStorage()
    ASSERT(ptr_A.getState() == assembled);
    // call disassembleStorage() and assert the matrix state
    ptr_A.disassembleStorage();
    ASSERT(ptr_A.getState() == building);
    delete(ptr_A);

}

// Banded Matrix Test
// unit test to check if assembleStorage() returns ELL arrays for nxn m-diagonal (d-gap between each diagonal) matrix correctly; NOTE: m is odd!
template <size_t n, size_t m, size_t d, typename T>
TEST_CASE(banded_nxn_mdiag_deld)
{
    ASSERT(n>0);
    ASSERT(m>0);
    ASSERT(d>0);
    size_t f = (m-1)/2;
    assert(f*d < n ); // valid values of total number of diagonals
    
    // total number of non-zero elements
    size_t nnz = n;
    for (size_t i=0;i<f;i++)
    {
        nnz += 2*(n - d*(i+1));
    }
    

    // calculate max number of non-zeros in each row
    size_t nnz_rmax = 0;
    size_t nnz_r = 0;
    
    // Define ELL Matrix
    SpMV::SparseMatrix_ELL<T> ptr_A(n, n);
    
    // set the coefficients + find the max. non-zero elements in rows
    int cid = 0;
    size_t cidd = 0;
    for(size_t i=0;i<n;i++)
    {
        nnz_r = 0;
        for(int j=-1*static_cast<int>(f);j<static_cast<int>(f)+1;j++)
        {
            cid = static_cast<int>(i) + j*static_cast<int>(d);
            if (cid >-1 && cid <static_cast<int>(n))
            {
                cidd = static_cast<size_t>(cid);
                ptr_A.setCoefficient(i, cidd, static_cast<T>(1+abs(j)));
                nnz_r +=1;
            } 
        }
        if (nnz_r>nnz_rmax)
        {
            nnz_rmax=nnz_r;
        }
        
    }
    // initialize the required ELL arrays

    std::vector<int> colIdx_req(nnz_rmax*n);
    std::vector<T> val_req(nnz_rmax*n);

    size_t id = 0;
    size_t nnz_c;
    for(size_t i=0;i<n;i++)
    {
        nnz_c = 0;
        for(int j=-1*static_cast<int>(f);j<static_cast<int>(f)+1;j++)
        {
            cid = static_cast<int>(i) + j*static_cast<int>(d);
            if (cid >-1 && cid <static_cast<int>(n))
            {
                colIdx_req[id] = cid;
                val_req[id] = static_cast<T>(1+abs(j));
                id += 1;
                nnz_c +=1;
            } 
        }
        if (nnz_c<nnz_rmax)
        {
            for (size_t j=0;j<(nnz_rmax-nnz_c);j++)
            {
                colIdx_req[id] = -1;
                val_req[id] = 0;
                id += 1;
            }
        }
    }

    
    // pass coefficients to assembleStorage()
    ptr_A.assembleStorage();
    // access the ELL quantities
    std::vector<int>* colIdx_obt = ptr_A.getColIdx(); // it should be int instead of size_t since -1 will also be an element in it
    std::vector<T>* val_obt = ptr_A.getVal();

    // Test that no column ID is below (-1)
    for (size_t i = 0; i < colIdx_obt.size(); ++i) 
    {
        ASSERT((*colIdx_obt)[i] > -2); 
    }
    // Test that no value is given for padded element
    for (size_t i = 0; i < colIdx_obt.size(); ++i) 
    {
        if((*colIdx_obt)[i] == -1)
        {
            ASSERT_NEAR((*val_obt)[i], static_cast<T>(0), static_cast<T>(1e-5));  
        }
    
    }
    // Test that no valid column has zero value
    for (size_t i = 0; i < colIdx_obt.size(); ++i) 
    {
        if((*colIdx_obt)[i] != -1)
        {
            ASSERT((*val_obt)[i]*(*val_obt)[i] > static_cast<T>(0)); 
        }
    
    }
    // Test that _req and _obt arrays have same size
    ASSERT( colIdx_obt.size() == colIdx_req.size());
    ASSERT( val_obt.size() == val_req.size());

    // Test that both colIdx are same element wise
    for (size_t i = 0; i < colIdx_obt.size(); ++i) 
    {
    ASSERT((*colIdx_obt)[i] == colIdx_req[i]); 
    }
   
   // Test that both val are same element wise
    for (size_t i = 0; i < val_obt.size(); ++i) 
    {
    ASSERT_NEAR((*val_obt)[i], val_req[i], static_cast<T>(1e-5)); 
    }

    // call disassembleStorage() and assert the matrix state
    ptr_A.disassembleStorage();

    delete(ptr_A);
}

template <typename T>
TEST_SUITE(ell_vec_compare) 
{
  // MAtrix state scheck
  TEST(matrix_state_test<T>);
  // Medium-sized Identity matrix
  TEST((banded_nxn_mdiag_deld<10,0,0, T>));

  // small-sized Identity matrix
  TEST((banded_nxn_mdiag_deld<1,0,0, T>));

  // large-sized Identity matrix
  TEST((banded_nxn_mdiag_deld<10000,0,0, T>));

  // Dense matrix
  TEST((banded_nxn_mdiag_deld<9,9,1, T>)) 

  // this case has no padding - basic check to see if _buildCoeff maps are being read correctly to give back correct colIdx and values
  TEST((banded_nxn_mdiag_deld<9,5,3, T>)); 

  // this case has padding - checks if the padding is called correctly
  TEST((banded_nxn_mdiag_deld<10,5,3, T>)); // this case has padding
} // add_sub_suite

/////////////////////////////////////////////////////////////////////////
// EDGE CASES
/////////////////////////////////////////////////////////////////////////
// One row is fully 0
// unit test to check if colIdx and val elements corrsponding to the row are padded
template <size_t n, size_t m, size_t d, size_t r, typename T>
TEST_CASE(banded_nxn_r0)
{
    ASSERT(n>0);
    ASSERT(m>0);
    ASSERT(d>0);
    ASSERT(r>0);
    ASSERT(r<n);
    size_t f = (m-1)/2;
    assert(f*d < n ); // valid values of total number of diagonals
    
    // Define ELL Matrix
    SpMV::SparseMatrix_ELL<T> ptr_A(n, n);

    // initialize the keys + find the max. non-zero elements in rows
    int cid = 0;
    size_t cidd = 0;
    for(size_t i=0;i<n;i++)
    {
        if(i!=r){
        for(int j=-1*static_cast<int>(f);j<static_cast<int>(f)+1;j++)
        {
            cid = static_cast<int>(i) + j*static_cast<int>(d);
            if (cid >-1 && cid <static_cast<int>(n))
            {
                cidd = static_cast<size_t>(cid);
                ptr_A.setCoefficient(i, cidd, static_cast<T>(1+abs(j)));
                
            } 
        }}       
    }
    
    // pass coefficients to assembleStorage()
    ptr_A.assembleStorage();
    // access the ELL quantities
    std::vector<int>* colIdx_obt = ptr_A.getColIdx(); // it should be int instead of size_t since -1 will also be an element in it
    std::vector<T>* val_obt = ptr_A.getVal();
    size_t lmax = ptr_A.getLmax();
    
    // Test that elements corresponding to r^th row are padded
    for (size_t i = r*lmax; i < (r+1)*lmax; ++i) 
    {
        ASSERT((*colIdx_obt)[i] == -1);
        ASSERT_NEAR((*val_obt)[i],static_cast<T>(0),static_cast<T>(1e-10));
    }
    
    // call disassembleStorage()
    ptr_A.disassembleStorage();
    delete(ptr_A);
    
}

// 0 matrix, except one element is 1
template <size_t n, typename T>
TEST_CASE(all0_but1)
{
    ASSERT(n>2);
    
    // Define ELL Matrix
    SpMV::SparseMatrix_ELL<T> ptr_A(n, n);

    // initialize the keys
    
    for(size_t i=0;i<n;i++)
    {
        ptr_A.setCoefficient(i, i, static_cast<T>(0));    
    }
    size_t r = n/2;
    size_t c = (n/2)-1;
    ptr_A.setCoefficient(r, c, static_cast<T>(1));

    
    // pass coefficients to assembleStorage()
    ptr_A.assembleStorage();
    // access the ELL quantities
    std::vector<int>* colIdx_obt = ptr_A.getColIdx(); // it should be int instead of size_t since -1 will also be an element in it
    std::vector<T>* val_obt = ptr_A.getVal();
    size_t lmax = ptr_A.getLmax();
    
    // assert that larget non-zero per row is 1
    assert( lmax==1);
    // assert that the array sizes are n
    ASSERT( colIdx_obt.size() == n);
    ASSERT( val_obt.size() == n);

    // Test that elements corresponding to all rows except n/2 are padded
    for (size_t i = 0; i < n; ++i) 
    {
        if (i!= r)
        {
            ASSERT((*colIdx_obt)[i] == -1);
            ASSERT_NEAR((*val_obt)[i],static_cast<T>(0),static_cast<T>(1e-10));
        }
        else
        {
            ASSERT((*colIdx_obt)[i] == static_cast<int>(c));
            ASSERT_NEAR((*val_obt)[i],static_cast<T>(1),static_cast<T>(1e-10));
        }
        
    }
    
    // call disassembleStorage()
    ptr_A.disassembleStorage();
    delete(ptr_A);
    
}

// 0 matrix: assemble storage should return empty pointers
template <size_t n, typename T>
TEST_CASE(all0)
{
    ASSERT(n>0);
    
    // Define ELL Matrix
    SpMV::SparseMatrix_ELL<T> ptr_A(n, n);

    // initialize the keys
    
    for(size_t i=0;i<n;i++)
    {
        ptr_A.setCoefficient(i, i, static_cast<T>(0));    
    }

    // pass coefficients to assembleStorage()
    ptr_A.assembleStorage();
    // access the ELL quantities
    std::vector<int>* colIdx_obt = ptr_A.getColIdx(); // it should be int instead of size_t since -1 will also be an element in it
    std::vector<T>* val_obt = ptr_A.getVal();
    //size_t lmax = ptr_A.getLmax();
    
    // assert that the pointers are null pointers
    ASSERT( colIdx_obt.empty());
    ASSERT( val_obt.empty());

    // call disassembleStorage() and assert the matrix state
    ptr_A.disassembleStorage();
    delete(ptr_A);
    
}


template <typename T>
TEST_SUITE(ell_vec_edgecase) 
{
  // this case has no padding - does it now have padding
  TEST((banded_nxn_r0<9,5,3,5, T>)); 

  // cases with padding
  TEST((banded_nxn_r0<10,5,3,2, T>)); // r=0 for row which already had padding (i.e. nnz_r != nnz_rmax)
  TEST((banded_nxn_r0<10,5,3,3, T>)); // r=0 for row which which didn't ahve padding (i.e. nnz_r == nnz_rmax) 

  // how it handles only one element filled
  TEST((all0_but1<6,T>));

  // how it handles 0 matrix
  TEST((all0<6,T>));
} // add_sub_suite


int main()
{
  // Run the unit tests. If a test fails, the program will print failure info
  // and return 1.
  RUN_SUITE(ell_vec_compare<double>);
  RUN_SUITE(ell_vec_compare<float>);
  RUN_SUITE(ell_vec_edgecase<double>);
  RUN_SUITE(ell_vec_edgecase<float>);
  return 0; 
}