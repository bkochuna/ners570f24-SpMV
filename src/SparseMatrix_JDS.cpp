#include "SparseMatrix_JDS.hpp"
#include <iostream>
#include <cstring> // For memcpy

namespace SpMV
{
    // Default constructor
    // Initializes the base class with 0 rows and columns
    template <class fp_type>
    SparseMatrix_JDS<fp_type>::SparseMatrix_JDS()
        : SparseMatrix<fp_type>::SparseMatrix(0, 0),
        perm_size(0), jds_col_size(0), jds_val_size(0)
    {
        std::cout << "Hello from SparseMatrix_JDS Default Constructor!\n";
    }

    // Parameterized constructor
    // Initializes the base class with given number of rows and columns, allocates and copies data, then calls assembleStorage
    template <class fp_type>
    SparseMatrix_JDS<fp_type>::SparseMatrix_JDS(
        const int nrows, const int ncols,
        size_t* p_perm, size_t p_perm_size,
        size_t* p_jds_col_ptr, size_t p_jds_col_size,
        fp_type* p_jds_values, size_t p_jds_val_size)
        : SparseMatrix<fp_type>::SparseMatrix(nrows, ncols),
          perm_size(p_perm_size), jds_col_size(p_jds_col_size), jds_val_size(p_jds_val_size)
    {
        std::cout << "Hello from SparseMatrix_JDS Parameterized Constructor!\n";

        // Allocate and copy perm array using std::make_unique
        if (perm_size > 0) {
            perm = std::make_unique<size_t[]>(perm_size);
            std::memcpy(perm.get(), p_perm, perm_size * sizeof(size_t));
        }

        // Allocate and copy jds_col_ptr array using std::make_unique
        if (jds_col_size > 0) {
            jds_col_ptr = std::make_unique<size_t[]>(jds_col_size);
            std::memcpy(jds_col_ptr.get(), p_jds_col_ptr, jds_col_size * sizeof(size_t));
        }

        // Allocate and copy jds_values array using std::make_unique
        if (jds_val_size > 0) {
            jds_values = std::make_unique<fp_type[]>(jds_val_size);
            std::memcpy(jds_values.get(), p_jds_values, jds_val_size * sizeof(fp_type));
        }

        // Call assembleStorage after initializing members
        assembleStorage();
    }

    // Destructor
    // Frees dynamically allocated memory for perm, jds_col_ptr, and jds_values arrays
    template <class fp_type>
    SparseMatrix_JDS<fp_type>::~SparseMatrix_JDS()
    {
        std::cout << "Hello from SparseMatrix_JDS Destructor!\n";
        // No need to manually delete memory; std::unique_ptr takes care of it.
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
    // Returns a const pointer to perm array
    template <class fp_type>
    const size_t* SparseMatrix_JDS<fp_type>::getPerm() const
    {
        return perm;
    }

    // Accessor for jds_col_ptr array
    // Returns a const pointer to jds_col_ptr array
    template <class fp_type>
    const size_t* SparseMatrix_JDS<fp_type>::getJdsColPtr() const
    {
        return jds_col_ptr;
    }

    // Accessor for jds_values array
    // Returns a const pointer to jds_values array
    template <class fp_type>
    const fp_type* SparseMatrix_JDS<fp_type>::getJdsValues() const
    {
        return jds_values;
    }

    // Explicitly instantiate the template classes for float and double types
    template class SparseMatrix_JDS<float>;
    template class SparseMatrix_JDS<double>;
} // namespace SpMV