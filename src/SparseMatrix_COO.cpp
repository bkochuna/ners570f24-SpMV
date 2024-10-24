#include "SparseMatrix_COO.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

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
        delete[] this->_I;
        delete[] this->_J;
        delete[] this->_val;
        std::cout << "Hello from SparseMatrix_COO Destructor!\n";
    }

    template <class fp_type> // assemble storage
    void SparseMatrix_COO<fp_type>::assembleStorage()
    {
        assert(this->_state == building);

        this->_I = new size_t[this->_nnz*sizeof(size_t)];
        this->_J = new size_t[this->_nnz*sizeof(size_t)];
        this->_val = new fp_type[this->_nnz*sizeof(fp_type)];

        int index = 0;
        for (const auto& aij : this->_buildCoeff)
        {
            this->_I[index] = aij.first.first;
            this->_J[index] = aij.first.second;
            this->_val[index] = aij.second;
            index += index;
        }
        this->_buildCoeff.clear();

        this->_state = assembled;
        assert(this->_state == assembled);
    }

    template <class fp_type> // disassemble storage
    void SparseMatrix_COO<fp_type>::disassembleStorage()
    {
        assert(this->_state == assembled);

        size_t len = this->_nnz; // setCoefficient updates _nnz each call
        for (size_t index = 0; index < len; ++index)
        {
            this->_buildCoeff[std::make_pair(this->_I[index],this->_J[index])] = this->_val[index];
        }
        delete[] this->_I;
        this->_I = nullptr;
        delete[] this->_J;
        this->_J = nullptr;
        delete[] this->_val;
        this->_val = nullptr;

        this->_state = building;
        assert(this->_state == building);
    }
    /*
     * This method prints the content of the matrix to the command line and to a file.
     * It requires that the matrix is built or assembles and will throw an error if the
     * matrix is only initialized. 
     * R equires that the nrows, ncols, I, J, val variables all are set
     */
    template <class fp_type>
    void SparseMatrix_COO<fp_type>::viewCOO() const
    {
        if(this->_state != building){
            std::ofstream file("view.out");

            size_t counter = 0;

            fp_type zero = 0.0;

            for(size_t i  = 0; i < this->_nrows; i ++)
            {
                for(size_t j = 0; j < this->_ncols; j ++)
                {
                    if(this->_J[counter] == j and this->_I[counter] == i)
                    {
                        file << std::to_string(this->_val[counter]);
                        counter++;
                    } else 
                    {
                        file << std::to_string(zero);
                    }
                    file << "\n";
                }
            }
            std::cout << "Wrote matrix to view.out\n";
        }
    }

    template <class fp_type>
    std::vector<fp_type> SparseMatrix_COO<fp_type>::matvec_COO(const std::vector<fp_type>& vec) const
    {
        std::cout << "Hello from SparseMatrix_COO matvec!\n";
        return vec;
    }

    // finish add

    template class SparseMatrix_COO<float>;
    template class SparseMatrix_COO<double>;
} // namespace SpMV
