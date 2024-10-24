#pragma once

#include <cassert>
#include <cstddef>
#include <map>
#include <utility>
// SparseMatrix.hpp header file: declares class and methods

namespace SpMV
{
    // Enumerations for matrix state to support Builder Pattern
    enum MatrixState { undefined, initialized, building, assembled };

    template <class fp_type> // SparseMatrix class is templated by fp_type floating-point allowing single(float) and double(double) precision
    class SparseMatrix
    {
    protected: 
        size_t _nrows = 0;
        size_t _ncols = 0;
        size_t _nnz   = 0;

        MatrixState _state = undefined;

        std::map<std::pair<size_t, size_t>, fp_type> _buildCoeff;
        // _buildCoeff is a map that stores matrix coefficients. They key is a pair(row,col)

    public:
        SparseMatrix(const int nrows, const int ncols); // constructor (cost)
        virtual ~SparseMatrix(); // destructor

        inline size_t      getNumRows()     const { return _nrows; };
        inline size_t      getNumCols()     const { return _ncols; };
        inline size_t      getNumNonZeros() const { return _nnz;   };
        inline MatrixState getState()       const { return _state; };

        void setCoefficient(const size_t row, const size_t col, const fp_type aij);
        virtual void assembleStorage() =0;
    }; // class SparseMatrix
} // namespace SpMV
