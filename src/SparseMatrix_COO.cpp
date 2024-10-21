#include "SparseMatrix_COO.hpp"

#include <iostream>
#include <vector>
#include <stdexcept> // for std:invalid_argument and others
#include <format> // for std::format

namespace SpMV
{
    template <class fp_type>
    SparseMatrix_COO<fp_type>::SparseMatrix_COO(const int nrows, const int ncols) :
         SparseMatrix<fp_type>::SparseMatrix(nrows, ncols)
    {
        std::cout << "Hello from SparseMatrix_COO Parametrized Constructor!\n";
    }

    // add
    template <class fp_type> //default constructor
    SparseMatrix_COO<fp_type>::SparseMatrix_COO() : 
        SparseMatrix<fp_type>::SparseMatrix(1,1)
    {
        std::cout << "Hello from SparseMatrix_COO default constructor!\n";
        this->setCoefficient(0,0,0.0);
        this->assembleStorage();
    }

    template <class fp_type> //destructor
    SparseMatrix_COO<fp_type>::~SparseMatrix_COO()
    {
        delete[] this->idx_row;
        delete[] this->idx_col;
        delete[] this->aij;
        std::cout << "Hello from SparseMatrix_COO Destructor!\n";
    }


    template <class fp_type>
    void SparseMatrix_COO<fp_type>::viewCOO() const
    {
        std::cout << "Hello from view SparseMatrix_COO!\n";
    }
    // template <class fp_type>
    // std::vector<fp_type> SparseMatrix_COO<fp_type>::matvec_COO(const std::vector<fp_type>& vec) const
    // {
    //     std::cout << "Hello from SparseMatrix_COO matvec!\n";
    // }

    template <class fp_type>
    fp_type SparseMatrix_COO<fp_type>::getCoefficient(const size_t row, const size_t col) const
    {
        assert(this->_state == building || this->_state==assembled); // make sure status is after initialized dimension step
        assert(col < this->_ncols); // ensure matrix size is within limit
        assert(row < this->_nrows);

        std::cout << "Hello from SparseMatrix_COO getCoefficient!\n";
        
        // initialize output
        fp_type value;

        if (this->_state==building) {
            // read value from un-assembled map data
            // check if the (row,col) pair exists in the map
            if (this->_buildCoeff.find(std::make_pair(row,col)) == this->_buildCoeff.end()) {
                throw std::invalid_argument("Index pair does not exist in data");
            } else {
                value=this->_buildCoeff.at(std::make_pair(row,col));
            }

        } else if (this->_state==assembled) {
            // read value from assembled data
            // check if the (row,col) pair exists in the assembled data
            size_t foundSomething=0;
            for(size_t z = 0; z < this->_nnz; ++z){
                if(this->idx_row[z] == row && this->idx_col[z] == col){
                    if (foundSomething==0){
                        value=this->aij[z];
                        ++foundSomething;
                    } if (foundSomething>0) {
                        throw std::runtime_error(std::format("Duplicate entries present in matrix data at (row,col)= (%d,%d)",row,col));
                    }
                    break;
                }
            }

            // check if we found anything
            if (foundSomething==0) {
                throw std::invalid_argument(std::format("No entries in matrix data matching input (row,col)= (%d,%d)",row,col));
            }
        } else {
            throw std::runtime_error("Invalid matrix state: should be building or assembled");
        }

        return value;
    }


    // finish add

    template class SparseMatrix_COO<float>;
    template class SparseMatrix_COO<double>;
} // namespace SpMV
