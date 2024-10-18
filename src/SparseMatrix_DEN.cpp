#include "SparseMatrix_DEN.hpp"

#include <iostream>

namespace SpMV
{
    template <class fp_type>
    SparseMatrix_DEN<fp_type>::SparseMatrix_DEN(const int nrows, const int ncols) :
         SparseMatrix<fp_type>::SparseMatrix(nrows, ncols)
    {
        std::cout << "Hello from SparseMatrix_DEN Constructor!\n";
        this->_nrows = nrows;
        this->_ncols = ncols;
        assembleStorage();

    }

    template <class fp_type>
    SparseMatrix_DEN<fp_type>::SparseMatrix_DEN() :
         SparseMatrix<fp_type>::SparseMatrix(1, 1)
    {
        std::cout << "Hello from SparseMatrix_DEN Default Constructor!\n";
        this->setCoefficient(0, 0, 0.0);
        assembleStorage();

    }

    template <class fp_type>
    SparseMatrix_DEN<fp_type>::~SparseMatrix_DEN()
    {
        std::cout << "Hello from SparseMatrix_DEN Destuctor!\n";
        disassembleStorage();
    }

    template class SparseMatrix_DEN<float>;
    template class SparseMatrix_DEN<double>;
} // namespace SpMV
