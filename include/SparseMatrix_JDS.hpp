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
} // namespace SpMV
