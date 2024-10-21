#include "SparseMatrix_COO.hpp"

#include <iostream>
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
        delete[] this->I;
        delete[] this->J;
        delete[] this->val;
        std::cout << "Hello from SparseMatrix_COO Destructor!\n";
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
        std::cout << "Hello from view SparseMatrix_COO!\n";
    }
    template <class fp_type>
    std::vector<fp_type> SparseMatrix_COO<fp_type>::matvec_COO(const std::vector<fp_type>& vec) const
    {
        std::cout << "Hello from SparseMatrix_COO matvec!\n";
    }

    // finish add

    template class SparseMatrix_COO<float>;
    template class SparseMatrix_COO<double>;
} // namespace SpMV
