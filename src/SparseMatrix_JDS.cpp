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
    // template <class fp_type>
    // SparseMatrix_JDS<fp_type>::~SparseMatrix_JDS()
    // {
    //     std::cout << "Hello from SparseMatrix_JDS Destructor!\n";
    // }

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
    void SparseMatrix_JDS<fp_type>::matvec(std::vector<fp_type>& b, const std::vector<fp_type>& x)
    {
        // Placeholder implementation
        std::cout << "matvec method called for SparseMatrix_JDS\n";
        //initialize b vector with 0.0
        std::fill(b.begin(), b.end(), 0.0);
        //loop through arrays to access matrix values and multiply by appropriate value in x vector
        //have result assigned to b vector
        for (size_t i = 0; i < this->max_nz_row; i++){
            for (size_t j = this->jd_ptr[i]; this->jd_ptr[i+1]-1; j++){
                size_t k = j - this->jd_ptr[j]+1;
                b[this->perm[k]] += this->jds_values[j] * x[this->jds_col_ptr[j]];
            }
        }

        //fortran ex code

        //do i = 1, maxval(nnz_row)
        //    ! Loop over the non-zero entries in this row
        //    do j = iter(i), iter(i + 1) - 1
        //        k = j - iter(i)+1
        //        ! Accumulate the result
        //        b(row_order(k)) = b(row_order(k)) + jds_val(j) * x(jds_cols(j))
        //    end do
        //end do

    }

    // Accessor to access the element value in [rowId,colId] in read-only mode
    template <class fp_type>
    const fp_type SparseMatrix_JDS<fp_type>::getVal(const size_t rowId, const size_t colId) const
    {
        // assert state of the matrix
        assert(this->_state==assembled);
        // ensure requested rowID and colID are valid
        assert(rowId < this->_nrows);
        assert(colId < this->_ncols);

        // define req. index in jds_values as indx
        size_t indx = std::numeric_limits<size_t>::max();  // default is invalid index
        
        // find the index of the rowID in perm
        auto it = std::find(perm.begin(),perm.end(),rowId);
        size_t rid = std::distance(perm.begin(),it); // rid is always defined since assert condition ensures this
        for(size_t j=0;j<jd_ptr.size();j++)
        {
            size_t cid = jd_ptr[j] + rid;
            if (cid < jds_col_ptr.size())
            {
                if (j!=jd_ptr.size()-1 && cid < jd_ptr[j+1] )
                {
                    if(jds_col_ptr[cid]==colId)
                    {
                        indx = cid;
                        break;
                    }
                }
                else{
                    if(jds_col_ptr[cid]==colId)
                    {
                        indx = cid;
                        break;
                    }
                }
            } 
        }

        if (indx==std::numeric_limits<size_t>::max())
        {
            std::cout << "No value stored in value array corr. to ["<< rowId <<","<<colId<<"]. Assuming it to be 0 and returning 0"<< std::endl;
            return static_cast<fp_type>(0);
        }
        else
        {
            std::cout << "Value stored in value array corr. to ["<< rowId <<","<<colId<<"]. Returning it"<< std::endl;
            return jds_values[indx];
        }
    }
    

    // Explicitly instantiate the template classes for float and double types
    template class SparseMatrix_JDS<float>;
    template class SparseMatrix_JDS<double>;
} // namespace SpMV