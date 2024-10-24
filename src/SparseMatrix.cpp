#include "SparseMatrix.hpp"

#include <iostream>

// SparseMatrix.cpp: defines the implementation of methods in SparseMatrix.hpp
namespace SpMV
{
    // SparseMatrix Constructor:============================================
    // ==================================================================
    template <class fp_type>
    SparseMatrix<fp_type>::SparseMatrix(const int nrows, const int ncols) :
        _nrows(nrows), _ncols(ncols)
    {
        assert(this->_state == undefined);
        std::cout << "Hello from SparseMatrix Constructor!\n";
        std::cout << "Size is (nrows,ncols) = (" 
            << this->_nrows << "," << this->_ncols << ")\n"; 
        this->_state = initialized;
        assert(this->_state == initialized);
    }


    // SparseMatrix Destructor:=================================================
    // ==================================================================
    template <class fp_type>
    SparseMatrix<fp_type>::~SparseMatrix()
    {
        std::cout << "Hello from SparseMatrix Destructor!\n";
    }



    // setCoefficient Method:===================================================
    //==========================================================================
    template <class fp_type>
    void SparseMatrix<fp_type>::setCoefficient(const size_t row, const size_t col, const fp_type aij)
    {
        assert(this->_state != undefined); // make sure status is after initialized dimension step
        assert(col < this->_ncols); // ensure matrix size is within limit
        assert(row < this->_nrows);

        this->_state = building;

        //Store value:
        // Add or update the value `aij` at the position (row, col) in the map `_buildCoeff`.
        this->_buildCoeff[std::make_pair(row,col)] = aij;
        
        // Update the count of non-zero elements in the matrix.
        this->_nnz = this->_buildCoeff.size();

        std::cout << "i,j,aij    " << row << ',' << col << ',' << aij << '\n';

        assert(this->_state == building);
    }

    template class SparseMatrix<float>;
    template class SparseMatrix<double>;
} // namespace SpMV
