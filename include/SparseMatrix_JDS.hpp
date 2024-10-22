#pragma once

#include "SparseMatrix.hpp"
#include <iostream>
#include <vector>

namespace SpMV{
    template <class fp_type>
    class SparseMatrix_CSR : public SparseMatrix<fp_type> {
    private:
        std::vector<size_t> rowIdx;
        std::vector<size_t> colIdx;
        std::vector<fp_type> values;

    public:
        SparseMatrix_CSR(const size_t nrows, const size_t ncols);
        void viewCSR(); // View the CSR representation and output the full matrix
        void assembleStorage() = {
          std::cout<<"hello from assembleStorage"<<std::endl;
        };
        void disassembleStorage() = {
          std::cout<<"hello from disassembleStorage"<<std::endl;
        };
    }; // class SparseMatrix_JDS

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
