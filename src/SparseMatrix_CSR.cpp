#include "../include/SparseMatrix_CSR.hpp"
#include <fstream>
#include <vector>
#include <iostream>

// yulupeng: I'm doing the CSR constructor/destructor test 2024/10/21:
    // for who is doing "add a CSR class": where is the default constructor and destructor??

namespace SpMV 
{
    template <class fp_type>
    SparseMatrix_CSR<fp_type>::SparseMatrix_CSR(const size_t nrows, const size_t ncols) :
        SparseMatrix<fp_type>(nrows, ncols) {
        rowIdx.reserve(nrows); // initialization
        colIdx.reserve(nrows);
        values.reserve(nrows);
    }
    // asssemble storage for CSR
    template <typename fp_type>
    void SparseMatrix_CSR<fp_type>::assembleStorage()
    {
        // Ensure the matrix is in the "building" state
        if (this->_state != building)
        {
            std::cerr << "Error: Matrix not in building state during assembly\n";
            assert(false);
            return;
        }
        std::cout << "Hello from CSR_assembleStorage"<< std::endl;
        // Step 1: Initialize rowIdx to correct size (nrows + 1)
        rowIdx.resize(this->_nrows + 1, 0);

        // Step 2: Count non-zeros per row using the COO data in _buildCoeff
        for (auto& entry : this->_buildCoeff)
        {
            size_t row = entry.first.first;  // Row index from COO format
            rowIdx[row + 1]++;               // Increment the count for this row
        }

        // Step 3: Cumulative sum to construct rowIdx (CSR row pointer)
        for (size_t i = 1; i <= this->_nrows; ++i)
        {
            rowIdx[i] += rowIdx[i - 1];
        }

        // Step 4: Allocate memory for values[] and colIdx[]
        values.resize(this->_buildCoeff.size());
        colIdx.resize(this->_buildCoeff.size());

        // Step 5: Populate values[] and colIdx[] using the COO data
        std::vector<size_t> row_pos(this->_nrows, 0); // To track current position in each row
        for (auto& entry : this->_buildCoeff)
        {
            size_t row = entry.first.first;     // Row index from COO data
            size_t col = entry.first.second;    // Column index from COO data
            fp_type value = entry.second;       // Value from COO data

            size_t index = rowIdx[row] + row_pos[row];  // Calculate index in values[] and colIdx[]
            values[index] = value;
            colIdx[index] = col;

            row_pos[row]++;  // Move to the next position in this row
        }

        // Step 6: Clear the _buildCoeff map since we've now assembled the CSR format
        this->_buildCoeff.clear();

        // Step 7: Mark the matrix as assembled
        this->_state = assembled;
    }

    // dissamble storage for CSR
    template <typename fp_type>
    void SparseMatrix_CSR<fp_type>::disassembleStorage()
    {
        // Ensure the matrix is in the "assembled" state before disassembly
        if (this->_state != assembled)
        {
            std::cerr << "Error: Matrix not in assembled state during disassembly\n";
            assert(false);
            return;
        }
        std::cout << "Hello from CSR_dissassembleStorage"<< std::endl;
        // Change the state to "building" so that we can modify the matrix
        this->_state = building;

        // Step 1: Rebuild the _buildCoeff map from CSR format
        for (size_t row = 0; row < this->_nrows; ++row)
        {
            for (size_t i = rowIdx[row]; i < rowIdx[row + 1]; ++i)
            {
                size_t col = colIdx[i];     // Column index from CSR
                fp_type value = values[i];  // Non-zero value from CSR

                // Store the (row, col, value) tuple in the _buildCoeff map (COO format)
                this->_buildCoeff[{row, col}] = value;
            }
        }

        // Step 2: Clear CSR-specific data since we've now disassembled to COO format
        values.clear();
        colIdx.clear();
        rowIdx.clear();
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

    template <class fp_type>
    fp_type SparseMatrix_CSR<fp_type>::getCSRvalue(const size_t row, const size_t col) const {
        
        // Initialize the CSR Value to 0
        fp_type CSRValue = 0;

        // Check that CSR matrix is assembled, then grab value from CSR format
        if (this->_state==assembled) {
            // Flag for determining if the value was or was not found
            size_t FoundVal = 0;

            // Check to see if the column and row and within the matrix dimensions
            if (col > this->_ncols || row > this->_nrows) {
                // If they are not within the matrix dimensions, return an error
                throw std::invalid_argument("The given indices are out of the matrix dimensions!");
            }

            // Use the row to grab all the possible column indices in the specific row
            for (size_t i = rowIdx[row]; i < rowIdx[row+1]; i++) {
                // When looping over the column indices, see if any match col
                if (colIdx[i]==col) {
                    //If the colIdx matches col, update CSRValue, FoundVal flag, then break out.
                    FoundVal++;
                    CSRValue = values[i];
                    break;
                }
            }
            // Check to see if a value was found if the specified row
            if (FoundVal == 0) {
                // If no value was found, then print a message that no value is assigned at the given indices, returning 0
                std::cout << "There is no value assigned at (" << row << "," << col << "), returning 0" << std::endl;
            }

        } else {
            // Return an error if the matrix state is not assembled
            throw std::runtime_error("Incorrect matrix state: Make sure state is 'assembled' before running getCSRValue!");
        } 
        // Return the CSRValue at the very end
        return CSRValue;
    }

    // Explicit instantiation for float and double types
    template class SparseMatrix_CSR<float>;
    template class SparseMatrix_CSR<double>;

} // namespace SpMV
