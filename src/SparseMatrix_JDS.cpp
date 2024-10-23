#include "SparseMatrix_JDS.hpp"
#include <iostream>
#include <algorithm> // for std::find and std::distance

namespace SpMV
{
    // Default constructor
    // Initializes the base class with 0 rows and columns
    template <class fp_type>
    SparseMatrix_JDS<fp_type>::SparseMatrix_JDS()
        : SparseMatrix<fp_type>::SparseMatrix(0, 0)
    {
        std::cout << "Hello from SparseMatrix_JDS Default Constructor!\n";
    }

    // Parameterized constructor
    // Initializes the base class with given number of rows and columns, copies the provided data, then calls assembleStorage
    template <class fp_type>
    SparseMatrix_JDS<fp_type>::SparseMatrix_JDS(
        const int nrows, const int ncols,
        const std::vector<size_t>& p_perm,
        const std::vector<size_t>& p_jds_col_ptr,
        const std::vector<fp_type>& p_jds_values,
        const std::vector<size_t>& p_jd_ptr)
        : SparseMatrix<fp_type>::SparseMatrix(nrows, ncols),
          perm(p_perm),
          jds_col_ptr(p_jds_col_ptr),
          jds_values(p_jds_values),
          jd_ptr(p_jd_ptr)
    {
        std::cout << "Hello from SparseMatrix_JDS Parameterized Constructor!\n";

        // Call assembleStorage after initializing members
        assembleStorage();
    }

    // Destructor
    // Automatically handles resource cleanup via standard destructors
    template <class fp_type>
    SparseMatrix_JDS<fp_type>::~SparseMatrix_JDS()
    {
        std::cout << "Hello from SparseMatrix_JDS Destructor!\n";
    }

    // Placeholder for assembleStorage method
    template <class fp_type>
    void SparseMatrix_JDS<fp_type>::assembleStorage()
    {
        // First check state of the matrix; only do something if the matrix is "building"
        // Default case not necessary since there are only 4 possible options
        switch (this->_state)
        {
        case undefined:
            std::cerr << "Error: SparseMatrix_JDS in undefined state\n";
            assert(false);
            break;
        case initialized:
            std::cerr << "Error: SparseMatrix_JDS in initialized state\n";
            assert(false);
            break;
        case building:
            // This is only valid state to proceed, so just break
            break;
        case assembled:
            // Just do nothing since matrix is already assembled
            return;
        }

        // Calculate the number of nonzeros in each row
        std::vector<size_t> row_cts(this->_nrows, 0), sorted_row_cts(this->_nrows, 0);

        // Iterate through all items in _buildCoeff and count the number of nonzeros in each row
        for (auto& item : this->_buildCoeff) row_cts[item.first.first]++;

        // Sort rows by size and get counts, also get max number of nonzeros in a row
        this->sortIndicesDescending<size_t>(row_cts, this->perm);
        for (size_t i=0; i<this->_nrows; i++) sorted_row_cts[i] = row_cts[this->perm[i]];
        this->max_nz_row = *std::max_element(row_cts.begin(), row_cts.end());

        // Now we need to figure out nnz-index where each col of row zero starts
        // Traverse sorted row_cts in reverse
        this->jd_ptr.resize(this->max_nz_row+1);
        this->jd_ptr[0] = 0;
        size_t iR = this->_nrows-1;
        size_t num_rows_tmp;
        for (size_t nz_ct=0; nz_ct<=this->max_nz_row; nz_ct++) {
            // First check if a row exists with exactly nz_ct non-zeros
            if (sorted_row_cts[iR] == nz_ct) {
                // Count how many rows have exactly nz_ct non-zeros
                iR--;
                num_rows_tmp = 1;
                while (sorted_row_cts[iR] == nz_ct) {
                    num_rows_tmp++;
                    iR--;
                }
                // Now we know that num_rows rows have exactly nz_ct non-zeros
                this->jd_ptr[nz_ct+1] = this->jd_ptr[nz_ct] + iR + 1;		
            }
            else {
                // There are no rows with exactly nz_ct non-zeros
                this->jd_ptr[nz_ct+1] = this->jd_ptr[nz_ct] + iR + 1;
            }
        }

        // jd_ptr gives us mapping from sorted row index to original
        // Let's calculate mapping from original idx to sorted to use when filling in jds_col_ptr and jds_values
        // Might not bee a need for this extra array, but keeping it here for ease of understanding
        std::vector<size_t> iR_rev_map(this->_nrows);
        for (size_t i=0; i<this->_nrows; i++) iR_rev_map[perm[i]] = i;
        
        // Now repass through COO and place elements in appropriate spot in jds_col_ptr and jds_values
        this->jds_col_ptr.resize(this->_nnz);
        this->jds_values.resize(this->_nnz);

        std::vector<size_t> num_added_rows(this->_nrows, 0);
        size_t iR_sorted;			// iR already declared earlier in function
        for (auto& item : this->_buildCoeff) {
            // Get original row index of element
            iR = item.first.first;
            
            // Get row index in sorted row cts
            iR_sorted = iR_rev_map[iR];

            // Store element in appropriate spot in col-major order
            this->jds_col_ptr[jd_ptr[num_added_rows[iR_sorted]] + iR_sorted] = item.first.second;
            this->jds_values[jd_ptr[num_added_rows[iR_sorted]] + iR_sorted] = item.second;
            
            num_added_rows[iR_sorted]++;
        }

        // Update the state of the matrix to assembled and deallocate the _buildCoeff map
        this->_buildCoeff.clear();
        this->_state = assembled;
    }

    // Placeholder for disassembleStorage method
    template <class fp_type>
    void SparseMatrix_JDS<fp_type>::disassembleStorage()
    {
        // First check state of the matrix; only do something if the matrix is "assembled"
        if (this->_state != assembled)
        {
            std::cerr << "Error: SparseMatrix_JDS not in assembled state\n";
            assert(false);
        }

        // Change the state of the matrix to building
        this->_state = building;

        // Iterate through all non-zero elements and add it to the _buildCoeff map
        size_t i_jdrow, iR, iC;
        fp_type val;
        for (size_t i_jdcol=0; i_jdcol<this->max_nz_row; i_jdcol++) {
            for (size_t i_nnz=this->jd_ptr[i_jdcol]; i_nnz<this->jd_ptr[i_jdcol+1]; i_nnz++) {
                i_jdrow = i_nnz - this->jd_ptr[i_jdcol];
                iR = this->perm[i_jdrow];
                iC = this->jds_col_ptr[i_nnz];
                val = this->jds_values[i_nnz];
                this->setCoefficient(iR, iC, val);
            }
        }

        // Deallocate the jds-specific vectors and set max_nz_row to 0
        this->perm.clear();
        this->jds_col_ptr.clear();
        this->jds_values.clear();
        this->jd_ptr.clear();
        this->max_nz_row = 0;
    }

    // Placeholder for matvec method
    template <class fp_type>
    void SparseMatrix_JDS<fp_type>::matvec(const fp_type* x, fp_type* y)
    {
        // Placeholder implementation
        std::cout << "matvec method called for SparseMatrix_JDS (Not Implemented)\n";
        assert(false);
    }

    // Explicitly instantiate the template classes for float and double types
    template class SparseMatrix_JDS<float>;
    template class SparseMatrix_JDS<double>;
} // namespace SpMV