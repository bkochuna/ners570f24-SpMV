// File: SparseMatrix_ELL.cpp
// Created: Oct 17,2024
//
// Description: ELL implements
//
// Modifications:
// - Oct 17, 2024:  Created by Xianyu Liu, implemented (default) constructor & destructor.
//
//

#include "SparseMatrix_ELL.hpp"

#include <iostream>

namespace SpMV
{   
    // Constructor
    // Inherited from SparseMatrix constructor
    // with nrows and ncols as inputs.
    template <class fp_type>
    SparseMatrix_ELL<fp_type>::SparseMatrix_ELL(const int nrows, const int ncols) :
         SparseMatrix<fp_type>::SparseMatrix(nrows, ncols)
    {
        std::cout << "Hello from SparseMatrix_ELL Constructor!\n";
    }

    // Default constructor
    // Inherited from SparseMatrix constructor
    // with nrows and ncols =1. No inputs.
    template <class fp_type>
    SparseMatrix_ELL<fp_type>::SparseMatrix_ELL() :
         SparseMatrix<fp_type>::SparseMatrix(1,1)
    {
        std::cout << "Hello from SparseMatrix_ELL >>Default<< Constructor!\n";
    }
    
    // Destructor
    // Delete ELL pointers and print.
    template <class fp_type>
    SparseMatrix_ELL<fp_type>::~SparseMatrix_ELL()
    {
        delete[]    colIdx;
        delete[]    val;
        std::cout << "Byebye from SparseMatrix_ELL Destructor~\n";
    }

    template class SparseMatrix_ELL<float>;
    template class SparseMatrix_ELL<double>;
} // namespace SpMV