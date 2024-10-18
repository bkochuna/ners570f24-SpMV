#pragma once

#include "SparseMatrix.hpp"
#include <vector>

namespace SpMV
{
    template <class fp_type>
    class SparseMatrix_DEN : public SparseMatrix<fp_type>
    {
    private:
        std::vector<std::vector<fp_type>> A_mat;

    public:
        SparseMatrix_DEN(const int nrows, const int ncols);
        SparseMatrix_DEN();
        ~SparseMatrix_DEN();
    }; // class SparseMatrix_DEN
} // namespace SpMV
