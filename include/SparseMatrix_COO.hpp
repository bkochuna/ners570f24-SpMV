#pragma once

#include "SparseMatrix.hpp" // inherit from abstact SparseMatrix class
#include <vector>

namespace SpMV
{
    template <class fp_type>
    class SparseMatrix_COO : public SparseMatrix<fp_type> // COO is a derived class that publicly inherits from SparseMatrix<fp_type>
    // protected members of SparseMatrix (_nrows, _ncols, _nnz) are accessible in SparseMatrix_COO
    {
    private:
        size_t *_I    = nullptr;
        size_t *_J    = nullptr;
        fp_type *_val = nullptr;

    public:
        SparseMatrix_COO(const int nrows, const int ncols); // Parametrized Constructor
        // add:
        SparseMatrix_COO(); // default constructor
        ~SparseMatrix_COO();
        
	    void assembleStorage() override;
	    void disassembleStorage();
        
	/// getter (optional)
        size_t* getRowIndices() const {return _I;}
        size_t* getColIndices() const {return _J;}
        fp_type* getValues() const {return _val;}

        /// some future interfaces:
        void viewCOO() const;
        std::vector<fp_type> matvec_COO(const std::vector<fp_type>& vec) const;

    }; // class SparseMatrix_COO
} // namespace SpMV
