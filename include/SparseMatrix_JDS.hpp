#pragma once

#include "SparseMatrix.hpp"
#include <cassert> // For assert
#include <vector>  // For std::vector

namespace SpMV
{
    template <class fp_type>
    class SparseMatrix_JDS : public SparseMatrix<fp_type>
    {
    private:
        std::vector<size_t> perm;         // Vector holding permutation indices
        std::vector<size_t> jds_col_ptr;  // Vector holding JDS column pointers
        std::vector<fp_type> jds_values;  // Vector holding non-zero values

    public:
        // Default constructor
        SparseMatrix_JDS();

        // Parameterized constructor
        SparseMatrix_JDS(const int nrows, const int ncols,
                         const std::vector<size_t>& perm,
                         const std::vector<size_t>& jds_col_ptr,
                         const std::vector<fp_type>& jds_values);

        ~SparseMatrix_JDS() = default; // Default destructor, as std::vector handles its own memory

        // Placeholder method declarations
        void assembleStorage() override;
        void disassembleStorage();
        void matvec(const fp_type* x, fp_type* y);

        // Accessor methods
        const std::vector<size_t>& getPerm() const;
        const std::vector<size_t>& getJdsColPtr() const;
        const std::vector<fp_type>& getJdsValues() const;
    };
} // namespace SpMV