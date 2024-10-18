#include "SparseMatrix_DEN.hpp"

#include <iostream>

namespace SpMV
{
    template <class fp_type>
    SparseMatrix_DEN<fp_type>::SparseMatrix_DEN(const int nrows, const int ncols) :
         SparseMatrix<fp_type>::SparseMatrix(nrows, ncols)
    {
        std::cout << "Hello from SparseMatrix_DEN Constructor!\n";
        this->nrows = nrows;
        this->ncols = ncols;
        assembleStorage();

    }

    SparseMatrix_DEN<fp_type>::SparseMatrix_DEN() :
         SparseMatrix<fp_type>::SparseMatrix(nrows, ncols)
    {
        std::cout << "Hello from SparseMatrix_DEN Default Constructor!\n";
        this->nrows = 1;
        this->ncols = 1;
        assembleStorage();

    }


    template class SparseMatrix_DEN<float>;
    template class SparseMatrix_DEN<double>;
} // namespace SpMV
