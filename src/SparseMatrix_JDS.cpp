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
        const std::vector<fp_type>& p_jds_values)
        : SparseMatrix<fp_type>::SparseMatrix(nrows, ncols),
          perm(p_perm),
          jds_col_ptr(p_jds_col_ptr),
          jds_values(p_jds_values)
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

    // Accessor for perm array
    template <class fp_type>
    const std::vector<size_t>& SparseMatrix_JDS<fp_type>::getPerm() const
    {
        return perm;
    }

    // Accessor for jds_col_ptr array
    template <class fp_type>
    const std::vector<size_t>& SparseMatrix_JDS<fp_type>::getJdsColPtr() const
    {
        return jds_col_ptr;
    }

    // Accessor for jds_values array
    template <class fp_type>
    const std::vector<fp_type>& SparseMatrix_JDS<fp_type>::getJdsValues() const
    {
        return jds_values;
    }

    // Explicitly instantiate the template classes for float and double types
    template class SparseMatrix_JDS<float>;
    template class SparseMatrix_JDS<double>;
} // namespace SpMV