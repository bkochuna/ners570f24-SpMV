#pragma once

#include "SparseMatrix.hpp"
#include <cassert> // For assert
#include <vector>  // For std::vector
#include <numeric> // For std::iota, std::find
#include <algorithm> // For std::stable_sort

namespace SpMV
{
    template <class fp_type>
    class SparseMatrix_JDS : public SparseMatrix<fp_type>
    {
    private:
        std::vector<size_t> perm;         // Vector holding permutation indices
        std::vector<size_t> jds_col_ptr;  // Vector holding JDS column pointers
        std::vector<fp_type> jds_values;  // Vector holding non-zero values
        std::vector<size_t> jd_ptr;      // Vector holding row pointers
        size_t max_nz_row;                // Maximum number of non-zero elements in a row


    public:
        // Default constructor
        SparseMatrix_JDS();

        // Parameterized constructor
        SparseMatrix_JDS(const int nrows, const int ncols,
                         const std::vector<size_t>& perm,
                         const std::vector<size_t>& jds_col_ptr,
                         const std::vector<fp_type>& jds_values,
                         const std::vector<size_t>& jd_ptr);

        ~SparseMatrix_JDS() = default; // Default destructor, as std::vector handles its own memory

        // Placeholder method declarations
        void assembleStorage() override;
        void disassembleStorage();
        void matvec(const fp_type* x, fp_type* y);

        // Utility sorting methods to use during assembly/disassembly
        template <typename T>
        inline void sortIndicesDescending(const std::vector<T> &v, std::vector<size_t> &idx) {
            // Initialize original index locations
            idx.resize(v.size());
            std::iota(idx.begin(), idx.end(), 0);

            // Sort indices based on comparing values in v
            // Use stable sort so that if two elements are equal, their original order is preserved
            std::stable_sort(idx.begin(), idx.end(), [&v](size_t i1, size_t i2) {return v[i1] > v[i2];});
        }
    };

    // Accessor methods
    // Returning the entire vectors
    inline const std::vector<size_t> getPerm() const { return perm; };                // access the vector holding permutation indices in read-only mode
    inline const std::vector<size_t> getJdsColPtr() const { return jds_col_ptr; };    // access the vector holding JDS column pointers in read-only mode
    inline const std::vector<fp_type> getJdsValues() const { return jds_values; };    // access the vector holding non-zero values in read-only mode
    inline const std::vector<size_t> getJdPtr() const { return jd_ptr; };             // access the vector holding the row pointers in read-only mode
    // Returning the SparseMatrix attributes
    inline const size_t getNRows() const { return this->_nrows; };   // number of rows in matrix in read-only mode
    inline const size_t getNCols() const { return this->_ncols; };   // number of columns in matrix in read-only mode
    // Returning specific value given [i,j] index
    const fp_type getVal(const size_t rowId, const size_t colId) const;  // access the element value in [rowId,colId] in read-only mode
} // namespace SpMV