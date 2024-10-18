#pragma once

#include "SparseMatrix.hpp" // inherit from abstact SparseMatrix class

namespace SpMV
{
    template <class fp_type>
    class SparseMatrix_COO : public SparseMatrix<fp_type> // COO is a derived class that publicly inherits from SparseMatrix<fp_type>
    // protected members of SparseMatrix (_nrows, _ncols, _nnz) are accessible in SparseMatrix_COO
    {
    private:
        size_t *I    = nullptr;
        size_t *J    = nullptr;
        fp_type *val = nullptr;

    public:
        SparseMatrix_COO(const int nrows, const int ncols); // Parametrized Constructor
        // add:
        SparseMatrix_COO(); // default constructor
        ~SparseMatrix_COO();
        /// getter (optional)
        size_t* getRowIndices() const {return I;}
        size_t* getColIndices() const {return J;}
        fp_type* getValues() const {return val;}
        /// finish add

        void assembleStorage() {};
    }; // class SparseMatrix_COO
} // namespace SpMV
