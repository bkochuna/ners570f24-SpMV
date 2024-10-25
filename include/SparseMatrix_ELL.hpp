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
#include <vector>
#include <memory>

namespace SpMV
{
    template <class fp_type>
    class SparseMatrix_ELL : public SparseMatrix<fp_type>
    {
    private:
        // Here gives the basic variables that ELL 
        // needs but the basic class doesn't include.
        //
        size_t   _lmax   = 0;                                   // Max row length
        std::unique_ptr< size_t [] > _colIdx = nullptr;         // ELL column indice
        std::unique_ptr< fp_type[] > _val    = nullptr;         // ELL values

        void _disassembleStorage();

    public:
        // (Default) Constructor & Destructor
        SparseMatrix_ELL(const int nrows, const int ncols);     // Constructor
        SparseMatrix_ELL();                                     // Default constructor
        virtual ~SparseMatrix_ELL();                            // Destructor

        void assembleStorage();                                 // Assemble storage
        void disassembleStorage() {this->_disassembleStorage();};
        void matvec(std::vector<fp_type> & b, const std::vector<fp_type> & x);       // Multiplication
        void view();                            // View

    // Accessor methods

        // Accessor for lmax
        size_t getLmax() const {
            return _lmax;
        }

        // Accessor for colIdx
        const size_t* getColIdx() const {
            return _colIdx.get(); // Return pointer to colIdx
        }

        // Accessor for val
        const fp_type* getVal() const {
            return _val.get(); // Return pointer to val
        }

    }; // class SparseMatrix_ELL
} // namespace SpMV
