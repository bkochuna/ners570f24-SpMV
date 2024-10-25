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
#include <algorithm>

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
        //delete[]    _colIdx;
        //delete[]    _val;
        std::cout << "Byebye from SparseMatrix_ELL Destructor~\n";
    }

    template <class fp_type>
    void SparseMatrix_ELL<fp_type>::assembleStorage()
    {
        //Requirements/assumptions of the routine
        assert(this->_nnz > 0);
        assert(this->_state == MatrixState::building);

        //Find the maximum number of nonzeros in a row
        std::vector<size_t> nnz_row(this->_nrows,0);
        for (const auto& aij : this->_buildCoeff) {
            std::pair<size_t , size_t> ij = aij.first;
            size_t i = ij.first;
            nnz_row[i] += 1;
        }
        this->_lmax = std::distance(nnz_row.begin(),
                                    std::max_element(nnz_row.begin(),nnz_row.end())
                                   );
        nnz_row.clear();

        //Allocate storage for ELLPACK format
        this->_colIdx = std::make_unique< size_t [] >(this->_nrows*this->_lmax);
        this->   _val = std::make_unique< fp_type[] >(this->_nrows*this->_lmax);

        //Assign values to ELLPACK format variables
        std::vector<size_t> nnz_row_offset(this->_nrows,0);
        for (const auto& aij : this->_buildCoeff) {
            std::pair<size_t , size_t> ij = aij.first;
            size_t i = ij.first; 
            size_t j = ij.second;
            fp_type val = aij.second;

            this->_colIdx[this->_lmax*i + nnz_row_offset[i]] = static_cast<int>(j);
            this->   _val[this->_lmax*i + nnz_row_offset[i]] = val;

            nnz_row_offset[i] += 1;
        }

        //Define values for padded locations in colIdx and val
        for(size_t i=0; i<this->_nrows; i++)
        {
            if(nnz_row_offset[i] < this->_lmax)
            {
                for(size_t j=nnz_row_offset[i]; j < this->_lmax; j++)
                {
                    this->_colIdx[this->_lmax*i + j] = this->_ncols+1;
                    this->_val   [this->_lmax*i + j] = 0.0;
                }
            }
        }

        nnz_row_offset.clear();

        this->_buildCoeff.clear();
        this->_state = MatrixState::assembled;

        //Method ensures
        assert(this->_state == MatrixState::assembled);
    }

    template <class fp_type>
    void SparseMatrix_ELL<fp_type>::_disassembleStorage()
    {
        assert(this->_state == MatrixState::assembled);
        
        // Set state to building so we can call setCoefficient
        this->_state = MatrixState::building;
        for(size_t i=0; i < this->_nrows; i++)
        {
            for(size_t n=0; n < this->_lmax; n++)
            {
                size_t j = this->_colIdx[this->_lmax*i + n];
                if(j < this->_ncols+1)
                {
                    this->setCoefficient(i,j,this->_val[_lmax*i + n]);
                }
                    
            }
        }

        // Clear ELL variables
        this->_colIdx.reset();
        this->_val.reset();
        
        //DBC checks on assuring state of class
        assert(this->_state == MatrixState::building);
        assert(this->_colIdx == nullptr);
        assert(this->_val    == nullptr);
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
