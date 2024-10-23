// File: SparseMatrix_ELL.hpp
// Created: Oct 17,2024
//
// Description: Head file for ELL declarations
//
// Modifications:
// - Oct 17, 2024:  Created by Xianyu Liu, declared lmax, *colIdx[], *val,
//                  declared (default) constructor & destructor.
//                  Also declared view interface
//
//

#pragma once

#include "SparseMatrix.hpp"

namespace SpMV
{
    template <class fp_type>
    class SparseMatrix_ELL : public SparseMatrix<fp_type>
    {
    private:
        // Here gives the basic variables that ELL 
        // needs but the basic class doesn't include.
        //
        size_t  lmax    =   0;                                  // Max row length
        size_t  nnzs    =   0;                                  // number of nonzeros
        size_t  *colIdx =   nullptr;                            // ELL column indice
        fp_type *val    =   nullptr;                            // ELL values

    public:
        // (Default) Constructor & Destructor
        SparseMatrix_ELL(const int nrows, const int ncols);     // Constructor
        SparseMatrix_ELL();                                     // Default constructor
        virtual ~SparseMatrix_ELL();                            // Destructor

        void assembleStorage() {assert(false);};                // Assemble storage
        void MatVec() {assert(false);};                         // Multiplication
        void view() {assert(false);}                            // View

    }; // class SparseMatrix_ELL
} // namespace SpMV
