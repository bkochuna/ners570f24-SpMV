#include "../include/SparseMatrix_CSR.hpp"
#include <fstream>
#include <vector>
#include <iostream>

namespace SpMV
{
    template <class fp_type>
    SparseMatrix_CSR<fp_type>::SparseMatrix_CSR(const size_t nrows, const size_t ncols) :
        SparseMatrix<fp_type>(nrows, ncols) {
        rowIdx.reserve(nrows); // initialization
        colIdx.reserve(nrows);
        values.reserve(nrows);
    }

    // Correct implementation of viewCSR
    template <class fp_type>
    void SparseMatrix_CSR<fp_type>::viewCSR() {
        // Assume the COO format is already stored in rowIdx, colIdx, values
        const size_t nnz = values.size();
        const size_t nrows = this->_nrows;
        const size_t ncols = this->_ncols;

        // Build rowPtr from COO format
        std::vector<size_t> rowPtr(nrows + 1, 0);

        // Count non-zeros per row
        for (size_t i = 0; i < nnz; ++i) {
            rowPtr[rowIdx[i] + 1]++;
        }

        // Cumulative sum to get the final row pointer
        for (size_t i = 0; i < nrows; ++i) {
            rowPtr[i + 1] += rowPtr[i];
        }

        // Build colIdx and values arrays in CSR format
        std::vector<size_t> next(nrows, 0);
        std::vector<size_t> csr_colIdx(nnz);
        std::vector<fp_type> csr_values(nnz);

        for (size_t i = 0; i < nnz; ++i) {
            size_t row = rowIdx[i];
            size_t idx = rowPtr[row] + next[row];
            csr_colIdx[idx] = colIdx[i];
            csr_values[idx] = values[i];
            next[row]++;
        }

        // Now print the full matrix
        std::vector<std::vector<fp_type>> A(nrows, std::vector<fp_type>(ncols, 0));
        for (size_t row = 0; row < nrows; ++row) {
            for (size_t i = rowPtr[row]; i < rowPtr[row + 1]; ++i) {
                A[row][csr_colIdx[i]] = csr_values[i];
            }
        }

        // Print the matrix to console
        for (const auto& row : A) {
            for (const auto& val : row) {
                std::cout << val << " ";
            }
            std::cout << "\n";
        }

        // Write the full matrix to a file
        std::ofstream outFile("viewCSR.txt");
        for (const auto& row : A) {
            for (const auto& val : row) 
                outFile << val << " ";
            outFile << "\n";
        }
        outFile.close();
    }

    // Explicit instantiation for float and double types
    template class SparseMatrix_CSR<float>;
    template class SparseMatrix_CSR<double>;

} // namespace SpMV
