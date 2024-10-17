#include "../include/SparseMatrix.hpp"

#include <iostream>

namespace SpMV
{
    
    template <class fp_type>
    SparseMatrix<fp_type>::SparseMatrix(const size_t nrows, const size_t ncols) :
        _nrows(nrows), _ncols(ncols)
    {
        assert(this->_state == undefined);
        std::cout << "Hello from SparseMatrix Constructor!\n";
        std::cout << "Size is (nrows,ncols) = (" 
            << this->_nrows << "," << this->_ncols << ")\n"; 
        this->_state = initialized;
        assert(this->_state == initialized);
    }

    template <class fp_type>
    SparseMatrix<fp_type>::~SparseMatrix()
    {
        std::cout << "Hello from SparseMatrix Destructor!\n";
    }

    template <class fp_type>
    void SparseMatrix<fp_type>::setCoefficient(const size_t row, const size_t col, const fp_type values)
    {
        assert(this->_state != undefined);
        assert(col < this->_ncols);
        assert(row < this->_nrows);

        this->_state = building;

        //Store value
        this->_buildCoeff[std::make_pair(row,col)] = values;
        this->_nnz = this->_buildCoeff.size();

        std::cout << "i,j,aij    " << row << ',' << col << ',' << values << '\n';

        assert(this->_state == building);
    }

    template class SparseMatrix<float>;
    template class SparseMatrix<double>;
} // namespace SpMV
