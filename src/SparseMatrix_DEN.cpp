#include "SparseMatrix_DEN.hpp"

#include <iostream>

namespace SpMV
{
    template <class fp_type>
    SparseMatrix_DEN<fp_type>::SparseMatrix_DEN(const int nrows, const int ncols) :
         SparseMatrix<fp_type>::SparseMatrix(nrows, ncols)
    {
        std::cout << "Hello from SparseMatrix_DEN Constructor!\n";
        
    }

    template class SparseMatrix_DEN<float>;
    template class SparseMatrix_DEN<double>;
} // namespace SpMV
