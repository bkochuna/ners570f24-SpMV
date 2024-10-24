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
        delete[]    _colIdx;
        delete[]    _val;
        std::cout << "Byebye from SparseMatrix_ELL Destructor~\n";
    }

    template <class fp_type>
    void SparseMatrix_ELL<fp_type>::view()
    {
        assert(this->_state == assembled);
        if(this->_nnz==0)
        {
            std::cout << "Matrix is empty." << std::endl;
        }
        else
        {
            for(size_t i=0;i < this->_nrows; i++)
            {
                std::cout << "Col Indices: [";
                for (size_t j = 0; j < this->_lmax - 1; j++)
                {
                    std::cout << this->_colIdx[this->_lmax * i + j] << ", ";
                }
                std::cout << this->_colIdx[this->_lmax * i + this->_lmax -1] << "], Values: [";
                for (size_t j = 0; j < this->_lmax - 1; j++)
                {
                    std::cout << this->_val[this->_lmax * i + j] << ", ";
                }
                std::cout << this->_val[this->_lmax * i + this->_lmax -1] << "]" << std::endl;                
            }
        }
    }

    //matvec function 
    template <class fp_type>
    void SparseMatrix_ELL<fp_type>::matvec(std::vector<fp_type> & b, const std::vector<fp_type> & x){

        //definitions
        size_t count = 0;
        //main loop
        for (size_t i; i < this->_nnz; ++i){
            //multiply and increment 
            //this assumes that if val[i] is padded it will return 0 when multiplied 
            b[count] = this->_val[i] * x[this->_colIdx[i]];
            count += 1;
            //if we have gone through the column completly, reset the counter
            if (count == this->_nrows){
                count = 0;
            }
        }
    }

    template class SparseMatrix_ELL<float>;
    template class SparseMatrix_ELL<double>;
} // namespace SpMV
