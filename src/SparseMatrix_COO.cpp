#include "SparseMatrix_COO.hpp"

#include <iostream>

namespace SpMV
{
    template <class fp_type>
    SparseMatrix_COO<fp_type>::SparseMatrix_COO(const int nrows, const int ncols) :
         SparseMatrix<fp_type>::SparseMatrix(nrows, ncols)
    {
        std::cout << "Hello from SparseMatrix_COO Constructor!\n";
    }

    template class SparseMatrix_COO<float>;
    template class SparseMatrix_COO<double>;
} // namespace SpMV
