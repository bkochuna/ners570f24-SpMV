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

     // Accessor of value
    template <class fp_type>
    fp_type SparseMatrix_ELL<fp_type>::getELLvalue(const size_t row, const size_t col) const
    {
	// Initialize
	fp_type ELLvalue = 0;

	// Check if the matrix is assembled
	if (this->_state==assembled) {

		// Determine if the value is found
		size_t isFound = 0;

		// Check if row and col are valid
		if (col > this->_ncols || row > this->_nrows) {
			throw std::invalid_argument("Error: Indices out of bound!");
		}

		// Find value corresponding to (row,col)
		for (size_t i = this->lmax * row; i < this->lmax * (row+1): ++i) {
			if (this->colIdx[i] == col && this->val[i] > 0) {
				isFound++;
				ELLvalue = val[i];
				break;
			}
		}

		// No value found
		if (isFound == 0) {
			std::cout << "No value assigned at (" << row << "," << col << ")" << std::endl;
		}
	}
	else {
		throw std::runtime_error("Invalid matrix state: matrix is not assembled!");
	}

	return ELLvalue;
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
                for (size_t j = 0; j < this->lmax - 1; j++)
                {
                    std::cout << this->colIdx[this->lmax * i + j] << ", ";
                }
                std::cout << this->colIdx[this->lmax * i + this->lmax -1] << "], Values: [";
                for (size_t j = 0; j < this->lmax - 1; j++)
                {
                    std::cout << this->val[this->lmax * i + j] << ", ";
                }
                std::cout << this->val[this->lmax * i + this->lmax -1] << "]" << std::endl;                
            }
        }
    }

    //matvec function 
    template <class fp_type>
    void SparseMatrix_ELL<fp_type>::matvec(std::vector<fp_type> & b, const std::vector<fp_type> & x){

        //definitions
        size_t count = 0;
        //main loop
        for (size_t i; i < this->nnzs; ++i){
            //multiply and increment 
            //this assumes that if val[i] is padded it will return 0 when multiplied 
            b[count] = this->val[i] * x[this->colIdx[i]];
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
