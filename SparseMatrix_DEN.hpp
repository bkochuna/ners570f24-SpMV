#pragma once

#include "SparseMatrix.hpp"
#include <vector>

namespace SpMV
{
    template <class fp_type>
    class SparseMatrix_DEN : public SparseMatrix<fp_type>
    {
    private:
        std::vector<std::vector<fp_type>> _A_mat;

    public:
        SparseMatrix_DEN(const int nrows, const int ncols);
        SparseMatrix_DEN();
        ~SparseMatrix_DEN();
        void assembleStorage(); //placeholder
        void disassembleStorage(); //placeholder
	
	// Accessor Methods
	inline size_t getNumRows() const { return this->_nrows; };
	inline size_t getNumCols() const { return this->_ncols; };
	fp_type getCoefficient(const size_t row, const size_t col) const;

    void matvec(std::vector<fp_type>& b, const std::vector<fp_type>& x);
    }; // class SparseMatrix_DEN
} // namespace SpMV
