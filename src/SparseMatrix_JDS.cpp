#include "SparseMatrix_JDS.hpp"
#include <iostream>

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
        // Placeholder implementation
        std::cout << "assembleStorage method called for SparseMatrix_JDS (Not Implemented)\n";
        assert(false);
    }

    // Placeholder for disassembleStorage method
    template <class fp_type>
    void SparseMatrix_JDS<fp_type>::disassembleStorage()
    {
        // Placeholder implementation
        std::cout << "disassembleStorage method called for SparseMatrix_JDS (Not Implemented)\n";
        assert(false);
    }

    // Placeholder for matvec method
    template <class fp_type>
    void SparseMatrix_JDS<fp_type>::matvec(const fp_type* x, fp_type* y)
    {
        // Placeholder implementation
        std::cout << "matvec method called for SparseMatrix_JDS (Not Implemented)\n";
        assert(false);
    }

    // Accessor to access the element value in [rowId,colId] in read-only mode
    template <class fp_type>
    const fp_type SparseMatrix_JDS<fp_type>::getVal(const size_t rowId, const size_t colId) const
    {
        // ensure requested rowID and colID are valid
        assert(rowId < this->_nrows);
        assert(colId < this->_ncols);

        // define req. index in jds_values as indx
        size_t indx = std::numeric_limits<size_t>::max();  // default is invalid index
        
        // find the index of the rowID in perm
        auto it = std::find(perm.begin(),perm.end(),rowID);
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