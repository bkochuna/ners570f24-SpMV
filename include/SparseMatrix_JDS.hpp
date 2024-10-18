#pragma once

#include "SparseMatrix.hpp"
#include <cassert> // For assert
#include <memory>  // For std::unique_ptr

namespace SpMV
{
    template <class fp_type>
    class SparseMatrix_JDS : public SparseMatrix<fp_type>
    {
    private:
        std::unique_ptr<size_t[]> perm; // Array holding permutation indices
        std::unique_ptr<size_t[]> jds_col_ptr; // Array holding JDS column pointers
        std::unique_ptr<fp_type[]> jds_values; // Array holding non-zero values

        // Sizes of the respective arrays
        size_t perm_size = 0;
        size_t jds_col_size = 0;
        size_t jds_val_size = 0;

    public:
        // Default constructor
        SparseMatrix_JDS();

        // Parameterized constructor
        SparseMatrix_JDS(const int nrows, const int ncols,
                         size_t* perm, size_t perm_size,
                         size_t* jds_col_ptr, size_t jds_col_size,
                         fp_type* jds_values, size_t jds_val_size);

        ~SparseMatrix_JDS();

        // Placeholder method declarations
        void assembleStorage() override;
        void disassembleStorage();
        void matvec(const fp_type* x, fp_type* y);

        // Accessor methods
        const size_t* getPerm() const;
        const size_t* getJdsColPtr() const;
        const fp_type* getJdsValues() const;
    }; // class SparseMatrix_JDS
} // namespace SpMV