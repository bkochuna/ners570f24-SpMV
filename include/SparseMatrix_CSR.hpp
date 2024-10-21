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
      
        // Access the base matrix attributes from the Sparse Matrix Class
        inline size_t      getNumNonZeros() const { return this-> _nnz}
        inline size_t      getNRows()       const { return this-> _nrows}
        inline size_t      getNCols()       const { return this-> _ncols}

        // Access the attributes from the CSR Class
        inline const std::vector<size_t>   getRowIdx() const { return rowIdx }
        inline const std::vector<size_t>   getColIdx() const { return colIdx }
        inline const std::vector<fp_type>  getValues() const { return values }

        void viewCSR(); // View the CSR representation and output the full matrix
//        void assembleStorage() override; // Declare the implementation
    }; // class SparseMatrix_COO
} // namespace SpMV
