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

    template <class fp_type>
    std::vector<fp_type> SparseMatrix_COO<fp_type>::matvec_COO(const std::vector<fp_type>& vec) const 
    {
    // Preconditions
    assert(vec.size() > 0 && "Input vector must not be empty");
    assert(vec.size() == this->_ncols && "Input vector dimension must match matrix columns");
    assert(this->_nnz > 0 && "Matrix must contain at least one non-zero element");
    
    // Class invariants - using _nnz for size checks since we're working with raw pointers
    assert(this->_val != nullptr && "Values array must not be null");
    assert(this->_I != nullptr && "Row indices array must not be null");
    assert(this->_J != nullptr && "Column indices array must not be null");
    
    // Validate index bounds
    for (size_t i = 0; i < this->_nnz; ++i) {
        assert(this->_I[i] >= 0 && this->_I[i] < this->_nrows && 
               "Row index out of bounds");
        assert(this->_J[i] >= 0 && this->_J[i] < this->_ncols && 
               "Column index out of bounds");
    }

    // Initialize result vector with proper size and zero values
    std::vector<fp_type> result_vector(this->_nrows, 0);
    
    // Perform matrix-vector multiplication
    for (size_t k = 0; k < this->_nnz; ++k) {
        result_vector[this->_I[k]] += vec[this->_J[k]] * this->_val[k];
    }
    
    // Postconditions
    assert(result_vector.size() == this->_nrows && 
           "Result vector size must match matrix rows");
    
    return result_vector;
    }

    // finish add

    template class SparseMatrix_COO<float>;
    template class SparseMatrix_COO<double>;
} // namespace SpMV
