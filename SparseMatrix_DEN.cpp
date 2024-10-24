#include "SparseMatrix_DEN.hpp"

#include <iostream>

namespace SpMV
{
    template <class fp_type>
    SparseMatrix_DEN<fp_type>::SparseMatrix_DEN(const int nrows, const int ncols) :
         SparseMatrix<fp_type>::SparseMatrix(nrows, ncols)
    {
        std::cout << "Hello from SparseMatrix_DEN Constructor!\n";
        this->_nrows = nrows;
        this->_ncols = ncols;
        assembleStorage();
    }

    template <class fp_type>
    SparseMatrix_DEN<fp_type>::SparseMatrix_DEN() :
         SparseMatrix<fp_type>::SparseMatrix(1, 1)
    {
        std::cout << "Hello from SparseMatrix_DEN Default Constructor!\n";
        this->setCoefficient(0, 0, 0.0);
        assembleStorage();

    }

    template <class fp_type>
    SparseMatrix_DEN<fp_type>::~SparseMatrix_DEN()
    {
        std::cout << "Hello from SparseMatrix_DEN Destuctor!\n";
        disassembleStorage();
    }

    template <class fp_type>
    fp_type SparseMatrix_DEN<fp_type>::getCoefficient(const size_t row, const size_t col) const
    {
        // ensures requested row and col are within range
	assert(row < this->_nrows);
	assert(col < this->_ncols);

	// uses the _buildCoeff variable from SparseMatrix class to find the row and col
	auto it = this->_buildCoeff.find(std::make_pair(row, col));

	// if row and col are within range
	if(it!=this->_buildCoeff.end())
	{
	    // return the coefficient 
	    return it->second;
	}
	else
	{
            std::cout << "Coefficient not found at (" << row << ", " << col << "). Returning 0." << std::endl;
	    return static_cast<fp_type>(0);
	}

    }	      

    template <class fp_type>
    void SparseMatrix_DEN<fp_type>::matvec(std::vector<fp_type>& b, const std::vector<fp_type>& x){
        // Check for size
        assert(this->_ncols == x.size());
        if (b.size() != this->_nrows) b.resize(this->_nrows);
        
        for (size_t i = 0; i < this->_nrows; i++){
            b[i] = 0.0;
            for (size_t j = 0; j < this->_ncols; j++){
                b[i] += this->_A_mat[i][j] * x[j];
            }
        }
	
    
	    std::cout << x.size() << std::endl;
    }

    template <class fp_type>
    void SparseMatrix_DEN<fp_type>::assembleStorage()
    {
	    // typename std::map<std::pair<size_t, size_t>, fp_type>::iterator it;
	    for (auto it = this->_buildCoeff.begin(); it != this->_buildCoeff.end(); it++)
	    {
		    size_t i = std::get<0>(it->first);
		    size_t j = std::get<1>(it->first);
		    fp_type A_val = it->second;
		    _A_mat[i][j] = A_val;
	    }
	    assert(this->_nrows>0);
	    assert(this->_ncols>0);
	    this->_state = assembled;
    }
    template <class fp_type>
    void SparseMatrix_DEN<fp_type>::disassembleStorage() {}


    template class SparseMatrix_DEN<float>;
    template class SparseMatrix_DEN<double>;
} // namespace SpMV
