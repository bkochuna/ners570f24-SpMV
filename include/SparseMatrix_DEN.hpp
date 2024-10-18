#pragma once

#include "SparseMatrix.hpp"

namespace SpMV
{
    template <class fp_type>
    class SparseMatrix_DEN : public SparseMatrix<fp_type>
    {
    private:
        size_t *nrows = nullptr;
        size_t *ncols = nullptr;
        size_t *nnz   = nullptr;
        fp_type *A_mat  = nullptr;

    public:
        SparseMatrix_DEN(const int nrows, const int ncols);
        void assembleStorage() {};
        void disassembleStorage() {};
    }; // class SparseMatrix_DEN
} // namespace SpMV
