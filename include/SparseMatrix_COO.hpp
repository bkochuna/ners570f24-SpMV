#pragma once

#include "SparseMatrix.hpp" // inherit from abstact SparseMatrix class
#include <vector>

namespace SpMV
{
    template <class fp_type>
    class SparseMatrix_COO : public SparseMatrix<fp_type> // COO is a derived class that publicly inherits from SparseMatrix<fp_type>
    // protected members of SparseMatrix (_nrows, _ncols, _nnz) are accessible in SparseMatrix_COO
    {
    private:
        size_t *idx_row    = nullptr;
        size_t *idx_col    = nullptr;
        fp_type *aij = nullptr;

    public:
        SparseMatrix_COO(const int nrows, const int ncols); // Parametrized Constructor
        // add:
        SparseMatrix_COO(); // default constructor
        ~SparseMatrix_COO();
        /// getter (optional)
        size_t* getRowIndices() const {return idx_row;}
        size_t* getColIndices() const {return idx_col;}
        fp_type* getValues() const {return aij;}

        /// some future interfaces (uncomment when implemented):
        void viewCOO() const;
        // std::vector<fp_type> matvec_COO(const std::vector<fp_type>& vec) const;

        /// finish add
        fp_type getCoefficient(const size_t row, const size_t col) const;

        void assembleStorage() {};
    }; // class SparseMatrix_COO
} // namespace SpMV
